#!/usr/bin/env python3
"""
OpenBridge GUI — Control surface for the NEMO NMEA 2000 educational platform.

Communicates with NEMO firmware via JSON-line serial protocol.
Provides sensor control, network monitoring, traffic logging, and device profiles.

Usage:
    python openbridge_gui.py

Dependencies:
    pip install pyserial
"""

import json
import os
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from datetime import datetime
from pathlib import Path

from serial_handler import SerialHandler

# Sensor types matching firmware MessageType enum
SENSOR_TYPES = [
    ("engine_rpm", "Engine RPM", "0-6000 RPM"),
    ("water_depth", "Water Depth", "0-100 m"),
    ("heading", "Heading", "0-360 deg"),
    ("speed", "Speed", "0-20 kn"),
    ("rudder", "Rudder", "-45 to +45 deg"),
    ("wind_speed", "Wind Speed", "0-50 m/s"),
    ("wind_angle", "Wind Angle", "0-360 deg"),
    ("water_temp", "Water Temp", "-5 to +40 C"),
    ("outside_temp", "Outside Temp", "-20 to +50 C"),
    ("pressure", "Pressure", "80-110 kPa"),
    ("humidity", "Humidity", "0-100%"),
    ("battery_volt", "Battery Volt", "0-30 V"),
    ("tank_level", "Tank Level", "0-100%"),
]

MANUFACTURERS = [
    ("Garmin", 229),
    ("Raymarine", 1851),
    ("Simrad", 1857),
    ("Navico", 275),
    ("Lowrance", 140),
    ("Furuno", 1855),
    ("B&G", 381),
    ("Mercury", 144),
    ("Yamaha", 1862),
    ("Volvo Penta", 174),
    ("Maretron", 137),
]

PROFILES_DIR = Path(__file__).parent / "device_profiles"


class OpenBridgeGUI:
    """Main application window."""

    POLL_INTERVAL_MS = 50  # How often to check serial queue

    def __init__(self, root):
        self.root = root
        self.root.title("OpenBridge — NEMO Control")
        self.root.geometry("900x650")
        self.root.minsize(800, 550)

        self.serial = SerialHandler()
        self.traffic_paused = False
        self.traffic_lines = []
        self.max_traffic_lines = 5000

        self._build_connection_bar()
        self._build_notebook()
        self._start_polling()

    # ─── Connection Bar ───────────────────────────────────────────────

    def _build_connection_bar(self):
        bar = ttk.Frame(self.root, padding=5)
        bar.pack(fill=tk.X)

        ttk.Label(bar, text="Port:").pack(side=tk.LEFT)

        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(
            bar, textvariable=self.port_var, width=20, state="readonly"
        )
        self.port_combo.pack(side=tk.LEFT, padx=(2, 5))

        ttk.Button(bar, text="Refresh", command=self._refresh_ports).pack(
            side=tk.LEFT, padx=2
        )

        self.connect_btn = ttk.Button(bar, text="Connect", command=self._toggle_connect)
        self.connect_btn.pack(side=tk.LEFT, padx=5)

        self.status_label = ttk.Label(bar, text="Disconnected", foreground="red")
        self.status_label.pack(side=tk.LEFT, padx=10)

        ttk.Label(bar, text="115200 baud").pack(side=tk.RIGHT)

        self._refresh_ports()

    def _refresh_ports(self):
        ports = SerialHandler.list_ports()
        self.port_combo["values"] = ports
        if ports and not self.port_var.get():
            self.port_var.set(ports[0])

    def _toggle_connect(self):
        if self.serial.connected:
            self.serial.disconnect()
            self._update_connection_ui(False)
        else:
            port = self.port_var.get()
            if not port:
                messagebox.showwarning("No Port", "Select a serial port first.")
                return
            if self.serial.connect(port):
                self._update_connection_ui(True)
                # Request initial status
                self.serial.send_command({"cmd": "status"})
                # Enable streaming
                self.serial.send_command({"cmd": "stream", "enable": True})
            else:
                self._update_connection_ui(False)

    def _update_connection_ui(self, connected):
        if connected:
            self.connect_btn.config(text="Disconnect")
            self.status_label.config(text="Connected", foreground="green")
        else:
            self.connect_btn.config(text="Connect")
            self.status_label.config(text="Disconnected", foreground="red")

    # ─── Notebook (Tabs) ──────────────────────────────────────────────

    def _build_notebook(self):
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)

        self._build_sensors_tab()
        self._build_monitor_tab()
        self._build_traffic_tab()
        self._build_profiles_tab()

    # ─── Sensors Tab ──────────────────────────────────────────────────

    def _build_sensors_tab(self):
        frame = ttk.Frame(self.notebook, padding=10)
        self.notebook.add(frame, text="Sensors")

        self.sensor_panels = []
        for i in range(3):
            panel = self._build_sensor_panel(frame, i)
            panel["frame"].grid(row=0, column=i, padx=5, pady=5, sticky="nsew")
            self.sensor_panels.append(panel)
            frame.columnconfigure(i, weight=1)
        frame.rowconfigure(0, weight=1)

    def _build_sensor_panel(self, parent, sensor_id):
        lf = ttk.LabelFrame(parent, text=f"Sensor {sensor_id + 1}", padding=10)

        # Sensor type
        ttk.Label(lf, text="Type:").grid(row=0, column=0, sticky="w", pady=2)
        type_var = tk.StringVar(value=SENSOR_TYPES[0][1])
        type_combo = ttk.Combobox(
            lf,
            textvariable=type_var,
            values=[t[1] for t in SENSOR_TYPES],
            state="readonly",
            width=16,
        )
        type_combo.grid(row=0, column=1, sticky="ew", pady=2)
        type_combo.bind("<<ComboboxSelected>>", lambda e, sid=sensor_id: self._on_sensor_change(sid))

        # Range label
        range_label = ttk.Label(lf, text=SENSOR_TYPES[0][2], foreground="gray")
        range_label.grid(row=1, column=0, columnspan=2, sticky="w")

        # Value slider
        ttk.Label(lf, text="Value:").grid(row=2, column=0, sticky="w", pady=2)
        value_var = tk.DoubleVar(value=0.5)
        slider = ttk.Scale(
            lf, from_=0.0, to=1.0, variable=value_var, orient=tk.HORIZONTAL,
            command=lambda v, sid=sensor_id: self._on_slider_change(sid),
        )
        slider.grid(row=2, column=1, sticky="ew", pady=2)

        value_label = ttk.Label(lf, text="0.500")
        value_label.grid(row=3, column=1, sticky="w")

        # Manufacturer
        ttk.Label(lf, text="Mfr:").grid(row=4, column=0, sticky="w", pady=2)
        mfr_var = tk.StringVar(value=MANUFACTURERS[0][0])
        mfr_combo = ttk.Combobox(
            lf,
            textvariable=mfr_var,
            values=[m[0] for m in MANUFACTURERS],
            state="readonly",
            width=16,
        )
        mfr_combo.grid(row=4, column=1, sticky="ew", pady=2)
        mfr_combo.bind("<<ComboboxSelected>>", lambda e, sid=sensor_id: self._on_sensor_change(sid))

        # Active toggle
        active_var = tk.BooleanVar(value=False)
        active_check = ttk.Checkbutton(
            lf, text="Active", variable=active_var,
            command=lambda sid=sensor_id: self._on_sensor_change(sid),
        )
        active_check.grid(row=5, column=0, columnspan=2, sticky="w", pady=5)

        # Status display
        status_label = ttk.Label(lf, text="Addr: -- | Idle", foreground="gray")
        status_label.grid(row=6, column=0, columnspan=2, sticky="w")

        lf.columnconfigure(1, weight=1)

        return {
            "frame": lf,
            "type_var": type_var,
            "type_combo": type_combo,
            "range_label": range_label,
            "value_var": value_var,
            "value_label": value_label,
            "mfr_var": mfr_var,
            "active_var": active_var,
            "status_label": status_label,
        }

    def _on_slider_change(self, sensor_id):
        panel = self.sensor_panels[sensor_id]
        val = panel["value_var"].get()
        panel["value_label"].config(text=f"{val:.3f}")

        # Update range label
        type_name = panel["type_var"].get()
        for key, display, rng in SENSOR_TYPES:
            if display == type_name:
                panel["range_label"].config(text=rng)
                break

        self._send_sensor_command(sensor_id)

    def _on_sensor_change(self, sensor_id):
        panel = self.sensor_panels[sensor_id]
        # Update range label
        type_name = panel["type_var"].get()
        for key, display, rng in SENSOR_TYPES:
            if display == type_name:
                panel["range_label"].config(text=rng)
                break
        self._send_sensor_command(sensor_id)

    def _send_sensor_command(self, sensor_id):
        if not self.serial.connected:
            return
        panel = self.sensor_panels[sensor_id]

        # Find sensor type key
        type_display = panel["type_var"].get()
        type_key = None
        for key, display, _ in SENSOR_TYPES:
            if display == type_display:
                type_key = key
                break

        # Find manufacturer code
        mfr_name = panel["mfr_var"].get()
        mfr_code = 229  # default Garmin
        for name, code in MANUFACTURERS:
            if name == mfr_name:
                mfr_code = code
                break

        cmd = {
            "cmd": "sensor",
            "id": sensor_id,
            "type": type_key,
            "value": round(panel["value_var"].get(), 4),
            "active": panel["active_var"].get(),
            "mfr": mfr_code,
        }
        self.serial.send_command(cmd)

    # ─── Monitor Tab ──────────────────────────────────────────────────

    def _build_monitor_tab(self):
        frame = ttk.Frame(self.notebook, padding=10)
        self.notebook.add(frame, text="Monitor")

        # Controls
        ctrl = ttk.Frame(frame)
        ctrl.pack(fill=tk.X, pady=(0, 5))
        ttk.Button(ctrl, text="Refresh", command=self._request_monitor).pack(side=tk.LEFT)
        self.auto_refresh_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(ctrl, text="Auto-refresh (5s)", variable=self.auto_refresh_var).pack(
            side=tk.LEFT, padx=10
        )

        # Device tree
        columns = ("addr", "name", "pgns", "last_seen")
        self.device_tree = ttk.Treeview(frame, columns=columns, show="headings", height=15)
        self.device_tree.heading("addr", text="Addr")
        self.device_tree.heading("name", text="Device Name")
        self.device_tree.heading("pgns", text="PGNs")
        self.device_tree.heading("last_seen", text="Last Seen")
        self.device_tree.column("addr", width=60, anchor="center")
        self.device_tree.column("name", width=200)
        self.device_tree.column("pgns", width=350)
        self.device_tree.column("last_seen", width=120, anchor="center")

        scrollbar = ttk.Scrollbar(frame, orient=tk.VERTICAL, command=self.device_tree.yview)
        self.device_tree.configure(yscrollcommand=scrollbar.set)
        self.device_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        # Auto-refresh timer
        self._monitor_refresh_id = None
        self._schedule_monitor_refresh()

    def _request_monitor(self):
        if self.serial.connected:
            self.serial.send_command({"cmd": "monitor"})

    def _schedule_monitor_refresh(self):
        if self.auto_refresh_var.get() and self.serial.connected:
            self._request_monitor()
        self._monitor_refresh_id = self.root.after(5000, self._schedule_monitor_refresh)

    def _update_device_tree(self, devices):
        """Update device tree from monitor event data."""
        self.device_tree.delete(*self.device_tree.get_children())
        for dev in devices:
            addr = dev.get("addr", "?")
            name = dev.get("name", "Unknown")
            pgns = ", ".join(str(p) for p in dev.get("pgns", []))
            last_seen = dev.get("last_seen", 0)
            # Convert millis to relative time
            self.device_tree.insert("", tk.END, values=(addr, name, pgns, f"{last_seen}ms"))

    # ─── Traffic Tab ──────────────────────────────────────────────────

    def _build_traffic_tab(self):
        frame = ttk.Frame(self.notebook, padding=10)
        self.notebook.add(frame, text="Traffic")

        # Controls
        ctrl = ttk.Frame(frame)
        ctrl.pack(fill=tk.X, pady=(0, 5))

        self.pause_btn = ttk.Button(ctrl, text="Pause", command=self._toggle_pause)
        self.pause_btn.pack(side=tk.LEFT)

        ttk.Button(ctrl, text="Clear", command=self._clear_traffic).pack(
            side=tk.LEFT, padx=5
        )
        ttk.Button(ctrl, text="Export...", command=self._export_traffic).pack(
            side=tk.LEFT, padx=5
        )

        ttk.Label(ctrl, text="Filter:").pack(side=tk.LEFT, padx=(20, 2))
        self.traffic_filter_var = tk.StringVar()
        filter_entry = ttk.Entry(ctrl, textvariable=self.traffic_filter_var, width=20)
        filter_entry.pack(side=tk.LEFT)

        self.traffic_count_label = ttk.Label(ctrl, text="0 frames")
        self.traffic_count_label.pack(side=tk.RIGHT)

        # Traffic text area
        text_frame = ttk.Frame(frame)
        text_frame.pack(fill=tk.BOTH, expand=True)

        self.traffic_text = tk.Text(
            text_frame, wrap=tk.NONE, font=("Courier", 10), state=tk.DISABLED
        )
        y_scroll = ttk.Scrollbar(text_frame, orient=tk.VERTICAL, command=self.traffic_text.yview)
        x_scroll = ttk.Scrollbar(text_frame, orient=tk.HORIZONTAL, command=self.traffic_text.xview)
        self.traffic_text.configure(yscrollcommand=y_scroll.set, xscrollcommand=x_scroll.set)

        self.traffic_text.grid(row=0, column=0, sticky="nsew")
        y_scroll.grid(row=0, column=1, sticky="ns")
        x_scroll.grid(row=1, column=0, sticky="ew")
        text_frame.rowconfigure(0, weight=1)
        text_frame.columnconfigure(0, weight=1)

    def _toggle_pause(self):
        self.traffic_paused = not self.traffic_paused
        self.pause_btn.config(text="Resume" if self.traffic_paused else "Pause")

    def _clear_traffic(self):
        self.traffic_lines.clear()
        self.traffic_text.config(state=tk.NORMAL)
        self.traffic_text.delete("1.0", tk.END)
        self.traffic_text.config(state=tk.DISABLED)
        self.traffic_count_label.config(text="0 frames")

    def _export_traffic(self):
        if not self.traffic_lines:
            messagebox.showinfo("Empty", "No traffic to export.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".log",
            filetypes=[("Log files", "*.log"), ("All files", "*.*")],
            initialfile=f"capture_{datetime.now():%Y%m%d_%H%M%S}.log",
        )
        if path:
            with open(path, "w") as f:
                f.write("\n".join(self.traffic_lines) + "\n")

    def _append_traffic(self, line):
        if self.traffic_paused:
            return

        # Apply filter
        filt = self.traffic_filter_var.get().strip()
        if filt and filt.upper() not in line.upper():
            return

        self.traffic_lines.append(line)
        if len(self.traffic_lines) > self.max_traffic_lines:
            self.traffic_lines = self.traffic_lines[-self.max_traffic_lines:]

        self.traffic_text.config(state=tk.NORMAL)
        self.traffic_text.insert(tk.END, line + "\n")
        self.traffic_text.see(tk.END)
        self.traffic_text.config(state=tk.DISABLED)
        self.traffic_count_label.config(text=f"{len(self.traffic_lines)} frames")

    # ─── Profiles Tab ─────────────────────────────────────────────────

    def _build_profiles_tab(self):
        frame = ttk.Frame(self.notebook, padding=10)
        self.notebook.add(frame, text="Profiles")

        # Profile list
        list_frame = ttk.LabelFrame(frame, text="Device Profiles", padding=5)
        list_frame.pack(fill=tk.BOTH, expand=True, side=tk.LEFT, padx=(0, 5))

        self.profile_listbox = tk.Listbox(list_frame, width=30)
        self.profile_listbox.pack(fill=tk.BOTH, expand=True)
        self.profile_listbox.bind("<<ListboxSelect>>", self._on_profile_select)

        btn_frame = ttk.Frame(list_frame)
        btn_frame.pack(fill=tk.X, pady=(5, 0))
        ttk.Button(btn_frame, text="Load File...", command=self._load_profile_file).pack(
            side=tk.LEFT, padx=2
        )
        ttk.Button(btn_frame, text="Save Current...", command=self._save_profile).pack(
            side=tk.LEFT, padx=2
        )

        # Profile detail
        detail_frame = ttk.LabelFrame(frame, text="Profile Details", padding=10)
        detail_frame.pack(fill=tk.BOTH, expand=True, side=tk.LEFT)

        self.profile_info = tk.Text(
            detail_frame, wrap=tk.WORD, height=12, state=tk.DISABLED, font=("Courier", 10)
        )
        self.profile_info.pack(fill=tk.BOTH, expand=True)

        ttk.Button(detail_frame, text="Apply Profile", command=self._apply_selected_profile).pack(
            pady=(10, 0)
        )

        self._profiles = {}
        self._load_builtin_profiles()

    def _load_builtin_profiles(self):
        """Load all JSON files from the device_profiles directory."""
        if not PROFILES_DIR.exists():
            return
        for f in sorted(PROFILES_DIR.glob("*.json")):
            try:
                with open(f) as fp:
                    profile = json.load(fp)
                name = profile.get("name", f.stem)
                self._profiles[name] = profile
                self.profile_listbox.insert(tk.END, name)
            except (json.JSONDecodeError, OSError):
                pass

    def _on_profile_select(self, event):
        sel = self.profile_listbox.curselection()
        if not sel:
            return
        name = self.profile_listbox.get(sel[0])
        profile = self._profiles.get(name)
        if not profile:
            return

        self.profile_info.config(state=tk.NORMAL)
        self.profile_info.delete("1.0", tk.END)

        text = f"Name: {profile.get('name', '?')}\n"
        text += f"Description: {profile.get('description', '')}\n"
        mfr = profile.get("manufacturer", {})
        text += f"Manufacturer: {mfr.get('name', '?')} (code {mfr.get('code', '?')})\n"
        text += "\nSensors:\n"
        for s in profile.get("sensors", []):
            text += f"  Slot {s.get('slot', '?')}: {s.get('type', '?')} "
            text += f"value={s.get('default_value', 0.5)} "
            text += f"active={s.get('active', False)}\n"

        self.profile_info.insert("1.0", text)
        self.profile_info.config(state=tk.DISABLED)

    def _apply_selected_profile(self):
        sel = self.profile_listbox.curselection()
        if not sel:
            messagebox.showinfo("No Selection", "Select a profile first.")
            return
        name = self.profile_listbox.get(sel[0])
        profile = self._profiles.get(name)
        if not profile:
            return
        self._apply_profile(profile)

    def _apply_profile(self, profile):
        """Apply a device profile — configure all 3 sensors at once."""
        mfr = profile.get("manufacturer", {})
        mfr_code = mfr.get("code", 229)
        mfr_name = mfr.get("name", "Garmin")

        for sensor_cfg in profile.get("sensors", []):
            slot = sensor_cfg.get("slot", 0)
            if slot < 0 or slot > 2:
                continue

            type_key = sensor_cfg.get("type", "heading")
            value = sensor_cfg.get("default_value", 0.5)
            active = sensor_cfg.get("active", True)

            # Update GUI panel
            panel = self.sensor_panels[slot]

            # Find display name for type
            for key, display, _ in SENSOR_TYPES:
                if key == type_key:
                    panel["type_var"].set(display)
                    break

            panel["value_var"].set(value)
            panel["value_label"].config(text=f"{value:.3f}")
            panel["mfr_var"].set(mfr_name)
            panel["active_var"].set(active)

            # Update range label
            for key, display, rng in SENSOR_TYPES:
                if key == type_key:
                    panel["range_label"].config(text=rng)
                    break

            # Send command to firmware
            if self.serial.connected:
                cmd = {
                    "cmd": "sensor",
                    "id": slot,
                    "type": type_key,
                    "value": round(value, 4),
                    "active": active,
                    "mfr": mfr_code,
                }
                self.serial.send_command(cmd)

    def _load_profile_file(self):
        path = filedialog.askopenfilename(
            filetypes=[("JSON files", "*.json"), ("All files", "*.*")],
            initialdir=str(PROFILES_DIR),
        )
        if not path:
            return
        try:
            with open(path) as f:
                profile = json.load(f)
            name = profile.get("name", Path(path).stem)
            self._profiles[name] = profile
            # Check if already in listbox
            items = self.profile_listbox.get(0, tk.END)
            if name not in items:
                self.profile_listbox.insert(tk.END, name)
        except (json.JSONDecodeError, OSError) as e:
            messagebox.showerror("Error", f"Failed to load profile: {e}")

    def _save_profile(self):
        """Save current sensor configuration as a profile."""
        name = tk.simpledialog.askstring("Profile Name", "Enter profile name:") if hasattr(tk, 'simpledialog') else None
        if not name:
            # Fallback: use timestamp
            name = f"Custom_{datetime.now():%Y%m%d_%H%M%S}"

        profile = {
            "name": name,
            "description": "Custom profile saved from GUI",
            "manufacturer": {},
            "sensors": [],
        }

        for i, panel in enumerate(self.sensor_panels):
            type_display = panel["type_var"].get()
            type_key = "heading"
            for key, display, _ in SENSOR_TYPES:
                if display == type_display:
                    type_key = key
                    break

            mfr_name = panel["mfr_var"].get()
            mfr_code = 229
            for mname, code in MANUFACTURERS:
                if mname == mfr_name:
                    mfr_code = code
                    break

            if i == 0:
                profile["manufacturer"] = {"name": mfr_name, "code": mfr_code}

            profile["sensors"].append({
                "slot": i,
                "type": type_key,
                "default_value": round(panel["value_var"].get(), 4),
                "active": panel["active_var"].get(),
            })

        path = filedialog.asksaveasfilename(
            defaultextension=".json",
            filetypes=[("JSON files", "*.json")],
            initialdir=str(PROFILES_DIR),
            initialfile=f"{name.lower().replace(' ', '_')}.json",
        )
        if path:
            with open(path, "w") as f:
                json.dump(profile, f, indent=2)

    # ─── Event Polling ────────────────────────────────────────────────

    def _start_polling(self):
        self._poll_serial()

    def _poll_serial(self):
        events = self.serial.poll_events(max_events=100)
        for event in events:
            self._handle_event(event)

        # Check connection state
        if not self.serial.connected and self.connect_btn.cget("text") == "Disconnect":
            self._update_connection_ui(False)

        self.root.after(self.POLL_INTERVAL_MS, self._poll_serial)

    def _handle_event(self, event):
        evt_type = event.get("evt", "")

        if evt_type == "status":
            self._handle_status_event(event)
        elif evt_type == "monitor":
            devices = event.get("devices", [])
            self._update_device_tree(devices)
        elif evt_type == "can":
            # Format as candump-style line
            ts = event.get("ts", 0)
            can_id = event.get("id", "00000000")
            dlc = event.get("dlc", 0)
            data = event.get("data", "")
            # Format data with spaces between bytes
            data_spaced = " ".join(data[i:i+2] for i in range(0, len(data), 2))
            line = f"  can2  {can_id}   [{dlc}]  {data_spaced}"
            self._append_traffic(line)
        elif evt_type == "raw_line":
            line = event.get("line", "")
            if line:
                self._append_traffic(line)
        elif evt_type == "error":
            msg = event.get("msg", "Unknown error")
            self._append_traffic(f"[ERROR] {msg}")
        elif evt_type == "disconnected":
            self._update_connection_ui(False)
        elif evt_type == "ack":
            pass  # Command acknowledged, nothing to show

    def _handle_status_event(self, event):
        """Update sensor panels from status event."""
        sensors = event.get("sensors", [])
        for s in sensors:
            sid = s.get("id", -1)
            if sid < 0 or sid > 2:
                continue
            panel = self.sensor_panels[sid]

            # Update type display
            type_key = s.get("type", "")
            for key, display, rng in SENSOR_TYPES:
                if key == type_key:
                    panel["type_var"].set(display)
                    panel["range_label"].config(text=rng)
                    break

            # Update value (only if not serial controlled — avoid fighting with slider)
            if not s.get("serial_ctrl", False):
                val = s.get("value", 0)
                panel["value_var"].set(val)
                panel["value_label"].config(text=f"{val:.3f}")

            # Update manufacturer
            mfr_code = s.get("mfr", 229)
            for name, code in MANUFACTURERS:
                if code == mfr_code:
                    panel["mfr_var"].set(name)
                    break

            # Update active
            panel["active_var"].set(s.get("active", False))

            # Update status
            addr = s.get("addr", 254)
            active = s.get("active", False)
            status_text = f"Addr: {addr}" if active else "Addr: -- | Inactive"
            panel["status_label"].config(
                text=status_text,
                foreground="green" if active else "gray",
            )


def main():
    root = tk.Tk()
    app = OpenBridgeGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()
