# Usage Guide

Panduan lengkap penggunaan Temp Monitor.

## Build

```bash
# Release build
make

# Debug build (with symbols)
make debug

# Clean build
make clean && make
```

## Run

```bash
# Basic (2s default refresh)
./bin/temp

# With statistics and 1s refresh (recommended)
./bin/temp -s 1

# List sensors only
./bin/temp --list
```

## Command Line Options

| Option | Long Form | Description |
|--------|-----------|-------------|
| `-h` | `--help` | Show help message |
| `-v` | `--version` | Show version |
| `-f` | `--fahrenheit` | Use Fahrenheit instead of Celsius |
| `-s` | `--stats` | Show min/max/avg statistics |
| `-l` | `--list` | List all sensors and exit |
| `-F` | `--fans` | Show fan speeds |
| `-n` | `--no-fans` | Hide fan information |
| `-c` | `--compact` | Compact display mode |
| `1-60` | - | Refresh rate in seconds |

## Examples

```bash
# Basic monitoring
./bin/temp

# Real-time with 1 second refresh
./bin/temp 1

# With statistics
./bin/temp -s

# Statistics + 1s refresh (recommended)
./bin/temp -s 1

# Fahrenheit + stats + 3s refresh
./bin/temp -f -s 3

# Show fans
./bin/temp -F

# Compact mode
./bin/temp -c 1

# List all detected sensors
./bin/temp --list

# Check version
./bin/temp --version
```

## Display Explanation

```
+-- [SSD] NVMe SENSORS (9 detected) ----------------------------------------
| Sensor 1                       45.0C [#########...........] [  40.0C->  48.0C]
                                  │     │                      │
                                  │     │                      └── Min -> Max range
                                  │     └── Temperature bar (color coded)
                                  └── Current temperature
```

### Temperature Colors

| Color | Temperature Range | Status |
|-------|-------------------|--------|
| Green | < 40°C | Cool |
| Light Green | 40-50°C | Normal |
| Yellow | 50-60°C | Warm |
| Orange | 60-70°C | Hot |
| Red | 70-80°C | Very Hot |
| Bright Red | 80-90°C | Warning |
| Magenta | > 90°C | Critical |

## Installation

### System-wide Install

```bash
# Install to /usr/local/bin
sudo make install

# Run from anywhere
temp -s 1

# Uninstall
sudo make uninstall
```

### Using Install Script

```bash
# Install
./install.sh

# Uninstall
./uninstall.sh
```

## Troubleshooting

### No sensors detected

```bash
# 1. Load kernel modules
sudo modprobe coretemp   # Intel CPU
sudo modprobe k10temp    # AMD CPU
sudo modprobe drivetemp  # SATA drives

# 2. Install lm-sensors
# Debian/Ubuntu
sudo apt install lm-sensors

# Fedora
sudo dnf install lm_sensors

# Arch
sudo pacman -S lm_sensors

# openSUSE
sudo zypper install sensors

# 3. Detect and configure sensors
sudo sensors-detect

# 4. Verify
sensors
ls /sys/class/hwmon/
```

### Check available sensors

```bash
# List hwmon devices
ls -la /sys/class/hwmon/

# Check sensor names
for d in /sys/class/hwmon/hwmon*; do
    echo "$d: $(cat $d/name 2>/dev/null)"
done

# Use lm-sensors
sensors
```

### Permission issues

```bash
# Most sensors readable without root
# Some may require:
sudo ./bin/temp -s 1
```

## Keyboard Controls

| Key | Action |
|-----|--------|
| `Ctrl+C` | Exit program |

## Tips

1. **Recommended command:**
   ```bash
   temp -s 1
   ```
   Shows statistics with 1-second refresh.

2. **Run in background:**
   ```bash
   temp -s 1 &
   ```

3. **Quick sensor check:**
   ```bash
   temp --list
   ```

## Technical Details

### Sensor Sources

Temp Monitor reads from:
- `/sys/class/hwmon/` - Primary source (hwmon subsystem)
- `/sys/class/thermal/` - Fallback (thermal zones)

### Supported Sensor Types

| Type | Common Drivers |
|------|----------------|
| CPU | coretemp, k10temp, zenpower |
| GPU | amdgpu, nvidia, i915, nouveau |
| NVMe | nvme |
| Chipset | pch_*, nct*, it87, w83* |
| Memory | ddr* |
| Disk | drivetemp |

### Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | Error (no sensors, etc.) |

---

v0.0.2
