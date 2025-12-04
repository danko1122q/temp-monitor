# ⚡ Quick Start Guide

Get up and running with Ultimate Hardware Temperature Monitor in 5 minutes!

---

## 🚀 Installation (60 seconds)

### One-Line Install

```bash
git clone https://github.com/yourusername/temp-monitor.git && cd temp-monitor && chmod +x build.sh && ./build.sh --build
```

### Step by Step

```bash
# 1. Clone
git clone https://github.com/yourusername/temp-monitor.git
cd temp-monitor

# 2. Build
chmod +x build.sh
./build.sh --build

# 3. Done!
./bin/tempmonitor
```

---

## 🎯 First Run

### Option 1: Direct Run
```bash
./bin/tempmonitor
```

### Option 2: Install Then Run
```bash
sudo ./build.sh --install
tempmonitor
```

---

## ⚠️ No Sensors? Fix in 2 Minutes

### Quick Fix
```bash
# Setup sensors automatically
./build.sh --setup

# Load modules
./build.sh --load-modules

# Try again
./bin/tempmonitor
```

### Manual Fix
```bash
# Install lm-sensors
sudo apt install lm-sensors

# Detect sensors
sudo sensors-detect --auto

# Load modules
sudo modprobe coretemp k10temp

# Run
./bin/tempmonitor
```

---

## 📖 Basic Usage

### Simple Commands

```bash
# Default (2 second refresh)
tempmonitor

# Fast updates (1 second)
tempmonitor 1

# With statistics
tempmonitor -s

# Slow updates (5 seconds)
tempmonitor 5

# List all sensors
tempmonitor --list

# Fahrenheit mode
tempmonitor -f
```

### Combined Options

```bash
# Stats + Fahrenheit + 3 second refresh
tempmonitor -s -f 3

# Short version
tempmonitor -sf 3
```

---

## 🎨 What You'll See

```
╔═══════════════════════════════════════════════════════════╗
║  🔥 ULTIMATE HARDWARE TEMPERATURE MONITOR v0.0.1         ║
║  ⏰ 2024-12-04 15:30:45  │  ✓ Real-time Monitoring       ║
╚═══════════════════════════════════════════════════════════╝

┌─ ⚙ CPU SENSORS (4 detected) ──────────────────────────────
│ Core 0           45.0°C [████████████░░░░░░░░]
│ Core 1           47.0°C [█████████████░░░░░░░]
│ Package id 0     48.0°C [█████████████░░░░░░░]

┌─ 🎮 GPU SENSORS (1 detected) ──────────────────────────────
│ edge             55.0°C [███████████████░░░░░]

Temperature Ranges: <40°C 40-50°C 50-60°C 60-70°C 70-80°C 80-90°C >90°C
Controls: Ctrl+C=Exit │ Refresh: 2s
```

---

## 🎯 Common Tasks

### Check What Sensors You Have
```bash
tempmonitor --list
```

### Monitor CPU Temperature
```bash
# Shows CPU temps prominently
tempmonitor -s
```

### Watch GPU Temperature
```bash
# All sensors including GPU
tempmonitor 1
```

### Save Sensor List
```bash
tempmonitor --list > my-sensors.txt
```

---

## 🛠️ Troubleshooting One-Liners

### "No sensors detected"
```bash
./build.sh --setup && ./build.sh --load-modules && tempmonitor --list
```

### "Permission denied"
```bash
ls -la /sys/class/hwmon/  # Check if readable
```

### "Module not found"
```bash
# Intel
sudo modprobe coretemp

# AMD
sudo modprobe k10temp
```

### "Compilation failed"
```bash
sudo apt install build-essential
make clean && make
```

---

## 🎓 Next Steps

### Learn More
1. Read [README.md](README.md) for full documentation
2. Check [INSTALL.md](INSTALL.md) for detailed setup
3. Run `tempmonitor --help` for all options

### Customize
```bash
# Try different refresh rates
tempmonitor 1    # Fast
tempmonitor 5    # Slow
tempmonitor 10   # Very slow

# Try different modes
tempmonitor -s   # Statistics
tempmonitor -f   # Fahrenheit
tempmonitor -c   # Compact (coming soon)
```

### Advanced
```bash
# Check build system
make help

# Check your system
./build.sh --check

# Debug build
./build.sh --debug
```

---

## ⌨️ Keyboard Controls

While running:
- `Ctrl+C` - Exit program
- `Ctrl+Z` - Suspend (use `fg` to resume)

---

## 💡 Pro Tips

1. **Best refresh rate:** 2-3 seconds for general monitoring
2. **For stress testing:** Use 1 second refresh
3. **For background monitoring:** Use 5-10 seconds
4. **Compare with lm-sensors:** Run `sensors` to verify temps
5. **AMD Ryzen users:** Install zenpower for better accuracy

---

## 📊 Temperature Guidelines

| Range | Status | Action |
|-------|--------|--------|
| < 40°C | Cold | Normal (idle) |
| 40-60°C | Normal | Typical usage |
| 60-70°C | Warm | Heavy load OK |
| 70-80°C | Hot | Check cooling |
| 80-90°C | Very Hot | Improve cooling |
| > 90°C | Critical | Stop work, check system |

---

## 🐛 Quick Fixes

### Build Issues
```bash
# Clean and rebuild
make clean && make
```

### Sensor Issues
```bash
# Reload modules
sudo rmmod coretemp && sudo modprobe coretemp
```

### Display Issues
```bash
# Fix terminal
export TERM=xterm-256color
```

---

## 🎉 You're Ready!

You should now see:
- ✅ Real-time temperature monitoring
- ✅ Color-coded temperature bars
- ✅ Multiple sensor types (CPU, GPU, etc.)
- ✅ Automatic updates

If something's not working, check:
1. [Troubleshooting section above](#-troubleshooting-one-liners)
2. [Full INSTALL.md](INSTALL.md) guide
3. Output of: `./build.sh --check`

---

## 📞 Need Help?

```bash
# Show help
tempmonitor --help

# Show version
tempmonitor --version

# Check system
./build.sh --check

# List available Make commands
make help
```

### Still stuck?
- Check [README.md](README.md) for detailed docs
- Check [INSTALL.md](INSTALL.md) for installation help
- Open an issue on GitHub with error details

---

## 🚀 Advanced Quick Start

### Install Globally
```bash
make && sudo make install
tempmonitor  # Run from anywhere
```

### Auto-load Modules at Boot
```bash
echo "coretemp" | sudo tee -a /etc/modules
echo "k10temp" | sudo tee -a /etc/modules
```

### Create Desktop Launcher
```bash
cat > ~/.local/share/applications/tempmonitor.desktop << 'EOF'
[Desktop Entry]
Type=Application
Name=Temperature Monitor
Exec=tempmonitor
Terminal=true
Categories=System;Monitor;
EOF
```

---

<div align="center">

## ⭐ Quick Reference Card

| Command | Action |
|---------|--------|
| `tempmonitor` | Start monitoring (2s refresh) |
| `tempmonitor 1` | Fast monitoring (1s refresh) |
| `tempmonitor -s` | Show statistics |
| `tempmonitor -f` | Use Fahrenheit |
| `tempmonitor --list` | List all sensors |
| `Ctrl+C` | Exit |

**That's it! You're monitoring like a pro!** 🔥

</div>

---

**Time to first temperature reading: < 60 seconds** ⚡

Happy monitoring! 🌡️
