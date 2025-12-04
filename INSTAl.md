# 📦 Installation Guide

Complete installation guide for Ultimate Hardware Temperature Monitor v0.0.1

---

## 📋 Table of Contents

1. [System Requirements](#system-requirements)
2. [Quick Installation](#quick-installation)
3. [Manual Installation](#manual-installation)
4. [Sensor Setup](#sensor-setup)
5. [Troubleshooting](#troubleshooting)
6. [Platform-Specific Notes](#platform-specific-notes)

---

## 🖥️ System Requirements

### Minimum Requirements
- Linux kernel 2.6.25 or later
- GCC 4.8 or later
- GNU Make
- Terminal with Unicode support
- Read access to `/sys/class/hwmon/`

### Recommended
- Linux kernel 4.4 or later (better sensor support)
- lm-sensors package
- Kernel modules: coretemp, k10temp, or zenpower
- Terminal with 256 color support

### Tested Platforms
- ✓ MX Linux 21+
- ✓ Ubuntu 20.04, 22.04, 24.04
- ✓ Debian 10, 11, 12
- ✓ Arch Linux
- ✓ Fedora 35+
- ✓ Linux Mint 20+
- ✓ Pop!_OS 20.04+

---

## 🚀 Quick Installation

### Method 1: Using Build Script (Recommended)

```bash
# Clone the repository
git clone https://github.com/yourusername/temp-monitor.git
cd temp-monitor

# Make build script executable
chmod +x build.sh

# Build and install
./build.sh --build
sudo ./build.sh --install

# Setup sensors (first time only)
./build.sh --setup

# Run
tempmonitor
```

### Method 2: Using Make

```bash
# Clone the repository
git clone https://github.com/yourusername/temp-monitor.git
cd temp-monitor

# Build
make

# Install (optional)
sudo make install

# Run
./bin/tempmonitor
# or if installed:
tempmonitor
```

---

## 🔧 Manual Installation

### Step 1: Install Dependencies

#### Debian/Ubuntu/MX Linux
```bash
sudo apt update
sudo apt install build-essential git lm-sensors
```

#### Arch Linux
```bash
sudo pacman -S base-devel git lm_sensors
```

#### Fedora
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install git lm_sensors
```

### Step 2: Clone Repository
```bash
git clone https://github.com/yourusername/temp-monitor.git
cd temp-monitor
```

### Step 3: Compile
```bash
make
```

This will create the executable at `bin/tempmonitor`

### Step 4: Install System-Wide (Optional)
```bash
sudo make install
```

This installs to `/usr/local/bin/tempmonitor`

### Step 5: Verify Installation
```bash
tempmonitor --version
tempmonitor --list
```

---

## 🌡️ Sensor Setup

### Initial Sensor Detection

```bash
# Method 1: Using build script
./build.sh --setup

# Method 2: Manual setup
sudo apt install lm-sensors
sudo sensors-detect --auto
sudo systemctl restart kmod
```

### Load Sensor Modules

#### Automatic Loading
```bash
./build.sh --load-modules
```

#### Manual Loading

**For Intel CPUs:**
```bash
sudo modprobe coretemp
```

**For AMD CPUs (K10, K11, K12):**
```bash
sudo modprobe k10temp
```

**For AMD Ryzen:**
```bash
sudo modprobe zenpower
```

**For NVMe drives:**
```bash
sudo modprobe nvme
```

### Permanent Module Loading

To load modules automatically at boot:

```bash
# Create modules config
sudo nano /etc/modules-load.d/sensors.conf
```

Add these lines:
```
coretemp
k10temp
nvme
```

Save and reboot.

### Verify Sensors

```bash
# Check loaded modules
lsmod | grep -E "coretemp|k10temp|zenpower"

# Check available sensors
ls -la /sys/class/hwmon/

# Test with lm-sensors
sensors

# Test with our tool
tempmonitor --list
```

---

## 🐛 Troubleshooting

### Problem: No Sensors Detected

**Solution 1: Load Kernel Modules**
```bash
# Check if modules are available
modprobe -l | grep -E "coretemp|k10temp"

# Load appropriate module
sudo modprobe coretemp  # Intel
sudo modprobe k10temp   # AMD
```

**Solution 2: Install lm-sensors**
```bash
sudo apt install lm-sensors
sudo sensors-detect
```

**Solution 3: Check Permissions**
```bash
# Verify you can read sensor files
cat /sys/class/hwmon/hwmon0/temp1_input

# If permission denied, check groups
groups
sudo usermod -aG systemd-journal $USER
```

### Problem: AMD Ryzen Not Detected

Install zenpower driver:

```bash
# Install dependencies
sudo apt install dkms linux-headers-$(uname -r)

# Clone zenpower
git clone https://github.com/ocerman/zenpower3.git
cd zenpower3

# Build and install
make
sudo make install
sudo modprobe zenpower

# Blacklist k10temp (optional)
echo "blacklist k10temp" | sudo tee /etc/modprobe.d/blacklist-k10temp.conf
sudo update-initramfs -u
```

### Problem: Compilation Errors

**Missing build-essential:**
```bash
sudo apt install build-essential
```

**Wrong GCC version:**
```bash
gcc --version  # Should be 4.8+
sudo apt install gcc-11  # If needed
```

**Missing headers:**
```bash
sudo apt install linux-headers-$(uname -r)
```

### Problem: Display Issues

**Unicode characters not showing:**
```bash
# Check locale
locale

# Install UTF-8 locale
sudo apt install locales
sudo locale-gen en_US.UTF-8
export LANG=en_US.UTF-8
```

**Colors not working:**
```bash
# Check terminal supports 256 colors
echo $TERM

# Should output: xterm-256color or similar
# If not, set it:
export TERM=xterm-256color
```

---

## 🖥️ Platform-Specific Notes

### MX Linux

MX Linux usually works out of the box. If sensors aren't detected:

```bash
# Use MX Linux tools
mx-packageinstaller  # Search and install lm-sensors

# Or command line
sudo apt install lm-sensors
sudo sensors-detect

# Load modules
./build.sh --load-modules
```

### Ubuntu/Debian

```bash
# Standard installation
sudo apt update
sudo apt install build-essential lm-sensors

# For newer Ubuntu (22.04+)
sudo apt install gcc-11 make

# Build and install
make && sudo make install
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S base-devel lm_sensors

# AUR helper (if needed)
yay -S zenpower3-dkms  # For Ryzen

# Build
make && sudo make install
```

### Fedora/RHEL

```bash
# Install dependencies
sudo dnf groupinstall "Development Tools"
sudo dnf install lm_sensors

# Load modules
sudo modprobe coretemp k10temp

# Build
make && sudo make install
```

---

## 🔄 Updating

### Update from Git

```bash
cd temp-monitor
git pull
make clean
make
sudo make install
```

### Update Build Script

```bash
./build.sh --rebuild
sudo ./build.sh --install
```

---

## 🗑️ Uninstallation

### Remove Installed Binary

```bash
sudo make uninstall
# or
sudo rm /usr/local/bin/tempmonitor
```

### Remove Build Files

```bash
make clean
```

### Complete Removal

```bash
cd temp-monitor
make clean
sudo make uninstall
cd ..
rm -rf temp-monitor
```

---

## ✅ Post-Installation

### Test Installation

```bash
# Check version
tempmonitor --version

# List sensors
tempmonitor --list

# Run with stats
tempmonitor -s

# Run for 5 seconds then exit (Ctrl+C)
tempmonitor 1
```

### Set Up Autostart (Optional)

To run at login, create a desktop file:

```bash
mkdir -p ~/.config/autostart
nano ~/.config/autostart/tempmonitor.desktop
```

Add:
```ini
[Desktop Entry]
Type=Application
Name=Temperature Monitor
Exec=/usr/local/bin/tempmonitor
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
```

---

## 📞 Getting Help

If you encounter issues:

1. Check [Troubleshooting](#troubleshooting) section
2. Run diagnostics: `./build.sh --check`
3. Check logs: `dmesg | grep -i temp`
4. Open an issue on GitHub with:
   - Your Linux distribution and version
   - Output of `./build.sh --check`
   - Error messages

---

## 🎯 Next Steps

After successful installation:

1. Read the [README.md](README.md) for usage instructions
2. Try different display options
3. Set up your preferred refresh rate
4. Configure autostart if desired

**Enjoy monitoring your hardware temperatures!** 🔥

---

<div align="center">

**Installation complete!** ✓

Run `tempmonitor --help` for usage information

</div>
