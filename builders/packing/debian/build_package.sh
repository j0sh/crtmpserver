#!/bin/sh

SVNPATH="https://rtmpd.com/crtmpserver/trunk"
ORIGPATH="rtmpd-original"
DEBPATH="rtmpd-`date +%Y%m%d`"
LOGFILE="install.log"
APPS="admin applestreamingclient appselector flvplayback proxypublish samplefactory stresstest vptests"
PATCHDIR="patches"
PATCHLIST="list"
STARTPWD=`pwd`

copyProject() {
	if [ -z "$1" ]
	then
		echo "parameters must be not empty"
		exit 1
	fi

	if [ ! -d "$DEBPATH/$1" ]
	then
#		echo "Creating dir $1"
		mkdir -p "$DEBPATH/$1"
	fi

	cp $ORIGPATH/builders/cmake/$1/CMakeLists.txt $DEBPATH/$1
	cp -r $ORIGPATH/sources/$1/* $DEBPATH/$1
}

echo "Fetch current sources from svn"
svn co --username "anonymous" --password "" $SVNPATH $ORIGPATH 2>&1 >$LOGFILE
result=$?
if [ $result != 0 ] 
then
	echo "fail to fetch sources"
	exit $result
fi

DEBPATH="rtmpd-0.`svnversion -n ${ORIGPATH}/sources`"

echo "Build debian structures"
if [ -d $DEBPATH ]
then
	read -p "Debian build directory $DEBPATH already exists. Remove?(y/n): "
	if [ $REPLY == "y" ]
	then
		rm -rf $DEBPATH
	else
		echo "Exiting...."
		exit 0
	fi
fi

############# Main CMakeLists.txt
mkdir -p $DEBPATH
cp $ORIGPATH/builders/cmake/CMakeLists.txt $DEBPATH

############# constants
mkdir -p $DEBPATH/constants
cp $ORIGPATH/constants/* $DEBPATH/constants

############# cmake_find_modules
echo "Processing cmake_find_modules"
mkdir -p "$DEBPATH/cmake_find_modules"
cp $ORIGPATH/builders/cmake/cmake_find_modules/* "$DEBPATH/cmake_find_modules"

############# LUA
echo "Processing lua"
mkdir -p "$DEBPATH/lua"
cp $ORIGPATH/builders/cmake/lua/CMakeLists.txt $DEBPATH/lua
mkdir -p "$DEBPATH/lua/src"
cp $ORIGPATH/3rdparty/lua-dev/* $DEBPATH/lua/src

############# Common
echo "Processing common"
copyProject "common"

############# Thelib
echo "Processing thelib"
copyProject "thelib"

############# rtmpserver
echo "Processing rtmpserver"
copyProject "rtmpserver"
cp $ORIGPATH/builders/cmake/rtmpserver/rtmpserver.lua $DEBPATH/rtmpserver

############ applications ##################
echo "Processing applications"
mkdir -p $DEBPATH/applications
cp $ORIGPATH/builders/cmake/applications/CMakeLists.txt $DEBPATH/applications
for app in $APPS
do
	echo " ...$app"
	copyProject "applications/$app"
done

############ Patching
echo "Patching files"
while read pfile fpatch
do
	echo -n " ...$pfile"
	patch "$DEBPATH/$pfile" "$PATCHDIR/$fpatch" > /dev/null
	echo "...done"
done < $PATCHDIR/$PATCHLIST

############ Prepare debian sources
echo "************ Hit <ENTER> here *****************"
cd $DEBPATH
dh_make -c gpl3 -e jet@jet.kiev.ua -s -p rtmpd --createorig
cd debian
rm -f *.ex control copyright README.Debian README.source rtmpd.doc-base.EX
cd $STARTPWD 
cp -vf debian/* $DEBPATH/debian

############ Build debina package
cd $DEBPATH
dpkg-buildpackage -rfakeroot -us -uc

echo " All done"
echo "If no errors displayed you can easy install rtmpd via 'sudo dpkg -i *.deb'"
echo "After this you can run rtmpd directly or via init script(not complete yet)"
echo
echo "All errors and wishes please sent me to e-mail: jet@jet.kiev.ua or"
echo "report via Google group(http://groups.google.com/group/c-rtmp-server/topics)"

exit 0

