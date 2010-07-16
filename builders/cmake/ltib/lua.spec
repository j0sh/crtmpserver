%define pfx /opt/freescale/rootfs/%{_target_cpu}

Summary         : The LUA programming language
Name            : lua
Version         : 5.1.4
Release         : 1
License         : Free http://www.lua.org/license.html
Vendor          : www.lua.org
Packager        : Theo Aukerman
Group           : xxxx
URL             : http://www.lua.org/ftp
Source          : %{name}-%{version}.tar.gz
BuildRoot       : %{_tmppath}/%{name}
Prefix          : %{pfx}

%Description
%{summary}

%Prep
%setup 

%Build
pwd
echo name=%{name}, version=%{version}, _tmppath=%{_tmppath}, pfx=%{pfx}, RPM_BUILD_ROOT=$RPM_BUILD_ROOT
make linux
%Install
rm -rf $RPM_BUILD_ROOT
make install INSTALL_TOP=$RPM_BUILD_ROOT/%{pfx}/usr
cp $RPM_BUILD_ROOT/%{pfx}/usr/lib/liblua.a $RPM_BUILD_ROOT/%{pfx}/usr/lib/liblua5.1.so

%Clean
rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*
