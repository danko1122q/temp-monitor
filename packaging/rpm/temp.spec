Name:           temp
Version:        0.0.1
Release:        1%{?dist}
Summary:        Hardware temperature monitor for Linux

License:        MIT
URL:            https://github.com/danko1122/temp-monitor
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%description
Real-time terminal-based temperature monitoring tool.
Supports CPU, GPU, NVMe, chipset, and fan sensors.
Auto-detects sensors from hwmon subsystem.

%prep
%autosetup

%build
make %{?_smp_mflags}

%install
install -D -m 755 bin/temp %{buildroot}%{_bindir}/temp

%files
%license LICENSE
%doc README.md USAGE.md
%{_bindir}/temp

%changelog
* Thu Dec 05 2024 Danko <davaniko1122@gmail.com> - 0.0.1-1
- Initial release
