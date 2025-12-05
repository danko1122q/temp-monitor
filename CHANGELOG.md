# Changelog

All notable changes to Temp Monitor will be documented in this file.

Format based on [Keep a Changelog](https://keepachangelog.com/).

---

## [0.0.2] - 2024-12-05

### Fixed
- Fixed display leaving trail/ghost output in terminal scroll buffer
- Implemented alternate screen buffer for clean display (like vim, htop)
- Screen now switches to alternate buffer on start and restores on exit
- No more visual artifacts or traces left in terminal history

### Changed
- Renamed from "Ultimate Hardware Temperature Monitor" to "Temp Monitor"
- Updated clear_screen() to also clear scroll buffer with \033[3J
- Added enter_alternate_screen() and exit_alternate_screen() functions
- Simplified and professionalized naming throughout codebase

### Added
- Comprehensive code documentation with Doxygen-style comments
- Copyright headers (MIT License) to all source files:
  - main.c, main.h
  - display.c, display.h
  - sensor.c, sensor.h
  - utils.c, utils.h
- GitHub Actions CI/CD workflow for automated releases
- RELEASE.md - Complete release guide (Indonesian)
- Pre-built binary releases for x86_64 and aarch64

### Packaging
- Updated all packaging files to version 0.0.2
- Fixed packaging scripts to use GitHub release URLs
- Added proper source URL references for all distributions:
  - Debian/Ubuntu: packaging/debian/*
  - Arch Linux: packaging/arch/PKGBUILD
  - Fedora/RHEL: packaging/rpm/temp.spec
  - Alpine: packaging/alpine/APKBUILD
  - Gentoo: packaging/gentoo/temp-0.0.2.ebuild
  - openSUSE: packaging/opensuse/temp.spec
- Added comprehensive packaging guide (packaging/README.md)

### Documentation
- Updated README.md with complete project information
- Updated USAGE.md with detailed usage guide
- Added release workflow documentation
- Created replit.md for project overview

---

## [0.0.1] - 2024-12-05

### Added
- Initial release
- Real-time temperature monitoring from hwmon subsystem
- CPU sensor support (Intel coretemp, AMD k10temp, zenpower)
- GPU sensor support (amdgpu, nvidia, i915, nouveau)
- NVMe drive temperature monitoring
- Chipset and motherboard sensor support
- Fan speed detection and display
- Color-coded temperature bars
- Statistics tracking (min/max/average per sensor)
- Temperature alerts for warning and critical levels
- Fahrenheit and Celsius modes
- Refresh rate configuration (1-60 seconds)
- Sensor listing mode
- Cross-distribution build script with support for:
  - Debian, Ubuntu, Mint
  - Fedora, RHEL, CentOS
  - Arch, Manjaro
  - openSUSE
  - Alpine
  - Gentoo, NixOS

### Known Issues
- Graph feature not implemented yet
- Interactive keyboard controls not implemented yet
- No config file support

---

## Planned Features

- [ ] Temperature graphs
- [ ] Interactive keyboard controls
- [ ] Config file support
- [ ] Logging to file
- [ ] Custom alert thresholds
- [ ] System tray integration
- [ ] Web interface

---

v0.0.2
