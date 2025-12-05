# Packaging

Build packages for different Linux distributions.

## Debian/Ubuntu (.deb)

```bash
# Install build tools
sudo apt install build-essential debhelper

# Build
cp -r packaging/debian .
dpkg-buildpackage -us -uc -b

# Install
sudo dpkg -i ../temp_0.0.1-1_amd64.deb
```

## Fedora/RHEL (.rpm)

```bash
# Install build tools
sudo dnf install rpm-build gcc make

# Setup
mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
tar czf ~/rpmbuild/SOURCES/temp-0.0.1.tar.gz --transform 's,^,temp-0.0.1/,' .
cp packaging/rpm/temp.spec ~/rpmbuild/SPECS/

# Build
rpmbuild -bb ~/rpmbuild/SPECS/temp.spec

# Install
sudo rpm -i ~/rpmbuild/RPMS/x86_64/temp-0.0.1-1.*.rpm
```

## Arch Linux

```bash
# Copy files
cp packaging/arch/PKGBUILD .
tar czf temp-0.0.1.tar.gz --transform 's,^,temp-0.0.1/,' .

# Build
makepkg -si
```

## Alpine Linux

```bash
# Copy files
cp packaging/alpine/APKBUILD .
tar czf temp-0.0.1.tar.gz --transform 's,^,temp-0.0.1/,' .

# Build
abuild -r

# Install
sudo apk add --allow-untrusted ~/packages/*/temp-0.0.1-r0.apk
```

## Gentoo

```bash
# Create overlay
sudo mkdir -p /usr/local/portage/sys-apps/temp
sudo cp packaging/gentoo/temp-0.0.1.ebuild /usr/local/portage/sys-apps/temp/
sudo ebuild /usr/local/portage/sys-apps/temp/temp-0.0.1.ebuild manifest

# Install
sudo emerge temp
```

## openSUSE

```bash
# Install build tools
sudo zypper install rpm-build gcc make

# Build
rpmbuild -bb packaging/opensuse/temp.spec

# Install
sudo rpm -i ~/rpmbuild/RPMS/x86_64/temp-0.0.1-1.*.rpm
```

## Manual Install (any distro)

```bash
./install.sh
# or
make && sudo make install
```
