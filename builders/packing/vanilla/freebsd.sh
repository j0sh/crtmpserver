#!/bin/sh

#define platform variables
SRC_DIR=../../cmake
DST_DIR=/tmp/rtmpserver
LIBPREFIX=lib
LIBSUFFIX=.so
TMP_DIR=$DST_DIR/tmp
STANDARD_LIBRARIES=$TMP_DIR/libraries
APPLICATIONS_LIBRARIES=$TMP_DIR/applications
EXECUTABLES=$TMP_DIR/executables
CONFIG_FILES=$TMP_DIR/configfiles
CHANGES_SCRIPT=$TMP_DIR/changes

#abort if the dest dir exists
if [ -d "$DST_DIR" ]
then
	echo "Directory \`$DST_DIR\` exists. Abort";
	exit;
fi

#re-link everything withoutput directory
PREVDIR=`pwd`
cd $SRC_DIR
sh cleanup.sh
cmake -D MY_OUTPUT_DIRECTORY=$DST_DIR .
make
cd $PREVDIR


#create the temp directory where we are going to put
#intermediate files. It will be deleted as last step
mkdir -p $TMP_DIR

#create the list of libraries needed by the server
for i in `find $SRC_DIR -mindepth 1 -maxdepth 1 -type d|sed "s/^\.\.\/\.\.\/cmake\///"`
do
	if [ ! -f "$SRC_DIR/$i/$LIBPREFIX$i$LIBSUFFIX" ]
	then
		continue;
	fi
	echo $i >> $STANDARD_LIBRARIES
done


#create the list of available applications
for i in `find $SRC_DIR/applications -mindepth 1 -maxdepth 1 -type d|sed "s/^\.\.\/\.\.\/cmake\/applications\///"`
do
	if [ ! -f "$SRC_DIR/applications/$i/$LIBPREFIX$i$LIBSUFFIX" ]
	then
		continue
	fi
	echo $i >> $APPLICATIONS_LIBRARIES
done

#create the list of executables
echo "rtmpserver" >> $EXECUTABLES
echo "tests" >> $EXECUTABLES
echo "vmtests" >>$EXECUTABLES

#create the list of config files
echo "$SRC_DIR/rtmpserver/rtmpserver.lua" >>$CONFIG_FILES
echo "$SRC_DIR/vmtests/vmtests.lua" >>$CONFIG_FILES
echo "$SRC_DIR/vmtests/utils.lua" >>$CONFIG_FILES


#create the needed directories
mkdir -p $DST_DIR
for i in `cat $APPLICATIONS_LIBRARIES`
do
	mkdir -p $DST_DIR/applications/$i/mediaFolder
done

#copy the libraries and change permissions
for i in `cat $STANDARD_LIBRARIES`
do
	cp $SRC_DIR/$i/$LIBPREFIX$i$LIBSUFFIX $DST_DIR/
	chmod 644 $DST_DIR/$LIBPREFIX$i$LIBSUFFIX
done

#copy the executables and change permissions
for i in `cat $EXECUTABLES`
do
	cp $SRC_DIR/$i/$i $DST_DIR/
	chmod 755 $DST_DIR/$i
done

#copy the config files and change permissions
for i in `cat $CONFIG_FILES`
do
	cp $i $DST_DIR/
done

#copy the applications and change permissions
for i in `cat $APPLICATIONS_LIBRARIES`
do
	cp $SRC_DIR/applications/$i/$LIBPREFIX$i$LIBSUFFIX $DST_DIR/applications/$i/
	chmod 644 $DST_DIR/applications/$i/$LIBPREFIX$i$LIBSUFFIX
done

#remove temp directory
rm -rf $TMP_DIR

#add the startup script
echo "#!/bin/sh" >>$DST_DIR/run.sh
echo "./rtmpserver ./rtmpserver.lua" >>$DST_DIR/run.sh
echo "" >>$DST_DIR/run.sh
chmod 755 $DST_DIR/run.sh

echo "Package created in \"$DST_DIR\""

