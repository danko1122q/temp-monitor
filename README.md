# Temp Monitor

Real-time hardware temperature monitoring for Linux.

Simple, fast, and professional temperature monitoring tool.

## Features

- Real-time temperature display with color-coded bars
- Supports CPU, GPU, NVMe, chipset, memory, VRM sensors
- Fan speed monitoring
- Statistics tracking (min/max/average)
- Clean terminal display (no artifacts)
- No external dependencies

## Quick Start

```bash
# Build
make

# Run with statistics (1s refresh)
./bin/temp -s 1

# Install system-wide
sudo make install
temp -s 1
```

## Installation

### From Source (Recommended)

```bash
git clone https://github.com/danko1122/temp-monitor.git
cd temp-monitor
make
sudo make install
```

### From GitHub Release

```bash
# Download pre-built binary
wget https://github.com/danko1122/temp-monitor/releases/latest/download/temp-0.0.2-linux-x86_64
chmod +x temp-0.0.2-linux-x86_64
sudo mv temp-0.0.2-linux-x86_64 /usr/local/bin/temp
```

### Package Managers

See [packaging/README.md](packaging/README.md) for:
- Debian/Ubuntu (.deb)
- Fedora/RHEL (.rpm)
- Arch Linux (AUR)
- Alpine Linux (.apk)
- Gentoo (ebuild)
- openSUSE (.rpm)

## Usage

```bash
./bin/temp           # default 2s refresh
./bin/temp 1         # 1s refresh
./bin/temp -s        # show stats
./bin/temp -s 1      # stats + 1s refresh
./bin/temp -F        # show fans
./bin/temp --list    # list sensors
./bin/temp --help    # help
```

## Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help |
| `-v, --version` | Show version |
| `-f, --fahrenheit` | Use Fahrenheit |
| `-s, --stats` | Show statistics |
| `-l, --list` | List sensors |
| `-F, --fans` | Show fan speeds |
| `-n, --no-fans` | Hide fans |
| `-c, --compact` | Compact mode |
| `1-60` | Refresh rate (seconds) |

## Supported Sensors

| Type | Drivers |
|------|---------|
| CPU | Intel coretemp, AMD k10temp/zenpower |
| GPU | amdgpu, nvidia, i915, nouveau |
| NVMe | nvme |
| Chipset | pch_* , nct*, it87 |
| Memory | ddr* |
| VRM | Various motherboard drivers |

## Troubleshooting

### No sensors detected?

```bash
# Load kernel modules
sudo modprobe coretemp   # Intel CPU
sudo modprobe k10temp    # AMD CPU

# Install lm-sensors
sudo apt install lm-sensors   # Debian/Ubuntu
sudo dnf install lm_sensors   # Fedora
sudo pacman -S lm_sensors     # Arch

# Detect sensors
sudo sensors-detect

# Check available sensors
ls /sys/class/hwmon/
```

## Project Structure

```
temp-monitor/
├── main.c, main.h      # Entry point
├── display.c, .h       # Terminal UI
├── sensor.c, .h        # Sensor detection
├── utils.c, .h         # Utilities
├── Makefile            # Build system
├── packaging/          # Linux packages
├── .github/workflows/  # CI/CD
├── CHANGELOG.md        # Version history
├── RELEASE.md          # Release guide
└── LICENSE             # MIT License
```

## Documentation

- [USAGE.md](USAGE.md) - Detailed usage guide
- [CHANGELOG.md](CHANGELOG.md) - Version history
- [RELEASE.md](RELEASE.md) - Release process
- [packaging/README.md](packaging/README.md) - Packaging guide

## Contributing

1. Fork the repository
2. Create feature branch
3. Make changes
4. Test: `make && ./bin/temp -s 1`
5. Submit pull request

## License

MIT License - Copyright (c) 2024 Danko

See [LICENSE](LICENSE) for details.

---

**v0.0.2** | [Releases](https://github.com/danko1122/temp-monitor/releases) | [Issues](https://github.com/danko1122/temp-monitor/issues)
