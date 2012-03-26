HAS_PCH=`xcodebuild -version|grep Xcode|cut -d "." -f2`
if test $HAS_PCH -ge 3
then
	echo -n 1
else
	echo -n 0
fi

