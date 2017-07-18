#!/bin/sh
# Note that this script requires xcpretty and GNU tar.

set -e
set -o pipefail

cd "$(dirname $0)"

rm -rf build
xcodebuild -configuration Release build | xcpretty

KEXT=NVWebDriverLibValFix.kext
KEXTVERSION=$(/usr/libexec/PlistBuddy -c "Print :CFBundleShortVersionString" build/Release/$KEXT/Contents/Info.plist)

(set -e && cd build/Release/ && COPYFILE_DISABLE=1 gtar -c --owner=0 --group=0 -z -f $KEXT.$KEXTVERSION.tar.gz $KEXT)

echo "▸ Done, your archive file is under build/Release/"
