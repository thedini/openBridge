"""
Serial handler for OpenBridge GUI ↔ NEMO communication.

Manages a background thread that reads JSON-line events from the NEMO device
over USB serial. The main tkinter thread polls a queue for events.
"""

import json
import threading
import queue
import time
import serial
import serial.tools.list_ports


class SerialHandler:
    """Thread-safe serial handler for NEMO JSON-line protocol."""

    BAUD_RATE = 115200
    RECONNECT_DELAY = 2.0  # seconds between reconnect attempts

    def __init__(self):
        self.port_name = None
        self.serial_port = None
        self.event_queue = queue.Queue()
        self._read_thread = None
        self._running = False
        self._connected = False
        self._lock = threading.Lock()

    @staticmethod
    def list_ports():
        """Return list of available serial port names, prioritizing Teensy devices."""
        ports = []
        for p in serial.tools.list_ports.comports():
            ports.append(p.device)
        # Sort: Teensy/ACM ports first
        ports.sort(key=lambda x: (0 if "ACM" in x or "usbmodem" in x else 1, x))
        return ports

    @property
    def connected(self):
        return self._connected

    def connect(self, port_name):
        """Open serial port and start read thread."""
        self.disconnect()
        try:
            self.serial_port = serial.Serial(
                port=port_name,
                baudrate=self.BAUD_RATE,
                timeout=0.1,
            )
            self.port_name = port_name
            self._connected = True
            self._running = True
            self._read_thread = threading.Thread(
                target=self._read_loop, daemon=True, name="serial-reader"
            )
            self._read_thread.start()
            return True
        except serial.SerialException as e:
            self._connected = False
            self.event_queue.put({"evt": "error", "msg": f"Connect failed: {e}"})
            return False

    def disconnect(self):
        """Stop read thread and close serial port."""
        self._running = False
        if self._read_thread and self._read_thread.is_alive():
            self._read_thread.join(timeout=2.0)
        self._read_thread = None
        with self._lock:
            if self.serial_port and self.serial_port.is_open:
                try:
                    self.serial_port.close()
                except Exception:
                    pass
            self.serial_port = None
        self._connected = False

    def send_command(self, cmd_dict):
        """Serialize dict to JSON line and send to NEMO."""
        with self._lock:
            if not self.serial_port or not self.serial_port.is_open:
                return False
            try:
                line = json.dumps(cmd_dict, separators=(",", ":")) + "\n"
                self.serial_port.write(line.encode("utf-8"))
                self.serial_port.flush()
                return True
            except serial.SerialException:
                self._connected = False
                self.event_queue.put({"evt": "disconnected"})
                return False

    def poll_events(self, max_events=50):
        """Non-blocking: return up to max_events from the queue."""
        events = []
        for _ in range(max_events):
            try:
                events.append(self.event_queue.get_nowait())
            except queue.Empty:
                break
        return events

    def _read_loop(self):
        """Background thread: read serial lines, parse JSON, enqueue events."""
        buf = ""
        while self._running:
            try:
                with self._lock:
                    if not self.serial_port or not self.serial_port.is_open:
                        break
                    raw = self.serial_port.read(self.serial_port.in_waiting or 1)

                if not raw:
                    continue

                buf += raw.decode("utf-8", errors="replace")

                # Process complete lines
                while "\n" in buf:
                    line, buf = buf.split("\n", 1)
                    line = line.strip()
                    if not line:
                        continue

                    # Try to parse as JSON event
                    if line.startswith("{"):
                        try:
                            event = json.loads(line)
                            self.event_queue.put(event)
                        except json.JSONDecodeError:
                            # Not valid JSON — treat as candump line
                            self.event_queue.put({"evt": "raw_line", "line": line})
                    else:
                        # Candump-style or debug output
                        self.event_queue.put({"evt": "raw_line", "line": line})

            except serial.SerialException:
                self._connected = False
                self.event_queue.put({"evt": "disconnected"})
                break
            except Exception as e:
                self.event_queue.put({"evt": "error", "msg": str(e)})
                time.sleep(0.1)

        self._connected = False
