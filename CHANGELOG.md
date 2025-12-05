# Changelog

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

### Planned
- Temperature graphs
- Interactive keyboard controls
- Config file support
- Logging to file
- Custom alert thresholds

---
v0.0.1
