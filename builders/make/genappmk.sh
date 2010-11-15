>apps.mk

FINALTARGET="applications: thelib"
ALL_OBJS="ALL_APPS_OBJS="
ACTIVE_APPS="ACTIVE_APPS="

for i in `cat apps|grep -v "#"`
do
	LC=`echo $i|tr "[:upper:]" "[:lower:]"`
	UC=`echo $i|tr "[:lower:]" "[:upper:]"`
	cat template|sed "s/__appname__/$LC/g"|sed "s/__APPNAME__/$UC/g" >>apps.mk
	FINALTARGET="$FINALTARGET $i"
	ALL_OBJS="$ALL_OBJS \$($UC""_OBJS)"
	ACTIVE_APPS="$ACTIVE_APPS -DHAS_APP_$UC"
done



echo $ALL_OBJS >> apps.mk
echo $ACTIVE_APPS >> apps.mk
echo $FINALTARGET >>apps.mk


