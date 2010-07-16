cp ./rtmpserver.lua $3
cp ./rtmpserver.lua $2
for i in `find $3/applications -mindepth 1 -maxdepth 1 -type d`;do mkdir -p $i/mediaFolder;done
tar -pcf ./rtmpd-android.tar.gz $1
tar -pcf ./rtmpd-android-dynamic.tar.gz $3
tar -pcf ./rtmpd-android-static.tar.gz $2

