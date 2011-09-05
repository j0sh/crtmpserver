#!/bin/sh

NUMBER=`tail -100 /Volumes/ramdisk/crtmpserver.log |grep "basertmpprotocol.cpp:717"|tail -1|sed "s/.*ProcessBytes.\([0-9]*\).*/\1/"`
cat /Volumes/ramdisk/crtmpserver.log |grep "ProcessBytes:$NUMBER"|sed "s/.*input: \(.*\)/\1/"|tail -100

NUMBER=`tail -100 /Volumes/ramdisk/crtmpserver.log|grep "URSI"|grep "Write" |tail -1|cut -d ";" -f9|cut -d ":" -f2|sed "s/[ ]*\([0-9]*\).*/\1/"`
cat /Volumes/ramdisk/crtmpserver.log |grep "URSI: $NUMBER"|grep -v "AllowExecution"|tr "\n" " "|tr ")" "\n"|sed "s/.*length: \(.*\)/\1/"|sed "s/\([0-9]*\) .*(\(.*\)/\1 (\2/"|sed "s/^\([0-9]\) \(.*\)/  \1 \2/"|sed "s/^\([0-9][0-9]\) \(.*\)/ \1 \2/"

