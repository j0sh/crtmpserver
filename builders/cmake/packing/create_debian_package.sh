#!/bin/sh

#create the distribution
#sh linux.sh

#prepare the debian folder
ROOT=/tmp/crtmpserver
rm -rf $ROOT
mkdir -p $ROOT/DEBIAN
mkdir -p $ROOT/opt
cp -R /tmp/rtmpserver $ROOT/opt/crtmpserver
echo "Version: 0.`svnversion -n ../../../sources/`" >$ROOT/DEBIAN/control
echo "Architecture: `dpkg-architecture|grep DEB_HOST_ARCH=|cut -d "=" -f2`" >>$ROOT/DEBIAN/control
cat control >>$ROOT/DEBIAN/control
dpkg --build /tmp/crtmpserver /tmp/

