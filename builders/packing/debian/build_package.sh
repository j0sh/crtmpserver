#!/bin/sh

SVNPATH="https://svn.rtmpd.com/crtmpserver/trunk"
ORIGPATH="crtmpserver-original"
DEBPATH="crtmpserver-`date +%Y%m%d`"
LOGFILE="install.log"
APPS="admin applestreamingclient appselector flvplayback proxypublish samplefactory stresstest vptests"
PATCHDIR="patches"
PATCHLIST="list"
STARTPWD=`pwd`

test_package() {
	RES=`dpkg-query -W -f='\${Status}' $1 2>/dev/null`
	R=$?
	if [ $R -eq 1 ] 
	then 
		echo "Package $1 is required but not installed"
		echo "You can install it using command like"
		echo "sudo apt-get install $1"
		exit 1
	fi
	if [ `expr "$RES" : "^install"` -eq 0 ]
	then
		echo "Package $1 is required but not installed"
		echo "You can install it using command like"
		echo "sudo apt-get install $1"
		echo "***********************"
		exit 1
	fi
}

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

	cp $ORIGPATH/builders/cmake/$1/* $DEBPATH/$1 2>/dev/null
	if [ -d "$ORIGPATH/sources/$1/include" ]; then 
		mkdir -p $DEBPATH/$1/include
		cp -r $ORIGPATH/sources/$1/include/* $DEBPATH/$1/include  2>/dev/null
	fi
	if [ -d "$ORIGPATH/sources/$1/src" ]; then
		mkdir -p $DEBPATH/$1/src
		cp -r $ORIGPATH/sources/$1/src/* $DEBPATH/$1/src  2>/dev/null
	fi
}

echo "Check for dependencies"
test_package "g++"
test_package "gcc"
test_package "cmake"
test_package "dh-make"
test_package "libssl-dev"
test_package "fakeroot"
test_package "dpkg-dev"
test_package "subversion"

read -p "Do you want to use exists sources?(y/n): " answer
if [ "$answer" = "y" ]
then
	if [ -d ../../../sources -a -d ../../cmake ]
	then
		echo "$ORIGPATH change to ../../../"
		ORIGPATH="../../.."
	else
		read -p "Please specify path to source tree: " path
		if [ -d $path/sources -a -d $path/builders/cmake ]
		then
			ORIGPATH=$path
		else
			echo "Wrong path"
			echo "Exiting...."
			exit 1
		fi
	fi
	SVER="0.`svnversion -n ${ORIGPATH}/sources | tr -d 'M|S|P'`"
else
	echo "Fetch current sources from svn"
	svn co --username "anonymous" --password "" $SVNPATH $ORIGPATH 
	result=$?
	if [ $result != 0 ] 
	then
		echo "fail to fetch sources"
		exit $result
	fi
	SVER="0.`svnversion -n ${ORIGPATH}/sources | tr -d 'M|S|P'`"
#	find $ORIGPATH -name '.svn' -type d -exec rm -rf {} \;  2>/dev/null
fi

DEBPATH="`pwd`/crtmpserver-${SVER}"

echo "Build debian structures"
if [ -d $DEBPATH ]
then
	read -p "Debian build directory $DEBPATH already exists. Remove?(y/n): " answer
	if [ $answer="y" ]
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

############# Common
echo "Processing common"
copyProject "common"

############# Thelib
echo "Processing thelib"
copyProject "thelib"

############# VM
#echo "Processing vm"
#copyProject "vm"

############# rtmpserver
echo "Processing crtmpserver"
copyProject "crtmpserver"
#cp $ORIGPATH/builders/cmake/crtmpserver/crtmpserver.lua $DEBPATH/crtmpserver

############ applications ##################
echo "Processing applications"
mkdir -p $DEBPATH/applications
cp $ORIGPATH/builders/cmake/applications/CMakeLists.txt $DEBPATH/applications
for app in $APPS
do
	echo " ...$app"
	copyProject "applications/$app"
done

############ Change paths
echo "Fix paths"
find ${DEBPATH} -name CMakeLists.txt -type f -exec sed -r -i -f fix_paths.sed {} \;

############ Prepare debian sources
echo "************ Hit <ENTER> here *****************"
find $DEBPATH -name '.svn' -type d -exec rm -rf {} \; > /dev/null 2>&1
cd $DEBPATH
dh_make -c gpl3 -e jet@jet.kiev.ua -s -p crtmpserver --createorig
cd ../
if [ ! -f "crtmpserver_${SVER}.orig.tar.gz" ] 
then
	tar -czpf crtmpserver_${SVER}.orig.tar.gz crtmpserver-${SVER}.orig/*
fi
cd $DEBPATH/debian
rm -f *.ex control copyright README.Debian README.source crtmpserver.doc-base.EX docs
cd $STARTPWD 
cp -vrf debian/* $DEBPATH/debian
cp -vf $ORIGPATH/man/crtmpserver.1 $DEBPATH/debian

############ Build debian package
dpkg-source -b $DEBPATH
cd $DEBPATH
export SVER DEBPATH
dpkg-buildpackage -rfakeroot -us -uc -b
R=$?
if [ $R -ne 0 ] 
then 
	echo "Build failed"
	exit $R
fi

echo
echo "********************************************************************"
echo "All done!!!"
echo "You can easy install crtmpserver via 'sudo dpkg -i crtmpserver_${SVER}-1_`dpkg-architecture -qDEB_BUILD_ARCH_CPU | tr -d '\n'`.deb'"
echo "After this you can run crtmpserver directly or via init script"
echo
echo "All errors and wishes please sent me to e-mail: jet@jet.kiev.ua or"
echo "report via Google group(http://groups.google.com/group/c-rtmp-server/topics)"
echo "********************************************************************"

exit 0

