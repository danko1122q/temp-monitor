# Temperature Monitor

Real-time hardware temperature monitoring for Linux.

## Build

```bash
make
```

## Run

```bash
./bin/temp           # default 2s refresh
./bin/temp 1         # 1s refresh
./bin/temp -s        # show stats
./bin/temp -F        # show fans
./bin/temp --list    # list sensors
./bin/temp --help    # help
```

## Install

```bash
sudo make install
temp
```

## Options

```
-h, --help        show help
-v, --version     show version
-f, --fahrenheit  use fahrenheit
-s, --stats       show statistics
-l, --list        list sensors
-F, --fans        show fan speeds
-n, --no-fans     hide fans
-c, --compact     compact mode
1-60              refresh rate (seconds)
```

## Sensors

Supports:
- CPU (Intel coretemp, AMD k10temp/zenpower)
- GPU (amdgpu, nvidia, i915)
- NVMe drives
- Chipset/Motherboard
- Fans

## No sensors?

```bash
# Intel
sudo modprobe coretemp

# AMD
sudo modprobe k10temp

# Install lm-sensors
sudo apt install lm-sensors
sudo sensors-detect
```

## Distributions

Works on: Debian, Ubuntu, Fedora, RHEL, Arch, Alpine, openSUSE, Gentoo, NixOS

## License

MIT

---
v0.0.1
