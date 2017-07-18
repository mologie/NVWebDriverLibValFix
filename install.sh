#!/bin/sh
set -e

KEXT=NVWebDriverLibValFix.kext
KEXTDPREFIX=

cd "$(dirname $0)"

case "$1" in
"")
	if [ "$(id -u)" -eq 0 ]; then
		echo "Please don't run me as root. I want to build as normal user."
		exit 1
	fi
	rm -rf build
	xcodebuild -configuration Release build
	echo "Done building. I'll now ask for your user password to install the kernel extension."
	sudo $0 install
	;;
install)
	if [ ! "$(id -u)" -eq 0 ]; then
		echo "The install action must be run as root."
		exit 1
	fi
	ditto build/Release/$KEXT /tmp/$KEXT
	chown -R root:wheel /tmp/$KEXT
	$0 uninstall
	ditto /tmp/$KEXT $KEXTDPREFIX/Library/Extensions/$KEXT
	rm -rf /tmp/$KEXT
	kextutil $KEXTDPREFIX/Library/Extensions/$KEXT
	echo "\033[0;32mSuccess! $KEXT has been built, installed and loaded.\033[0m"
	;;
uninstall)
	if [ ! "$(id -u)" -eq 0 ]; then
		echo "The uninstall action must be run as root."
		exit 1
	fi
	if [ -d $KEXTDPREFIX/Library/Extensions/$KEXT ]; then
		kextunload $KEXTDPREFIX/Library/Extensions/$KEXT || (
			echo "Couldn't unload kernel extension; I'll assume that it's already gone."
		)
		rm -rf $KEXTDPREFIX/Library/Extensions/$KEXT
		echo "Previous version of $KEXT has been removed."
	else
		echo "$KEXT has not yet been installed."
	fi
	;;
*)
	echo "Unknown action $1"
	exit 1
	;;
esac
