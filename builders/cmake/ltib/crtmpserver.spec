%define pfx /opt/freescale/rootfs/%{_target_cpu}

Summary         : An RTMP server implemented in C++
Name            : crtmpserver
Version         : dev
Release         : 1
License         : GPL
Vendor          : rtmpd.com
Packager        : Theo Aukerman
Group           : xxxx
URL             : http://www.rtmpd.com
Source          : %{name}-%{version}.tar.gz
BuildRoot       : %{_tmppath}/%{name}
Prefix          : %{pfx}

%Description
%{summary}

%Prep
%setup


%Build
#echo name=%{name}, version=%{version}, _tmppath=%{_tmppath}, pfx=%{pfx}, RPM_BUILD_ROOT=$RPM_BUILD_ROOT
cd builders/cmake
cmake -DCMAKE_TOOLCHAIN_FILE=ltib/toolchain-$CFGHOST.cmake .
make

%Install
rm -rf $RPM_BUILD_ROOT
cd builders/cmake
mkdir -p $RPM_BUILD_ROOT/%{pfx}/lib
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/applications/appselector
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/applications/flvplayback/
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/applications/flvplayback/mediaFolder
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/thelib
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/common
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/vm
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/tests
mkdir -p $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/tests/vmtests
cp rtmpserver/rtmpserver $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd
cp rtmpserver/rtmpserver.lua $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd
#These 3 libs need to be in /lib
cp thelib/libthelib.so $RPM_BUILD_ROOT/%{pfx}/lib
cp common/libcommon.so $RPM_BUILD_ROOT/%{pfx}/lib
cp vm/libvm.so $RPM_BUILD_ROOT/%{pfx}/lib
#Other libs need to be found from run directory which will be /usr/local/rtmpd
cp applications/appselector/libappselector.so $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/applications/appselector/
cp applications/flvplayback/libflvplayback.so $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/applications/flvplayback/
cp tests/tests $RPM_BUILD_ROOT/%{pfx}/usr/local/rtmpd/tests

%Clean
#rm -rf $RPM_BUILD_ROOT

%Files
%defattr(-,root,root)
%{pfx}/*
