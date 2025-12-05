# Release Guide

Panduan lengkap untuk merilis versi baru Temp Monitor.

## Alur Release

```
1. Update Version
       ↓
2. Update CHANGELOG.md
       ↓
3. Update Packaging Files
       ↓
4. Commit & Push
       ↓
5. Create Git Tag
       ↓
6. Push Tag (triggers CI/CD)
       ↓
7. GitHub Actions builds binaries
       ↓
8. Release created automatically
       ↓
9. Update Linux repos (manual/optional)
```

---

## 1. Update Version

Edit file-file berikut dengan versi baru:

### main.c
```c
#define VERSION "0.0.3"
```

### main.h (jika ada)
```c
#define TEMP_VERSION "0.0.3"
```

### build.sh
```bash
VERSION="0.0.3"
```

---

## 2. Update CHANGELOG.md

Tambahkan entry baru di bagian atas:

```markdown
## [0.0.3] - 2024-12-06

### Added
- Fitur baru

### Changed
- Perubahan

### Fixed
- Perbaikan bug
```

---

## 3. Update Packaging Files

### Debian (packaging/debian/changelog)
```
temp (0.0.3-1) unstable; urgency=low

  * Deskripsi perubahan

 -- Danko <davaniko1122@gmail.com>  Fri, 06 Dec 2024 00:00:00 +0000
```

### Arch Linux (packaging/arch/PKGBUILD)
```bash
pkgver=0.0.3
```

### RPM - Fedora (packaging/rpm/temp.spec)
```
Version:        0.0.3

%changelog
* Fri Dec 06 2024 Danko <davaniko1122@gmail.com> - 0.0.3-1
- Deskripsi perubahan
```

### Alpine (packaging/alpine/APKBUILD)
```bash
pkgver=0.0.3
```

### openSUSE (packaging/opensuse/temp.spec)
```
Version:        0.0.3
```

### Gentoo
```bash
# Rename file
mv packaging/gentoo/temp-0.0.2.ebuild packaging/gentoo/temp-0.0.3.ebuild
```

---

## 4. Commit Changes

```bash
git add -A
git commit -m "Release v0.0.3"
git push origin main
```

---

## 5. Create Git Tag

```bash
# Create annotated tag
git tag -a v0.0.3 -m "Version 0.0.3 - Deskripsi singkat"

# Atau dengan GPG signing (recommended)
git tag -s v0.0.3 -m "Version 0.0.3 - Deskripsi singkat"
```

---

## 6. Push Tag

```bash
git push origin v0.0.3

# Atau push semua tags
git push origin --tags
```

Setelah tag di-push, GitHub Actions akan:
1. Build binary untuk x86_64 dan aarch64
2. Generate source tarball
3. Create checksums (SHA256, SHA512)
4. Publish release otomatis

---

## 7. Verify Release

1. Buka https://github.com/danko1122/temp-monitor/releases
2. Pastikan semua asset tersedia:
   - `temp-monitor-0.0.3.tar.gz` (source)
   - `temp-0.0.3-linux-x86_64` (binary)
   - `temp-0.0.3-linux-aarch64` (binary)
   - `SHA256SUMS.txt`
   - `SHA512SUMS.txt`

---

## Hubungan dengan Linux Packaging

### Source Archive

GitHub secara otomatis membuat source archive untuk setiap tag:
```
https://github.com/danko1122/temp-monitor/archive/refs/tags/v0.0.2.tar.gz
```

Archive ini berisi:
- Semua source code (*.c, *.h)
- Makefile dan build scripts
- Dokumentasi (README, USAGE, CHANGELOG)
- Packaging files (packaging/*)
- License

**Penting:** Packaging scripts harus menggunakan URL ini, BUKAN binary releases.

### Bagaimana Packaging Scripts Mengambil Source

**Arch Linux (PKGBUILD):**
```bash
source=("$pkgname-$pkgver.tar.gz::https://github.com/danko1122/temp-monitor/archive/refs/tags/v$pkgver.tar.gz")
# Extracts to: temp-monitor-$pkgver/
```

**Fedora/RPM (temp.spec):**
```
Source0: https://github.com/danko1122/temp-monitor/archive/refs/tags/v%{version}.tar.gz
# Extracts to: temp-monitor-%{version}/
```

**Gentoo (ebuild):**
```bash
SRC_URI="https://github.com/danko1122/temp-monitor/archive/v${PV}.tar.gz -> ${P}.tar.gz"
# Extracts to: temp-monitor-${PV}/
```

**Alpine (APKBUILD):**
```bash
source="$pkgname-$pkgver.tar.gz::https://github.com/danko1122/temp-monitor/archive/refs/tags/v$pkgver.tar.gz"
builddir="$srcdir/temp-monitor-$pkgver"
```

### Struktur Extracted Archive

```
temp-monitor-0.0.2/
├── main.c, main.h
├── display.c, display.h
├── sensor.c, sensor.h
├── utils.c, utils.h
├── Makefile
├── LICENSE
├── README.md
├── USAGE.md
├── CHANGELOG.md
├── build.sh
├── install.sh
├── uninstall.sh
└── packaging/
    ├── debian/
    ├── arch/
    ├── rpm/
    ├── alpine/
    ├── gentoo/
    └── opensuse/
```

### Alur Distribusi ke Repository Linux

```
GitHub Release
     ↓
┌────────────────────────────────────────────┐
│                                            │
├→ AUR (Arch User Repository)                │
│   • Update PKGBUILD                        │
│   • Generate .SRCINFO                      │
│   • Push ke AUR git                        │
│                                            │
├→ COPR (Fedora Community)                   │
│   • Upload SRPM atau                       │
│   • Link ke GitHub repo                    │
│                                            │
├→ PPA (Ubuntu/Debian)                       │
│   • Build source package                   │
│   • Sign dengan GPG                        │
│   • Upload dengan dput                     │
│                                            │
├→ OBS (openSUSE Build Service)              │
│   • Add package via web/osc                │
│                                            │
└→ GURU (Gentoo User Repository)             │
    • Submit ebuild via PR                   │
└────────────────────────────────────────────┘
```

---

## Update ke AUR (Arch Linux)

```bash
# Clone AUR package
git clone ssh://aur@aur.archlinux.org/temp.git
cd temp

# Update PKGBUILD
vim PKGBUILD  # Update pkgver, checksums

# Generate .SRCINFO
makepkg --printsrcinfo > .SRCINFO

# Commit dan push
git add PKGBUILD .SRCINFO
git commit -m "Update to 0.0.3"
git push
```

### Generate Checksum untuk PKGBUILD

```bash
# Download source tarball
wget https://github.com/danko1122/temp-monitor/archive/refs/tags/v0.0.3.tar.gz

# Generate checksums
sha256sum v0.0.3.tar.gz
# atau
makepkg -g  # Auto-generate checksums
```

---

## Update ke COPR (Fedora)

1. Login ke https://copr.fedorainfracloud.org
2. Buka project Anda
3. Pilih "Builds" → "New Build"
4. Pilih "Upload SRPM" atau "SCM" (link ke GitHub)
5. Submit build

---

## Update ke PPA (Ubuntu/Debian)

```bash
# Buat source package
cd temp-monitor
cp -r packaging/debian .
dpkg-buildpackage -S -sa

# Sign dengan GPG
debsign ../temp_0.0.3-1_source.changes

# Upload ke PPA
dput ppa:username/ppa ../temp_0.0.3-1_source.changes
```

---

## Checklist Release

- [ ] Version updated di main.c
- [ ] Version updated di main.h
- [ ] Version updated di build.sh
- [ ] CHANGELOG.md updated
- [ ] packaging/debian/changelog updated
- [ ] packaging/arch/PKGBUILD updated
- [ ] packaging/rpm/temp.spec updated
- [ ] packaging/alpine/APKBUILD updated
- [ ] packaging/opensuse/temp.spec updated
- [ ] packaging/gentoo/temp-X.X.X.ebuild renamed
- [ ] All changes committed
- [ ] Git tag created
- [ ] Tag pushed to GitHub
- [ ] GitHub release verified
- [ ] (Optional) AUR updated
- [ ] (Optional) COPR updated
- [ ] (Optional) PPA updated

---

## Semantic Versioning

Format: `MAJOR.MINOR.PATCH`

- **MAJOR**: Perubahan breaking/incompatible
- **MINOR**: Fitur baru (backward compatible)
- **PATCH**: Bug fixes

Contoh:
- `0.0.1` → `0.0.2` : Bug fix
- `0.0.2` → `0.1.0` : Fitur baru
- `0.1.0` → `1.0.0` : Stable release pertama

---

## Tips

1. **Test sebelum release:**
   ```bash
   make clean && make
   ./bin/temp --version
   ./bin/temp -s 1
   ```

2. **Gunakan GPG signing untuk tag:**
   ```bash
   git tag -s v0.0.3 -m "Release v0.0.3"
   ```

3. **Preview release notes:**
   Tulis release notes di CHANGELOG.md terlebih dahulu

4. **Rollback jika ada masalah:**
   ```bash
   git tag -d v0.0.3
   git push origin :refs/tags/v0.0.3
   ```

---

v0.0.2
