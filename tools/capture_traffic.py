#!/usr/bin/env python3
"""
Capture CAN traffic from the NEMO device via serial.

Connects to NEMO, enables CAN2 streaming, and saves raw frames to a
candump-format log file for later analysis or replay.

Usage:
    python capture_traffic.py --port /dev/ttyACM0 --duration 60 --output capture.log

Dependencies:
    pip install pyserial
"""

import argparse
import json
import sys
import time
import serial


def main():
    parser = argparse.ArgumentParser(description="Capture NMEA 2000 CAN traffic via NEMO")
    parser.add_argument("--port", "-p", required=True, help="Serial port (e.g. /dev/ttyACM0)")
    parser.add_argument("--duration", "-d", type=int, default=60, help="Capture duration in seconds (default: 60)")
    parser.add_argument("--output", "-o", default="capture.log", help="Output file (default: capture.log)")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate (default: 115200)")
    args = parser.parse_args()

    print(f"Connecting to {args.port} at {args.baud} baud...")
    try:
        ser = serial.Serial(args.port, args.baud, timeout=0.5)
    except serial.SerialException as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

    time.sleep(1)  # Wait for Teensy to reset after serial open

    # Verify connection
    print("Verifying connection...")
    ser.write(b'{"cmd":"status"}\n')
    time.sleep(0.5)

    got_status = False
    while ser.in_waiting:
        line = ser.readline().decode("utf-8", errors="replace").strip()
        if line.startswith("{"):
            try:
                evt = json.loads(line)
                if evt.get("evt") == "status":
                    got_status = True
                    sensors = evt.get("sensors", [])
                    print(f"  Connected! {len(sensors)} sensors configured.")
            except json.JSONDecodeError:
                pass

    if not got_status:
        print("Warning: No status response. NEMO may not have Serial_Controller firmware.")

    # Enable streaming
    print("Enabling CAN2 traffic stream...")
    ser.write(b'{"cmd":"stream","enable":true}\n')
    time.sleep(0.2)

    # Drain ack
    while ser.in_waiting:
        ser.readline()

    print(f"Capturing for {args.duration}s → {args.output}")
    print("Press Ctrl+C to stop early.\n")

    frame_count = 0
    start_time = time.time()

    try:
        with open(args.output, "w") as f:
            while time.time() - start_time < args.duration:
                if ser.in_waiting:
                    raw = ser.readline().decode("utf-8", errors="replace").strip()
                    if not raw:
                        continue

                    if raw.startswith("{"):
                        try:
                            evt = json.loads(raw)
                            if evt.get("evt") == "can":
                                # Convert JSON event to candump format
                                can_id = evt.get("id", "00000000")
                                dlc = evt.get("dlc", 0)
                                data = evt.get("data", "")
                                data_spaced = " ".join(data[i:i+2] for i in range(0, len(data), 2))
                                ts = evt.get("ts", 0)
                                candump_line = f"  can2  {can_id}   [{dlc}]  {data_spaced}"
                                f.write(candump_line + "\n")
                                frame_count += 1

                                if frame_count % 100 == 0:
                                    elapsed = time.time() - start_time
                                    rate = frame_count / elapsed if elapsed > 0 else 0
                                    print(f"\r  {frame_count} frames ({rate:.1f}/s)", end="", flush=True)
                        except json.JSONDecodeError:
                            pass
                    else:
                        # Raw candump line from firmware
                        f.write(raw + "\n")
                        frame_count += 1
                else:
                    time.sleep(0.01)

    except KeyboardInterrupt:
        print("\nStopped by user.")

    # Disable streaming
    ser.write(b'{"cmd":"stream","enable":false}\n')
    time.sleep(0.1)
    ser.close()

    elapsed = time.time() - start_time
    print(f"\n\nCapture complete: {frame_count} frames in {elapsed:.1f}s → {args.output}")


if __name__ == "__main__":
    main()
