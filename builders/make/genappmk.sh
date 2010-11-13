>apps.mk

FINALTARGET="applications: thelib"
ALL_OBJS="ALL_APPS_OBJS="

for i in `cat apps|grep -v "#"`
do
	LC=`echo $i|tr "[:upper:]" "[:lower:]"`
	UC=`echo $i|tr "[:lower:]" "[:upper:]"`
	cat template|sed "s/__appname__/$LC/g"|sed "s/__APPNAME__/$UC/g" >>apps.mk
	FINALTARGET="$FINALTARGET $i"
	ALL_OBJS="$ALL_OBJS \$($UC""_OBJS)"
done



echo $ALL_OBJS >> apps.mk
echo $FINALTARGET >>apps.mk


