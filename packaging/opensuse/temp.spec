# Temp Monitor - Real-time hardware temperature monitoring for Linux
# Copyright (c) 2024 Danko

Name:           temp
Version:        0.0.2
Release:        1
Summary:        Real-time hardware temperature monitor for Linux
License:        MIT
Group:          System/Monitoring
URL:            https://github.com/danko1122/temp-monitor
Source:         https://github.com/danko1122/temp-monitor/archive/refs/tags/v%{version}.tar.gz#/temp-monitor-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%description
Temp Monitor - Real-time terminal-based temperature monitoring tool.
Supports CPU, GPU, NVMe, chipset, and fan sensors.
Auto-detects sensors from Linux hwmon subsystem.

%prep
%setup -q -n temp-monitor-%{version}

%build
make %{?_smp_mflags}

%install
install -D -m 755 bin/temp %{buildroot}%{_bindir}/temp

%files
%license LICENSE
%doc README.md USAGE.md CHANGELOG.md
%{_bindir}/temp

%changelog
