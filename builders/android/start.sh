rm -rf output
tar xvf rtmpd-android-dynamic.tar.gz
rm -rf output/dynamic/applications/flvplayback/mediaFolder
ln -s /sdcard output/dynamic/applications/flvplayback/mediaFolder
rm /sdcard/*.meta
rm /sdcard/*.seek
cd output/dynamic
./rtmpserver ./rtmpserver.lua
cd /data
ls

