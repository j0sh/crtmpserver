Content:
1. What is this ?
2. How I can build package ?
3. How I can install/uninstall package ?
4. How I can build package using existing sources with my applications ?

1. What is this ?
================================================================================
  This is packing tool. It can create valid debian package for debian-based OS 
(Debian, Ubuntu and so on). As result, after building you got single file named
crtmpserver-<VERSION>_<ARCH>.deb. This file(package) can be easy installed. 


2. How I can build package ?
================================================================================
  To build package you need to run script "build_package.sh". When build 
complete copy debian package to safe place and run "cleanup.sh". Cleanup will
clean this directory without any question. So do not forget to copy package.


3. How I can install/uninstall package ?
================================================================================
  To install package you need system administrator privileges. Command is next:

	dpkg -i crtmpserver-<VERSION>_<ARCH>.deb

After this crtmpserver will be installed in yout system. To remove crtmpserver run next:

	dpkg -P crtmpserver

4. How I can build package using existing sources with my applications ?
================================================================================
  Add your application to variable "APP" in script "build_package.sh" and run it
When script ask you "Do you want to use exists sources?(y/n):" answer "y".
Do not forget add install rules to CMakeLists.txt and update server config.

