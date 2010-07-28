help: 
	@echo Example environment setup:
	@echo '	export ANDROID_SDK_ROOT=/Users/gz/projects/android/android-sdk-mac_86'
	@echo '	export ANDROID_NDK_ROOT=/Users/gz/projects/android/android-sdk-mac_86/android-ndk-r4-crystax'
	@echo '	export ANDROID_NDK_PLATFORM=darwin-x86'
	@echo 'Add ANDROID_SDK_ROOT/tools and ANDROID_NDK_ROOT to your PATH'
	@echo
	@echo Usage: make { all, install, uninstall, clean }

all:
	ndk-build
	ant debug

install: all
	ant install

uninstall: 
	ant uninstall

clean:
	ant clean
        # remove ndk libs
	rm -rf libs
