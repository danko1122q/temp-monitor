# Usage

## Build

```bash
make
```

## Run

```bash
./bin/temp
```

## Options

| Option | Description |
|--------|-------------|
| -h | help |
| -v | version |
| -f | fahrenheit |
| -s | stats |
| -l | list sensors |
| -F | show fans |
| -n | hide fans |
| -c | compact |
| 1-60 | refresh rate |

## Examples

```bash
./bin/temp           # basic
./bin/temp 1         # 1s refresh
./bin/temp -s        # with stats
./bin/temp -s -f 3   # stats + fahrenheit + 3s
./bin/temp --list    # list only
```

## Install

```bash
sudo make install    # install to /usr/local/bin
temp                 # run from anywhere
sudo make uninstall  # remove
```

## Troubleshooting

### No sensors

```bash
# load modules
sudo modprobe coretemp   # intel
sudo modprobe k10temp    # amd

# install lm-sensors
sudo apt install lm-sensors   # debian
sudo dnf install lm_sensors   # fedora
sudo pacman -S lm_sensors     # arch

# detect
sudo sensors-detect
```

### Check sensors

```bash
ls /sys/class/hwmon/
sensors
```

---
v0.0.1
