Name:           temp
Version:        0.0.2
Release:        1%{?dist}
Summary:        Real-time hardware temperature monitor for Linux

License:        MIT
URL:            https://github.com/danko1122/temp-monitor
Source0:        https://github.com/danko1122/temp-monitor/archive/refs/tags/v%{version}.tar.gz#/temp-monitor-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%description
Temp Monitor - Real-time terminal-based temperature monitoring tool.
Supports CPU, GPU, NVMe, chipset, and fan sensors.
Auto-detects sensors from Linux hwmon subsystem.
Features color-coded display and statistics tracking.

%prep
%autosetup -n temp-monitor-%{version}

%build
make %{?_smp_mflags}

%install
install -D -m 755 bin/temp %{buildroot}%{_bindir}/temp

%files
%license LICENSE
%doc README.md USAGE.md CHANGELOG.md
%{_bindir}/temp

%changelog
* Thu Dec 05 2024 Danko <davaniko1122@gmail.com> - 0.0.2-1
- Fixed display leaving trail/ghost output in terminal
- Implemented alternate screen buffer for clean display
- Added comprehensive code documentation

* Thu Dec 05 2024 Danko <davaniko1122@gmail.com> - 0.0.1-1
- Initial release
