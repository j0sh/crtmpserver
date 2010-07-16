OUTPUT=/sdcard/output
DEVICE=-d

adb $DEVICE shell mkdir $OUTPUT
adb $DEVICE shell mkdir $OUTPUT/applications

adb $DEVICE shell mkdir $OUTPUT/applications/appselector
adb $DEVICE shell mkdir $OUTPUT/applications/appselector/mediaFolder

adb $DEVICE shell mkdir $OUTPUT/applications/flvplayback
adb $DEVICE shell mkdir $OUTPUT/applications/flvplayback/mediaFolder

adb $DEVICE shell mkdir $OUTPUT/applications/proxypublish
adb $DEVICE shell mkdir $OUTPUT/applications/proxypublish/mediaFolder

adb $DEVICE push output/dynamic/liblua.so $OUTPUT
adb $DEVICE push output/dynamic/libcommon.so $OUTPUT
adb $DEVICE push output/dynamic/libthelib.so $OUTPUT
adb $DEVICE push output/dynamic/tests $OUTPUT
adb $DEVICE push output/dynamic/rtmpserver $OUTPUT
adb $DEVICE push rtmpserver.lua $OUTPUT
adb $DEVICE push output/dynamic/applications/appselector/libappselector.so $OUTPUT/applications/appselector
adb $DEVICE push output/dynamic/applications/flvplayback/libflvplayback.so $OUTPUT/applications/flvplayback
adb $DEVICE push output/dynamic/applications/proxypublish/libproxypublish.so $OUTPUT/applications/proxypublish


#adb $DEVICE push ./rtmpd-uclinux-dynamic.tar.gz /data/

