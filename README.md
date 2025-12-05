# Ultimate Hardware Temperature Monitor

A powerful, real-time hardware temperature monitoring tool for Linux systems.

## Overview

Ultimate Hardware Temperature Monitor is a lightweight, terminal-based application that provides real-time monitoring of hardware temperatures across your system. It automatically detects and displays temperatures from CPUs, GPUs, NVMe drives, chipsets, and more.

## Features

- **Real-time Monitoring** - Live temperature updates with configurable refresh rates (1-60 seconds)
- **Auto-detection** - Automatically detects all available hardware sensors via hwmon and thermal subsystems
- **Multi-sensor Support** - CPU, GPU, NVMe, Chipset, Memory, VRM, and more
- **Fan Speed Monitoring** - Detects and displays fan RPM and percentage
- **Color-coded Display** - Visual temperature indicators based on thermal ranges
- **Statistics Tracking** - Min/Max/Average temperature tracking per sensor
- **Temperature Alerts** - Visual warnings for high and critical temperatures
- **Cross-distribution Support** - Works on Debian, Ubuntu, Fedora, RHEL, Arch, Alpine, and more
- **Lightweight** - Minimal resource usage, pure C implementation
- **No Dependencies** - Only requires standard C library and Linux kernel hwmon

## Supported Linux Distributions

- Debian, Ubuntu, Linux Mint
- Fedora, RHEL, CentOS, Rocky Linux, AlmaLinux
- Arch Linux, Manjaro, EndeavourOS
- openSUSE Leap, openSUSE Tumbleweed
- Alpine Linux
- Gentoo
- NixOS
- Any distribution with Linux kernel 2.6+ and hwmon support

## Supported Hardware

### CPU Sensors
- **Intel**: coretemp driver (Core series, Xeon)
- **AMD**: k10temp driver (K10, K11, K12), zenpower (Ryzen)

### GPU Sensors
- **AMD**: amdgpu (modern), radeon (legacy)
- **NVIDIA**: nvidia driver (proprietary)
- **Intel**: i915 (integrated graphics)

### Storage Sensors
- **NVMe**: All NVMe drives with temperature sensors
- **SATA**: drivetemp module (kernel 5.6+)

### Motherboard Sensors
- **Chipset**: ACPI thermal zones, PCH sensors
- **Super I/O**: NCT6775, IT87xx series
- **VRM**: Voltage regulator temperatures

### Fan Sensors
- All hwmon-compatible fan sensors
- RPM and percentage display

## Quick Start

```bash
# Clone the repository
git clone https://github.com/danko1122/temp-monitor.git
cd temp-monitor

# Build the program
make

# Run the monitor
./bin/tempmonitor
```

## Installation

See [USAGE.md](USAGE.md) for detailed installation and usage instructions.

### One-line Build

```bash
git clone https://github.com/danko1122/temp-monitor.git && cd temp-monitor && make && ./bin/tempmonitor
```

### System-wide Installation

```bash
make
sudo make install
tempmonitor
```

## Basic Usage

```bash
# Default monitoring (2 second refresh)
tempmonitor

# Fast updates (1 second)
tempmonitor 1

# Show statistics
tempmonitor -s

# List all sensors
tempmonitor --list

# Show help
tempmonitor --help
```

## Temperature Ranges

| Range | Status | Description |
|-------|--------|-------------|
| < 40C | Cold | Normal (idle) |
| 40-60C | Normal | Typical usage |
| 60-70C | Warm | Heavy load OK |
| 70-80C | Hot | Check cooling |
| 80-90C | Very Hot | Improve cooling |
| > 90C | Critical | Stop work, check system |

## Project Structure

```
temp-monitor/
├── main.c          # Main program entry point
├── sensor.c        # Sensor detection and management
├── sensor.h        # Sensor interface
├── display.c       # Display and UI functions
├── display.h       # Display interface
├── utils.c         # Utility functions
├── utils.h         # Utility interface
├── Makefile        # Build system
├── build.sh        # Automated build script
├── README.md       # This file
└── USAGE.md        # Usage documentation
```

## Building from Source

### Prerequisites

- GCC compiler (gcc)
- GNU Make (make)
- Linux kernel with hwmon support

### Build Commands

```bash
make                # Build release version
make debug          # Build with debug symbols
make clean          # Clean build files
make install        # Install to /usr/local/bin
make uninstall      # Remove from system
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

- GitHub: [@danko1122](https://github.com/danko1122)
- Email: davaniko1122@gmail.com

## Acknowledgments

- Linux kernel developers for the hwmon subsystem
- lm-sensors project for sensor detection utilities
- All contributors and users of this project

---

**Version:** 0.0.1
