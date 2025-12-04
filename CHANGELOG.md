# Changelog

All notable changes to Ultimate Hardware Temperature Monitor will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [0.0.1] - 2024-12-04

### 🎉 Initial Release

First public release of Ultimate Hardware Temperature Monitor!

### ✨ Added

#### Core Features
- Real-time temperature monitoring with configurable refresh rates (1-60 seconds)
- Automatic detection of hardware sensors via Linux hwmon subsystem
- Support for multiple sensor types:
  - CPU (Intel coretemp, AMD k10temp/zenpower)
  - GPU (AMD amdgpu, NVIDIA nvidia, Intel i915)
  - NVMe drives
  - Chipset sensors
  - Memory sensors
  - VRM sensors
  - Disk sensors

#### User Interface
- Beautiful terminal UI with Unicode box-drawing characters
- Color-coded temperature bars with gradient visualization
- 7-level temperature range indicators (<40°C to >90°C)
- Grouped sensor display by type
- Real-time temperature updates
- Min/Max/Average temperature tracking
- System statistics overview

#### Display Options
- Celsius and Fahrenheit support
- Statistics mode (--stats)
- Compact display mode (--compact)
- Sensor list mode (--list)
- Customizable refresh rates

#### Build System
- Comprehensive Makefile with multiple targets
- Automated build script (build.sh) with color output
- Debug build support
- System-wide installation support
- Module loading helpers
- Sensor detection and verification tools

#### Architecture
- Modular design with separated components:
  - sensor.c/h - Sensor management
  - display.c/h - UI and visualization
  - utils.c/h - Utility functions
  - main.c - Program entry point
- Clean separation of concerns
- Extensible architecture for future features

#### Documentation
- Comprehensive README.md with screenshots
- Detailed INSTALL.md with platform-specific instructions
- Inline code documentation
- Build system help (make help)
- Command-line help (--help)

#### Compatibility
- Tested on multiple Linux distributions:
  - MX Linux
  - Ubuntu/Debian
  - Arch Linux
  - Fedora
  - Linux Mint
  - Pop!_OS
- Support for both Intel and AMD processors
- Support for modern GPUs (AMD, NVIDIA, Intel)
- NVMe drive temperature monitoring

### 🛠️ Technical Details

#### Sensor Detection
- Scans `/sys/class/hwmon` for available sensors
- Intelligent sensor type detection based on name and label
- Reads temperature from `temp*_input` files
- Supports critical temperature thresholds
- Error handling for inaccessible sensors

#### Temperature Visualization
- 7-level color gradient system
- Unicode block characters for progress bars
- Dynamic terminal size detection
- Responsive layout
- ANSI color codes for cross-terminal compatibility

#### Performance
- Lightweight with minimal CPU usage
- Efficient sensor polling
- No memory leaks
- Fast startup time

### 📝 Known Limitations

- Interactive keyboard controls not yet implemented
- No temperature history graphs
- No fan control
- No configuration file support
- No logging functionality
- Statistics cannot be toggled during runtime

### 🔮 Planned for Next Release (0.1.0)

- [ ] Interactive keyboard controls (F/C toggle, stats toggle)
- [ ] Temperature history graphs with sparklines
- [ ] Fan speed monitoring
- [ ] Configuration file support
- [ ] Logging to file
- [ ] Alert thresholds
- [ ] Multiple color themes

---

## [Unreleased]

### In Progress
- Interactive keyboard input handling
- Temperature history tracking
- Sparkline graph implementation
- Fan speed display

### Under Consideration
- Web interface
- JSON/CSV export
- Email notifications
- System tray integration
- Custom sensor labels
- Plugin system

---

## Version History

- **0.0.1** (2024-12-04) - Initial release

---

## Release Notes

### v0.0.1 - First Public Release

This is the first official release of Ultimate Hardware Temperature Monitor. The focus has been on creating a solid foundation with:

1. **Robust sensor detection** - Works with a wide variety of hardware
2. **Beautiful UI** - Modern terminal interface with colors and Unicode
3. **Clean architecture** - Modular, maintainable code
4. **Comprehensive documentation** - Easy to install and use

While this version includes core monitoring functionality, many advanced features are planned for future releases. We welcome feedback and contributions!

### Installation

```bash
git clone https://github.com/danko1122/temp-monitor.git
cd temp-monitor
make
sudo make install
```

### Quick Start

```bash
# Basic usage
tempmonitor

# With statistics
tempmonitor -s

# List sensors
tempmonitor --list
```

### Bug Reports

Please report any issues on GitHub: https://github.com/danko1122/temp-monitor/issues

---

## Contributing

We welcome contributions! Please see our contributing guidelines for more information.

---

<div align="center">

**Thank you for using Ultimate Hardware Temperature Monitor!**

⭐ Star us on GitHub if you find this useful!

</div>
