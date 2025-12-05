# Packaging Guide

Build and distribute Temp Monitor for different Linux distributions.

**Current Version: 0.0.2**

---

## Quick Reference

| Distribution | Package Format | Build Command |
|--------------|----------------|---------------|
| Debian/Ubuntu | .deb | `dpkg-buildpackage` |
| Fedora/RHEL | .rpm | `rpmbuild` |
| Arch Linux | pkg.tar.zst | `makepkg` |
| Alpine Linux | .apk | `abuild` |
| Gentoo | ebuild | `emerge` |
| openSUSE | .rpm | `rpmbuild` |

---

## Debian/Ubuntu (.deb)

### Prerequisites
```bash
sudo apt install build-essential debhelper devscripts
```

### Build Package
```bash
# Copy debian directory to source root
cp -r packaging/debian .

# Build package (unsigned)
dpkg-buildpackage -us -uc -b

# Package will be created in parent directory
ls ../temp_0.0.2-1_amd64.deb
```

### Install
```bash
sudo dpkg -i ../temp_0.0.2-1_amd64.deb
```

### Submit to Repository
1. Create account on https://launchpad.net
2. Create PPA (Personal Package Archive)
3. Sign package with GPG key
4. Upload using `dput`

```bash
# Sign and build source package
dpkg-buildpackage -S -sa

# Upload to PPA
dput ppa:yourusername/ppa ../temp_0.0.2-1_source.changes
```

---

## Fedora/RHEL (.rpm)

### Prerequisites
```bash
sudo dnf install rpm-build gcc make rpmdevtools
rpmdev-setuptree
```

### Build Package
```bash
# Create source tarball
tar czf ~/rpmbuild/SOURCES/temp-0.0.2.tar.gz \
    --transform 's,^,temp-0.0.2/,' \
    *.c *.h Makefile LICENSE README.md USAGE.md CHANGELOG.md

# Copy spec file
cp packaging/rpm/temp.spec ~/rpmbuild/SPECS/

# Build RPM
rpmbuild -bb ~/rpmbuild/SPECS/temp.spec
```

### Install
```bash
sudo rpm -i ~/rpmbuild/RPMS/x86_64/temp-0.0.2-1.*.rpm
```

### Submit to COPR
1. Create account on https://copr.fedorainfracloud.org
2. Create new project
3. Upload SRPM or link to Git repository

---

## Arch Linux (AUR)

### Prerequisites
```bash
sudo pacman -S base-devel git
```

### Build Package
```bash
# Copy PKGBUILD
cp packaging/arch/PKGBUILD .

# Create source tarball
tar czf temp-0.0.2.tar.gz \
    --transform 's,^,temp-0.0.2/,' \
    *.c *.h Makefile LICENSE README.md USAGE.md CHANGELOG.md

# Build and install
makepkg -si
```

### Submit to AUR
1. Create account on https://aur.archlinux.org
2. Generate SSH key and add to AUR account
3. Clone AUR repository:
```bash
git clone ssh://aur@aur.archlinux.org/temp.git
cd temp

# Copy PKGBUILD and .SRCINFO
cp /path/to/PKGBUILD .
makepkg --printsrcinfo > .SRCINFO

# Commit and push
git add PKGBUILD .SRCINFO
git commit -m "Update to version 0.0.2"
git push
```

---

## Alpine Linux (.apk)

### Prerequisites
```bash
sudo apk add alpine-sdk
sudo addgroup $USER abuild
abuild-keygen -a -i
```

### Build Package
```bash
# Setup build directory
mkdir -p ~/aports/testing/temp
cp packaging/alpine/APKBUILD ~/aports/testing/temp/

# Create source tarball
tar czf temp-0.0.2.tar.gz \
    --transform 's,^,temp-0.0.2/,' \
    *.c *.h Makefile LICENSE README.md

# Move tarball
mv temp-0.0.2.tar.gz ~/aports/testing/temp/

# Build
cd ~/aports/testing/temp
abuild -r
```

### Install
```bash
sudo apk add --allow-untrusted ~/packages/testing/x86_64/temp-0.0.2-r0.apk
```

---

## Gentoo (ebuild)

### Local Installation
```bash
# Create local overlay
sudo mkdir -p /var/db/repos/local/sys-apps/temp

# Copy ebuild
sudo cp packaging/gentoo/temp-0.0.2.ebuild \
    /var/db/repos/local/sys-apps/temp/

# Generate manifest
sudo ebuild /var/db/repos/local/sys-apps/temp/temp-0.0.2.ebuild manifest

# Install
sudo emerge sys-apps/temp
```

### Submit to GURU
1. Request access to GURU overlay
2. Clone GURU repository
3. Add ebuild following Gentoo guidelines
4. Submit pull request

---

## openSUSE (.rpm)

### Prerequisites
```bash
sudo zypper install rpm-build gcc make
```

### Build Package
```bash
# Setup rpmbuild
mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

# Create source tarball
tar czf ~/rpmbuild/SOURCES/temp-0.0.2.tar.gz \
    --transform 's,^,temp-0.0.2/,' \
    *.c *.h Makefile LICENSE README.md USAGE.md CHANGELOG.md

# Build
rpmbuild -bb packaging/opensuse/temp.spec
```

### Submit to OBS
1. Create account on https://build.opensuse.org
2. Create home project
3. Add package via web interface or `osc` tool

---

## Manual Install (Any Distribution)

```bash
# Clone repository
git clone https://github.com/danko1122/temp-monitor.git
cd temp-monitor

# Build
make

# Install to /usr/local/bin
sudo make install

# Or use install script
./install.sh

# Uninstall
sudo make uninstall
# or
./uninstall.sh
```

---

## Version Update Checklist

When releasing new version:

1. Update `VERSION` in:
   - `main.c`
   - `main.h`
   - `build.sh`

2. Update packaging files:
   - `packaging/debian/changelog`
   - `packaging/arch/PKGBUILD` (pkgver)
   - `packaging/rpm/temp.spec` (Version, %changelog)
   - `packaging/alpine/APKBUILD` (pkgver)
   - `packaging/opensuse/temp.spec` (Version)
   - Rename `packaging/gentoo/temp-X.X.X.ebuild`

3. Update documentation:
   - `CHANGELOG.md`
   - `README.md`
   - `USAGE.md`

---

## File Structure

```
packaging/
├── README.md           # This file
├── debian/
│   ├── changelog       # Version history
│   ├── compat          # Debhelper compat level
│   ├── control         # Package metadata
│   ├── copyright       # License info
│   └── rules           # Build rules
├── arch/
│   └── PKGBUILD        # Arch build script
├── rpm/
│   └── temp.spec       # Fedora/RHEL spec
├── alpine/
│   └── APKBUILD        # Alpine build script
├── gentoo/
│   └── temp-0.0.2.ebuild
└── opensuse/
    └── temp.spec       # openSUSE spec
```

---

v0.0.2
