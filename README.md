# Interface Tester

Qt 6 GUI application for Raspberry Pi 4 (Yocto Embedded Linux) that runs and records interface tests (LED, LAN, Temperature, Display).

## Features
- Primary IP display with live updates and "Not Available" fallback
- Four tests: LED, LAN, Temperature, Display
- Results list with timestamped Pass/Fail entries
- Detailed log file per test with rotation and retention
- Modular test architecture for easy extension

## Requirements
- Qt 6 (Quick, QuickControls2, Network)
- C++ toolchain compatible with Yocto
- Access to `/sys/class/gpio` for LED test
- `ping` available on target for LAN test

## Project layout
- `app/` QML UI and application entrypoint
- `core/` shared backend (test manager, logger, network info)
- `tests/` test case implementations
- `docs/` design notes (logging, network selection)

## Build (Qt Creator)
1. Open `CMakeLists.txt`
2. Configure your kit (desktop or Yocto toolchain)
3. Build and run

## Build (CLI)
```
cmake -S . -B build
cmake --build build
./build/appInterfaceTester
```

## Tests behavior
### LED test
- Uses GPIO 538 (active high)
- Exports GPIO, sets direction to `out`, drives high
- Prompts user: "Did the LED turn on?"
- On response: drives low and unexports GPIO

### LAN test
- Non-interactive ping to `google.com`
- Pass if ping exits with code 0
- Logs interface state, IPv4s, and ping output

### Temperature test
- Reads from `/sys/class/thermal/thermal_zone0/temp`
- Converts to Celsius and compares to 85.0 C max (industrial)
- Logs raw value for `thermal_zone0`

### Display test
- Shows full-screen Red, then Green, then Blue
- Each color displays for 3 seconds before a Yes/No prompt
- Any "No" fails the test; all "Yes" passes

## Results format
```
[Timestamp] - [Test Name]: Pass|Fail
```

## Logging
See `docs/Logging.md` for full details.
- Primary: `/var/log/interface-tester/`
- Fallback: `QStandardPaths::AppDataLocation/logs`
- Daily rotation, 7-day retention

## Network selection
See `docs/Network.md` for selection rules and polling strategy.
