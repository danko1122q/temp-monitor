# Temp Monitor

Real-time hardware temperature monitoring for Linux.

## Project Overview

**Version:** 0.0.2  
**Language:** C (C11)  
**License:** MIT  
**Author:** Danko

## Quick Start

```bash
# Build
make

# Run (real-time monitoring with 1s refresh)
./bin/temp -s 1

# List sensors
./bin/temp --list

# Show help
./bin/temp --help
```

## Project Structure

```
temp-monitor/
├── main.c              # Entry point, argument parsing
├── main.h              # Main header (legacy, use main.c)
├── display.c           # Terminal UI rendering
├── display.h           # Display function prototypes
├── sensor.c            # Sensor detection and reading
├── sensor.h            # Sensor data structures
├── utils.c             # Utility functions
├── utils.h             # Utility prototypes, color codes
├── Makefile            # Build configuration
├── build.sh            # Multi-distro build script
├── .github/
│   └── workflows/
│       └── release.yml # GitHub Actions CI/CD
├── packaging/          # Linux distribution packages
│   ├── README.md       # Packaging guide
│   ├── debian/         # .deb package files
│   ├── arch/           # PKGBUILD for AUR
│   ├── rpm/            # Fedora/RHEL spec
│   ├── alpine/         # APKBUILD
│   ├── gentoo/         # ebuild file
│   └── opensuse/       # openSUSE spec
├── README.md           # User documentation
├── USAGE.md            # Usage guide
├── CHANGELOG.md        # Version history
├── RELEASE.md          # Release guide (ID)
└── LICENSE             # MIT License
```

## Build Commands

| Command | Description |
|---------|-------------|
| `make` | Build release binary |
| `make debug` | Build with debug symbols |
| `make clean` | Remove build artifacts |
| `make install` | Install to /usr/local/bin |
| `make uninstall` | Remove from system |
| `make run` | Build and run |

## Command Line Options

| Option | Description |
|--------|-------------|
| `-h, --help` | Show help |
| `-v, --version` | Show version |
| `-f, --fahrenheit` | Use Fahrenheit |
| `-s, --stats` | Show statistics |
| `-l, --list` | List sensors |
| `-F, --fans` | Show fans |
| `-n, --no-fans` | Hide fans |
| `-c, --compact` | Compact mode |
| `1-60` | Refresh rate (seconds) |

## Key Features

- Real-time temperature monitoring
- Supports CPU, GPU, NVMe, chipset, memory, VRM, disk sensors
- Color-coded temperature display
- Alternate screen buffer (no terminal artifacts)
- Min/Max/Average statistics
- Fan speed monitoring
- Auto-detects sensors from Linux hwmon

## Technical Notes

### Sensor Detection
- Reads from `/sys/class/hwmon/` (primary)
- Falls back to `/sys/class/thermal/` 
- Supports coretemp, k10temp, amdgpu, nvme, etc.

### Display
- Uses ANSI escape codes for colors
- Alternate screen buffer prevents terminal pollution
- Updates in-place without scrolling

### Dependencies
- Standard C library (glibc/musl)
- GCC or Clang compiler
- No external libraries required

## Workflow Configuration

The project uses a console-based workflow for real-time monitoring:
- **Command:** `./bin/temp -s 1`
- **Output:** Console (TUI)
- **Refresh:** 1 second

## Recent Changes (v0.0.2)

- Fixed display leaving ghost/trail output in terminal
- Implemented alternate screen buffer
- Added comprehensive code documentation
- Simplified application name to "Temp Monitor"
- Updated all packaging files
- Added GitHub Actions CI/CD for automated releases
- Added RELEASE.md guide for releases
- Updated packaging to use GitHub release URLs

## Release Workflow

1. Update version in main.c, main.h, build.sh
2. Update CHANGELOG.md
3. Update packaging files
4. Commit and push
5. Create git tag: `git tag -a v0.0.3 -m "Version 0.0.3"`
6. Push tag: `git push origin v0.0.3`
7. GitHub Actions automatically builds and releases

See RELEASE.md for detailed guide.

## User Preferences

- Professional, clean code style
- Indonesian language for communication
- Comprehensive documentation with comments
- Support for multiple Linux distributions
