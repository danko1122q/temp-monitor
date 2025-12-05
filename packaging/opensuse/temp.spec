Name:           temp
Version:        0.0.1
Release:        1
Summary:        Hardware temperature monitor for Linux
License:        MIT
Group:          System/Monitoring
URL:            https://github.com/danko1122/temp-monitor
Source:         %{name}-%{version}.tar.gz

BuildRequires:  gcc
BuildRequires:  make

%description
Real-time terminal-based temperature monitoring tool.
Supports CPU, GPU, NVMe, chipset, and fan sensors.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
install -D -m 755 bin/temp %{buildroot}%{_bindir}/temp

%files
%doc README.md USAGE.md
%{_bindir}/temp
