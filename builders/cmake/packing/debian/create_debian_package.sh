#!/bin/sh

#create the distribution
ORIG=`pwd`
cd ../
sh linux.sh
cd "$ORIG"

#prepare the debian folder
ROOT=/tmp/crtmpserver
rm -rf $ROOT*
mkdir -p $ROOT/DEBIAN
mkdir -p $ROOT/opt
mkdir -p $ROOT/etc/init.d
cp -R /tmp/rtmpserver $ROOT/opt/crtmpserver
cp crtmpserver $ROOT/etc/init.d/
echo "Version: 0.`svnversion -n ../../../../sources/`" >$ROOT/DEBIAN/control
echo "Architecture: `dpkg-architecture|grep DEB_HOST_ARCH=|cut -d "=" -f2`" >>$ROOT/DEBIAN/control
cat control >>$ROOT/DEBIAN/control
cp postinst $ROOT/DEBIAN/
cp postrm $ROOT/DEBIAN/
cp prerm $ROOT/DEBIAN/
dpkg --build /tmp/crtmpserver /tmp/

