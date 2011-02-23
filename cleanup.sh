#directories=( CMakeFiles build dist bin obj *.app _ReSharper.* *.out Debug Release output bin-debug bin-release *.gch )

#files=( cmake_install.cmake Makefile CMakeCache.txt *.dylib *.a *.log *.core .DS_Store .userprefs \
# .usertasks .pidb *.exe *.suo *.user *.bak *.mdp .dep.inc *~ *.ncb *.gmon *.orig \
# rtmpserver cppapplicationserver rtmpserver mcbtests trafficdisector vmtests pcapdumper )

#for name in ${directories[@]}; do
#	for i in `find  . -type d -name "$name"`;do  rm -rf $i;done
#done

#for name in ${files[@]}; do
#	for i in `find  . -type f -name "$name"`;do  rm -rf  $i;done
#done



for i in `find  ./ -type d -name "CMakeFiles"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "build"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "dist"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "bin"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "obj"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "*.app"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "_ReSharper.*"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "*.out"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "Debug"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "Release"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "output"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "bin-debug"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "bin-release"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "html-template"`;do  rm -rf $i;done
for i in `find  ./ -type d -name "*.gch"`;do rm -rf $i;done
for i in `find  ./ -type d -name "_CPack_Packages"`;do rm -rf $i;done
for i in `find  ./ -type f -name "cmake_install.cmake"`;do  rm -rf $i;done
for i in `find  ./ -type f -name "Makefile"`;do  rm -rf $i;done
for i in `find  ./ -type f -name "CMakeCache.txt"`;do  rm -rf $i;done
for i in `find  ./ -type f -name "*.dylib"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.so"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.a"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.core"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name ".DS_Store"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.userprefs"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.usertasks"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.pidb"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.exe"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.suo"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.user"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.bak"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.mdp"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name ".dep.inc"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*~"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.ncb"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.gmon"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.orig"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.rej"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.dmg"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "install_manifest.txt"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "CPackConfig.cmake"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "CPackSourceConfig.cmake"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "rtmpserver"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "cppapplicationserver"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "rtmpserver"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "mcbtests"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "trafficdissector"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "vmtests"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "pcapdumper"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "tssplitter"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.cxx"|grep  "/tests/"`;do  rm -rf $i;done
for i in `find  ./ -type f -name "*.bin"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.ii"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.s"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.o"`;do  rm -rf  $i;done
for i in `find  ./ -type f -name "*.deb"`;do  rm -rf  $i;done

