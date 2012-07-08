#!/bin/bash

rm -rf /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION*
mkdir -p /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/media
mkdir -p /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/configs
mkdir -p /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/logs
mkdir -p /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/ssl
cp crtmpserver/crtmpserver /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/
cp tests/tests /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/
strip -x crtmpserver/crtmpserver /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/crtmpserver
strip -x /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/tests
cp ../../media/README.txt /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/media/
cp ../../configs/*.lua /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/configs/
cp ../../LICENSE /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/
cp ../../configs/*.sh /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/
cp applications/appselector/server.* /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION/ssl/
(cd /tmp && tar czfv crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION.tar.gz crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION)
mv /tmp/crtmpserver-trunk-$OS_ARCH-$OS_NAME-$OS_VERSION.tar.gz ./

