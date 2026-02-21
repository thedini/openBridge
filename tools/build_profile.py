#!/usr/bin/env python3
"""
Build device profiles from captured CAN traffic.

Analyzes a candump-format capture file, discovers unique devices by source
address, identifies their PGN sets, and generates OpenBridge device profile
JSON files.

Usage:
    python build_profile.py capture.log --output ../gui/device_profiles/

Dependencies:
    None (stdlib only)
"""

import argparse
import json
import os
import re
import sys
from collections import defaultdict
from pathlib import Path

# PGN → human-readable name mapping (common NMEA 2000 PGNs)
PGN_NAMES = {
    59392: "ISO Acknowledgement",
    59904: "ISO Request",
    60928: "ISO Address Claim",
    126992: "System Time",
    126993: "Heartbeat",
    126996: "Product Information",
    127245: "Rudder",
    127250: "Vessel Heading",
    127251: "Rate of Turn",
    127257: "Attitude",
    127488: "Engine Parameters Rapid",
    127489: "Engine Parameters Dynamic",
    127493: "Transmission Parameters",
    127497: "Trip Fuel Parameters",
    127505: "Fluid Level",
    127506: "DC Detailed Status",
    127508: "Battery Status",
    128259: "Speed Water Referenced",
    128267: "Water Depth",
    129025: "Position Rapid Update",
    129026: "COG & SOG Rapid Update",
    129029: "GNSS Position Data",
    129033: "Time & Date",
    129038: "AIS Class A Position Report",
    129039: "AIS Class B Position Report",
    129283: "Cross Track Error",
    129284: "Navigation Data",
    130306: "Wind Data",
    130310: "Environmental Parameters Outside",
    130311: "Environmental Parameters",
    130312: "Temperature",
    130313: "Humidity",
    130314: "Pressure",
    130316: "Temperature Extended Range",
    130576: "Trim Tab Status",
    130577: "Direction Data",
}

# PGN → NEMO sensor type mapping (where possible)
PGN_TO_SENSOR_TYPE = {
    127488: "engine_rpm",
    128267: "water_depth",
    127250: "heading",
    128259: "speed",
    127245: "rudder",
    130306: "wind_speed",  # Could also be wind_angle
    130311: "water_temp",  # Could also be other env params
    130310: "outside_temp",
    130314: "pressure",
    130313: "humidity",
    127508: "battery_volt",
    127505: "tank_level",
}

# Manufacturer code → name mapping
MFR_NAMES = {
    229: "Garmin",
    1851: "Raymarine",
    1857: "Simrad",
    275: "Navico",
    140: "Lowrance",
    1855: "Furuno",
    381: "B&G",
    144: "Mercury",
    1862: "Yamaha",
    174: "Volvo Penta",
    137: "Maretron",
}


def parse_candump_line(line):
    """Parse a candump-format line and extract CAN ID, DLC, and data.

    Expected formats:
        can1  09F11224   [8]  2E 99 99 00 00 64 F6 FD
        can2  09F80105   [8]  FF 7F FF FF FF FF FF FF
    """
    line = line.strip()
    if not line:
        return None

    # Match candump format: interface  canid   [dlc]  data
    m = re.match(
        r"\s*(\w+)\s+([0-9A-Fa-f]+)\s+\[(\d+)\]\s+((?:[0-9A-Fa-f]{2}\s*)+)",
        line,
    )
    if not m:
        return None

    interface = m.group(1)
    can_id = int(m.group(2), 16)
    dlc = int(m.group(3))
    data_hex = m.group(4).strip()
    data_bytes = bytes.fromhex(data_hex.replace(" ", ""))

    # Extract fields from 29-bit extended CAN ID
    source = can_id & 0xFF
    pgn = (can_id >> 8) & 0x3FFFF
    priority = (can_id >> 26) & 0x7

    return {
        "interface": interface,
        "can_id": can_id,
        "source": source,
        "pgn": pgn,
        "priority": priority,
        "dlc": dlc,
        "data": data_bytes,
    }


def parse_address_claim(data):
    """Parse ISO Address Claim (PGN 60928) data to extract manufacturer code."""
    if len(data) < 8:
        return None
    name = int.from_bytes(data[:8], "little")
    mfr_code = (name >> 21) & 0x7FF
    device_function = (name >> 40) & 0xFF
    device_class = (name >> 43) & 0x7F
    return {
        "manufacturer_code": mfr_code,
        "device_function": device_function,
        "device_class": device_class,
    }


def analyze_capture(filepath):
    """Analyze a capture file and return per-device information."""
    devices = defaultdict(lambda: {
        "pgns": defaultdict(lambda: {"count": 0, "first_data": None}),
        "manufacturer_code": None,
        "frame_count": 0,
    })

    total_frames = 0

    with open(filepath) as f:
        for line in f:
            frame = parse_candump_line(line)
            if not frame:
                continue

            total_frames += 1
            src = frame["source"]
            pgn = frame["pgn"]
            dev = devices[src]
            dev["frame_count"] += 1

            pgn_info = dev["pgns"][pgn]
            pgn_info["count"] += 1
            if pgn_info["first_data"] is None:
                pgn_info["first_data"] = frame["data"]

            # Extract manufacturer from Address Claim
            if pgn == 60928 and dev["manufacturer_code"] is None:
                claim = parse_address_claim(frame["data"])
                if claim:
                    dev["manufacturer_code"] = claim["manufacturer_code"]

    return devices, total_frames


def build_profile_for_device(addr, dev_info):
    """Build a device profile JSON for a single discovered device."""
    mfr_code = dev_info["manufacturer_code"] or 229
    mfr_name = MFR_NAMES.get(mfr_code, f"Unknown ({mfr_code})")

    # Find sensor PGNs (exclude protocol PGNs)
    protocol_pgns = {59392, 59904, 60928, 126992, 126993, 126996, 126998}
    sensor_pgns = [pgn for pgn in dev_info["pgns"] if pgn not in protocol_pgns]
    sensor_pgns.sort()

    # Build sensor slots (max 3 for NEMO)
    sensors = []
    for i, pgn in enumerate(sensor_pgns[:3]):
        sensor_type = PGN_TO_SENSOR_TYPE.get(pgn, None)
        if sensor_type:
            sensors.append({
                "slot": i,
                "type": sensor_type,
                "default_value": 0.5,
                "active": True,
            })

    # Build PGN list for description
    pgn_descriptions = []
    for pgn in sensor_pgns:
        name = PGN_NAMES.get(pgn, f"PGN {pgn}")
        count = dev_info["pgns"][pgn]["count"]
        pgn_descriptions.append(f"{name} ({pgn}, {count} msgs)")

    device_name = f"Device_{addr}_{mfr_name.replace(' ', '_')}"

    profile = {
        "name": device_name,
        "description": f"Auto-generated from capture. Source addr {addr}. "
                       f"PGNs: {', '.join(pgn_descriptions)}",
        "manufacturer": {"name": mfr_name, "code": mfr_code},
        "sensors": sensors,
        "source_address": addr,
        "discovered_pgns": sensor_pgns,
        "total_frames": dev_info["frame_count"],
    }

    return device_name, profile


def main():
    parser = argparse.ArgumentParser(description="Build device profiles from CAN capture")
    parser.add_argument("capture_file", help="Candump-format capture file")
    parser.add_argument("--output", "-o", default=".", help="Output directory for profile JSONs")
    parser.add_argument("--min-frames", type=int, default=5,
                        help="Minimum frames to include a device (default: 5)")
    args = parser.parse_args()

    if not os.path.exists(args.capture_file):
        print(f"Error: {args.capture_file} not found", file=sys.stderr)
        sys.exit(1)

    print(f"Analyzing {args.capture_file}...")
    devices, total_frames = analyze_capture(args.capture_file)

    print(f"Found {total_frames} frames from {len(devices)} devices\n")

    os.makedirs(args.output, exist_ok=True)
    profiles_written = 0

    for addr in sorted(devices.keys()):
        dev = devices[addr]
        if dev["frame_count"] < args.min_frames:
            continue

        mfr_code = dev["manufacturer_code"] or "?"
        mfr_name = MFR_NAMES.get(mfr_code, f"Unknown") if isinstance(mfr_code, int) else "?"
        pgn_list = sorted(dev["pgns"].keys())
        pgn_strs = [PGN_NAMES.get(p, str(p)) for p in pgn_list]

        print(f"  Device {addr:3d} | {mfr_name:12s} | {dev['frame_count']:5d} frames | PGNs: {', '.join(pgn_strs)}")

        name, profile = build_profile_for_device(addr, dev)
        if profile["sensors"]:  # Only write if we found mappable sensor PGNs
            filename = f"{name.lower()}.json"
            filepath = os.path.join(args.output, filename)
            with open(filepath, "w") as f:
                json.dump(profile, f, indent=2)
            print(f"    → {filepath} ({len(profile['sensors'])} sensor slots)")
            profiles_written += 1
        else:
            print(f"    → skipped (no mappable sensor PGNs)")

    print(f"\nWrote {profiles_written} profile(s) to {args.output}/")


if __name__ == "__main__":
    main()
