# 🔥 Ultimate Hardware Temperature Monitor v0.0.1

<div align="center">

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-0.0.1-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux-orange.svg)

**A powerful, real-time hardware temperature monitoring tool for Linux systems**

Monitor CPU, GPU, NVMe, Chipset, and more with beautiful visual displays!

</div>

---

## ✨ Features

- 🌡️ **Real-time Monitoring** - Live temperature updates with configurable refresh rates
- 🎨 **Beautiful UI** - Color-coded temperature bars with Unicode box-drawing characters
- 🔍 **Auto-detection** - Automatically detects all available hardware sensors
- 📊 **Statistics** - Min/Max/Average temperature tracking
- ⚠️ **Alerts** - Visual warnings for high and critical temperatures
- 🖥️ **Multi-sensor Support** - CPU, GPU, NVMe, Chipset, Memory, VRM, and more
- 🎯 **Modular Design** - Clean, maintainable code structure
- 🚀 **Lightweight** - Minimal resource usage
- 🎛️ **Flexible Configuration** - Multiple display modes and options

## 📸 Screenshots

```
╔═══════════════════════════════════════════════════════════════════════════════╗
║         🔥 ULTIMATE HARDWARE TEMPERATURE MONITOR v0.0.1                      ║
║  ⏰ 2024-12-04 15:30:45  │  ✓ Real-time Monitoring                          ║
╚═══════════════════════════════════════════════════════════════════════════════╝

┌─ ⚙ CPU SENSORS (8 detected) ──────────────────────────────────────────────────
│ Core 0                             45.0°C [████████████░░░░░░░░] [40.0°C▸85.0°C]
│ Core 1                             47.0°C [█████████████░░░░░░░] [41.0°C▸86.0°C]
│ Core 2                             44.0°C [████████████░░░░░░░░] [40.0°C▸84.0°C]
│ Package id 0                       48.0°C [█████████████░░░░░░░] [42.0°C▸87.0°C]

┌─ 🎮 GPU SENSORS (1 detected) ──────────────────────────────────────────────────
│ edge                               55.0°C [███████████████░░░░░] [50.0°C▸75.0°C]

┌─ 💾 NVME SENSORS (2 detected) ─────────────────────────────────────────────────
│ Composite                          42.0°C [███████████░░░░░░░░░] [38.0°C▸65.0°C]
│ Sensor 1                           40.0°C [██████████░░░░░░░░░░] [36.0°C▸62.0°C]

┌─ 📊 SYSTEM STATISTICS ─────────────────────────────────────────────────────────
│ CPU Statistics:  Average: 46.0°C  │  Peak: 87.0°C  │  Min: 40.0°C
│ GPU Statistics:  Average: 55.0°C  │  Peak: 75.0°C
│ System Status:   Active Sensors: 11
└────────────────────────────────────────────────────────────────────────────────

Temperature Ranges: <40°C 40-50°C 50-60°C 60-70°C 70-80°C 80-90°C >90°C
Controls: Ctrl+C=Exit │ F/C=Toggle Unit │ S=Stats │ Refresh: 2s
```

## 🚀 Quick Start

### Prerequisites

- Linux system (tested on MX Linux, Ubuntu, Debian, Arch)
- GCC compiler
- Make build system
- lm-sensors (recommended)

### Installation

```bash
# Clone the repository
git clone https://github.com/danko1122/temp-monitor.git
cd temp-monitor

# Build the program
make

# Install system-wide (optional)
sudo make install

# Or just run directly
./bin/tempmonitor
```

### First Run Setup

If no sensors are detected, run:

```bash
# Setup lm-sensors (automated)
make setup-sensors

# Or manually:
sudo apt install lm-sensors
sudo sensors-detect --auto
sudo systemctl restart kmod

# Load kernel modules
make load-modules

# Verify sensors
make check-modules
```

## 📖 Usage

### Basic Usage

```bash
# Run with default settings (2 second refresh)
tempmonitor

# Update every 1 second
tempmonitor 1

# Show statistics
tempmonitor -s

# Use Fahrenheit
tempmonitor -f

# Combine options
tempmonitor -s -f 3
```

### Command Line Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help message |
| `-v, --version` | Show version information |
| `-f, --fahrenheit` | Use Fahrenheit instead of Celsius |
| `-s, --stats` | Show detailed statistics |
| `-l, --list` | List all detected sensors |
| `-c, --compact` | Use compact display mode |
| `[1-60]` | Refresh rate in seconds |

### Keyboard Controls

While monitoring:
- `Ctrl+C` - Exit the program
- `F` or `C` - Toggle between Fahrenheit/Celsius (coming soon)
- `S` - Toggle statistics display (coming soon)

## 🏗️ Project Structure

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
└── README.md       # This file
```

### Module Overview

- **main.c** - Program initialization, argument parsing, main loop
- **sensor.c** - Hardware sensor detection, temperature reading, statistics
- **display.c** - Terminal UI, color output, temperature visualization
- **utils.c** - File I/O, string manipulation, system utilities

## 🔧 Build System

### Available Make Targets

```bash
make                # Build the program
make debug          # Build with debug symbols
make install        # Install to /usr/local/bin
make uninstall      # Remove from system
make clean          # Clean build files
make run            # Build and run
make run-stats      # Run with statistics
make check-modules  # Check loaded modules
make load-modules   # Load sensor modules
make setup-sensors  # Install lm-sensors
make list-sensors   # List detected sensors
make help           # Show all targets
```

## 🌡️ Supported Sensors

### CPU Sensors
- **Intel**: coretemp (Core series, Xeon)
- **AMD**: k10temp (K10, K11, K12), zenpower (Ryzen)

### GPU Sensors
- **AMD**: amdgpu (modern), radeon (legacy)
- **NVIDIA**: nvidia (proprietary driver)
- **Intel**: i915 (integrated graphics)

### Storage Sensors
- **NVMe**: All NVMe drives with temperature sensors
- **SATA**: drivetemp module (kernel 5.6+)

### Motherboard Sensors
- **Chipset**: ACPI thermal zones, PCH sensors
- **Super I/O**: NCT6775, IT87xx series
- **VRM**: Voltage regulator temperatures

## 🎨 Temperature Ranges

| Range | Color | Description |
|-------|-------|-------------|
| < 40°C | 🔵 Cyan | Cold/Idle |
| 40-50°C | 🟢 Green | Safe |
| 50-60°C | 🟢 Bright Green | Normal |
| 60-70°C | 🟡 Yellow | Warm |
| 70-80°C | 🟡 Bright Yellow | High |
| 80-90°C | 🔴 Bright Red | Very High |
| > 90°C | 🔥 Red Bold | Critical! |

## 🐛 Troubleshooting

### No Sensors Detected

```bash
# 1. Load kernel modules
sudo modprobe coretemp k10temp

# 2. Check if sensors exist
ls -la /sys/class/hwmon/
sensors  # If lm-sensors is installed

# 3. Verify permissions
ls -l /sys/class/hwmon/hwmon*/temp*_input

# 4. Install lm-sensors
sudo apt install lm-sensors
sudo sensors-detect
```

### AMD Ryzen Users

For better Ryzen support, install zenpower:

```bash
# Install DKMS and build tools
sudo apt install dkms git

# Clone and install zenpower
git clone https://github.com/ocerman/zenpower3.git
cd zenpower3
make
sudo make install
sudo modprobe zenpower

# Blacklist k10temp (optional)
echo "blacklist k10temp" | sudo tee /etc/modprobe.d/blacklist-k10temp.conf
```

### Permission Issues

If you get permission errors:

```bash
# Add your user to the appropriate group
sudo usermod -aG systemd-journal $USER

# Or run with sudo (not recommended for monitoring)
sudo tempmonitor
```

## 🔮 Planned Features

- [ ] Interactive keyboard controls
- [ ] Temperature history graphs
- [ ] Fan speed monitoring and control
- [ ] Logging to file
- [ ] JSON/CSV export
- [ ] Web interface
- [ ] System tray integration
- [ ] Email/notification alerts
- [ ] Custom sensor thresholds
- [ ] Multiple color themes

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- Linux kernel developers for the hwmon subsystem
- lm-sensors project for sensor detection utilities
- All contributors and users of this project

## 📧 Contact

- GitHub: [@davaniko](https://github.com/danko1122)
- Email: davaniko1122@gmail.com

## ⭐ Star History

If you find this project useful, please consider giving it a star!

---

<div align="center">

**Made with ❤️ for the Linux community**

</div>
