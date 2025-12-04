# 🌡️ Advanced Hardware Temperature Monitor

Monitor suhu hardware secara realtime untuk Linux (MX Linux/Debian/Ubuntu) dengan tampilan visual yang menarik dan fitur-fitur canggih.

![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux-orange.svg)

## ✨ Fitur Utama

- 🔍 **Auto-detect** semua sensor hardware (CPU, GPU, NVMe, Chipset, dll)
- ⚡ **Realtime monitoring** dengan refresh rate yang dapat dikustomisasi
- 🎨 **Visualisasi bar warna** berdasarkan tingkat suhu
- 📊 **Statistik sistem** (rata-rata, min/max temperature)
- 🌡️ **Celsius & Fahrenheit** support
- 🖥️ **Multi-core CPU** detection
- 💾 **NVMe SSD** temperature monitoring
- 🎮 **GPU detection** (AMD, NVIDIA, Nouveau)
- 📈 **Min/Max tracking** untuk setiap sensor
- 🎯 **Zero dependencies** - hanya butuh GCC dan standard library

## 📋 Persyaratan Sistem

### Minimal Requirements
- **OS**: Linux (Ubuntu 18.04+, Debian 9+, MX Linux 19+)
- **Compiler**: GCC 7.0+ atau Clang 5.0+
- **Kernel**: Linux 3.10+ (dengan hwmon support)
- **RAM**: 10 MB
- **Privileges**: User biasa (tidak perlu root)

### Software Dependencies
```bash
# Install build tools (jika belum ada)
sudo apt install build-essential

# Install lm-sensors (untuk deteksi sensor yang lebih baik)
sudo apt install lm-sensors
```

## 🚀 Cara Build & Install

### 1️⃣ Clone atau Download Source Code

```bash
# Buat direktori project
mkdir -p ~/projects/temp-monitor
cd ~/projects/temp-monitor

# Download file check_temp.c dan README.md
# (atau copy dari artifact)
```

### 2️⃣ Compile Program

#### Opsi 1: Build Standar
```bash
gcc -o check_temp check_temp.c -Wall -O2
```

#### Opsi 2: Build dengan Optimasi Tinggi
```bash
gcc -o check_temp check_temp.c -Wall -O3 -march=native -flto
```

#### Opsi 3: Build untuk Debugging
```bash
gcc -o check_temp check_temp.c -Wall -g -O0
```

### 3️⃣ Verifikasi Build
```bash
# Cek apakah binary berhasil dibuat
ls -lh check_temp

# Test jalankan
./check_temp --help
```

### 4️⃣ Install ke System (Opsional)

```bash
# Install ke /usr/local/bin
sudo cp check_temp /usr/local/bin/
sudo chmod +x /usr/local/bin/check_temp

# Sekarang bisa dipanggil dari mana saja
check_temp
```

## 📖 Cara Penggunaan

### Basic Usage

```bash
# Jalankan dengan default settings (refresh 2 detik, Celsius)
./check_temp

# Jalankan dengan refresh 1 detik
./check_temp 1

# Jalankan dengan refresh 5 detik
./check_temp 5
```

### Advanced Options

```bash
# Tampilkan help
./check_temp --help
./check_temp -h

# Tampilkan versi
./check_temp --version
./check_temp -v

# Gunakan Fahrenheit
./check_temp --fahrenheit
./check_temp -f

# Tampilkan statistik tambahan
./check_temp --stats
./check_temp -s

# List semua sensor yang terdeteksi (tanpa monitoring)
./check_temp --list
./check_temp -l

# Kombinasi options
./check_temp -s -f 3        # Stats + Fahrenheit + refresh 3 detik
./check_temp --stats 1      # Stats + refresh 1 detik
```

### Contoh Output

```
╔══════════════════════════════════════════════════════════════════════╗
║         ★ ADVANCED HARDWARE TEMPERATURE MONITOR v2.0.0 ★           ║
║  2024-12-04 15:30:45  |  MX Linux Compatible                       ║
╚══════════════════════════════════════════════════════════════════════╝

▓▓▓ CPU TEMPERATURES (8 sensors) ▓▓▓
  Package id 0                    65.0°C  [████████████████░░░░] 
  Core 0                          62.0°C  [██████████████░░░░░░]
  Core 1                          64.0°C  [███████████████░░░░░]
  Core 2                          66.0°C  [████████████████░░░░]
  Core 3                          63.0°C  [███████████████░░░░░]

▓▓▓ GPU TEMPERATURES (1 sensor) ▓▓▓
  edge                            58.0°C  [█████████████░░░░░░░]

▓▓▓ NVME TEMPERATURES (1 sensor) ▓▓▓
  Composite                       42.0°C  [█████████░░░░░░░░░░░]

══════════════════════════════════════════════════════════════════════
Legend: <40°C 40-50°C 50-60°C 60-70°C 70-80°C 80-90°C >90°C
Controls: Ctrl+C=Exit | F/C=Toggle °F/°C | S=Stats
```

## 🔧 Troubleshooting

### Problem: Tidak Ada Sensor Terdeteksi

**Solusi 1: Load Kernel Modules**
```bash
# Untuk Intel CPU
sudo modprobe coretemp

# Untuk AMD CPU (pilih salah satu)
sudo modprobe k10temp
# atau
sudo modprobe zenpower

# Untuk GPU AMD
sudo modprobe amdgpu

# Untuk GPU NVIDIA (jika menggunakan driver proprietary)
sudo modprobe nvidia
```

**Solusi 2: Setup lm-sensors**
```bash
# Install lm-sensors
sudo apt update
sudo apt install lm-sensors

# Jalankan deteksi sensor
sudo sensors-detect

# Jawab 'YES' untuk semua pertanyaan
# Restart service
sudo systemctl restart kmod

# Test dengan sensors
sensors
```

**Solusi 3: Verifikasi Hardware Support**
```bash
# Cek apakah hwmon tersedia
ls -la /sys/class/hwmon/

# Cek isi setiap hwmon
for dir in /sys/class/hwmon/hwmon*; do
    echo "=== $dir ==="
    cat $dir/name 2>/dev/null
    ls $dir/temp*_input 2>/dev/null
done

# Cek kernel modules yang loaded
lsmod | grep -E 'coretemp|k10temp|zenpower|amdgpu|nvidia'
```

### Problem: Permission Denied

```bash
# Pastikan file sensor readable
ls -la /sys/class/hwmon/hwmon*/temp*_input

# Jika tidak, tambahkan user ke group yang sesuai
sudo usermod -a -G video $USER
sudo usermod -a -G render $USER

# Logout dan login kembali
```

### Problem: Suhu Tidak Akurat

**Untuk AMD Ryzen:**
```bash
# Install zenpower (lebih akurat daripada k10temp untuk Ryzen)
sudo apt install dkms git
git clone https://github.com/ocerman/zenpower.git
cd zenpower
make
sudo make install
sudo modprobe zenpower

# Blacklist k10temp
echo "blacklist k10temp" | sudo tee /etc/modprobe.d/blacklist-k10temp.conf
sudo update-initramfs -u
```

**Untuk Intel:**
```bash
# Pastikan coretemp di-load
sudo modprobe coretemp
lsmod | grep coretemp
```

### Problem: Kompilasi Error

**Error: `implicit declaration of function 'tolower'`**
- Sudah diperbaiki di versi 2.0.0 dengan `#include <ctype.h>`

**Error: `format-truncation warning`**
- Ini hanya warning, bukan error
- Program tetap bisa jalan
- Sudah diperbaiki dengan buffer size yang lebih besar

## 📊 Color Legend

| Warna | Suhu | Status |
|-------|------|--------|
| 🔵 Cyan | < 40°C | Dingin |
| 🟢 Green | 40-50°C | Aman |
| 🟢 Light Green | 50-60°C | Normal |
| 🟡 Yellow | 60-70°C | Hangat |
| 🟡 Light Yellow | 70-80°C | Tinggi |
| 🔴 Light Red | 80-90°C | Sangat Tinggi |
| 🔴 **Red Bold** | > 90°C | **BAHAYA!** |

## 🎯 Tested On

- ✅ MX Linux 21, 23
- ✅ Ubuntu 20.04, 22.04, 24.04
- ✅ Debian 11, 12
- ✅ Linux Mint 20, 21
- ✅ Pop!_OS 22.04

### Hardware Support

**CPU:**
- ✅ Intel Core (Sandy Bridge dan lebih baru)
- ✅ AMD Ryzen (semua generasi)
- ✅ AMD FX, A-Series
- ✅ AMD EPYC, Threadripper

**GPU:**
- ✅ AMD Radeon (dengan driver amdgpu)
- ✅ NVIDIA (dengan driver proprietary)
- ✅ Intel Integrated Graphics

**Storage:**
- ✅ NVMe SSD (Samsung, WD, Crucial, dll)
- ✅ SATA SSD dengan S.M.A.R.T support

## 🔄 Auto-start on Boot (Optional)

### Systemd Service

Buat file `/etc/systemd/system/temp-monitor.service`:

```ini
[Unit]
Description=Hardware Temperature Monitor
After=multi-user.target

[Service]
Type=simple
ExecStart=/usr/local/bin/check_temp 5
StandardOutput=journal
StandardError=journal
Restart=always
User=dava

[Install]
WantedBy=multi-user.target
```

Enable service:
```bash
sudo systemctl daemon-reload
sudo systemctl enable temp-monitor.service
sudo systemctl start temp-monitor.service

# Check status
sudo systemctl status temp-monitor.service
```

## 📝 Tips & Tricks

### 1. Monitor dalam Background
```bash
# Jalankan di background
./check_temp 5 &

# Lihat dengan screen atau tmux
screen -S temp
./check_temp 1
# Ctrl+A, D untuk detach
```

### 2. Export ke Log File
```bash
# Redirect output ke file
./check_temp 10 > temp_log.txt 2>&1 &

# Monitor dengan tail
tail -f temp_log.txt
```

### 3. Alert untuk Suhu Tinggi
Tambahkan script untuk alert:
```bash
#!/bin/bash
while true; do
    temp=$(sensors | grep "Package id 0" | awk '{print $4}' | cut -d'.' -f1 | tr -d '+')
    if [ "$temp" -gt 80 ]; then
        notify-send "⚠️ CPU Overheat!" "Temperature: ${temp}°C"
    fi
    sleep 30
done
```

## 🤝 Contributing

Kontribusi sangat diterima! Silakan:
1. Fork repository
2. Buat branch untuk fitur baru
3. Commit changes
4. Push ke branch
5. Buat Pull Request

## 📄 License

MIT License - Bebas digunakan untuk personal maupun komersial

## 👨‍💻 Author

**Dava**
- Platform: MX Linux
- Version: 2.0.0
- Last Update: December 2024

## 🔗 Resources

- [Linux Hardware Monitoring](https://www.kernel.org/doc/html/latest/hwmon/index.html)
- [lm-sensors Project](https://github.com/lm-sensors/lm-sensors)
- [sysfs Documentation](https://www.kernel.org/doc/Documentation/filesystems/sysfs.txt)

## 📞 Support

Jika mengalami masalah:
1. Baca bagian Troubleshooting di atas
2. Jalankan `./check_temp --list` untuk cek sensor
3. Jalankan `sensors` untuk verifikasi
4. Cek `dmesg | grep -i hwmon` untuk error kernel

---

**⭐ Jika program ini bermanfaat, jangan lupa beri star!**
