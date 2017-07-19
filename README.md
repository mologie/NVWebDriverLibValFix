NVWebDriverLibValFix.kext
=========================

This macOS kernel exstension modifies macOS to recognize NVIDIA's web drivers as platform binaries. This resolves the issue with transparent windows without content, which appear for applications that use Metal and have Library Validation enabled. Common affected applications are iBooks and Little Snitch Network Monitor, though this patch is universal and fixes them all.

In comparison to my previous solution to this problem, [DisableLibraryValidation.kext](https://github.com/mologie/macos-disable-library-validation), this fix is specifically tailored to NVIDIA's web driver and does *not* nuke any security features.

Installation
------------

The latest release and installation instructions  are available on the [Releases Page](https://github.com/mologie/NVWebDriverLibValFix/releases).

All releases are unsigned and require kernel code signing to be disabled (`csrutil enable --without kext`).

Installation (from source)
--------------------------

Install Xcode, then `./install.sh`.

On success, you should see a message from 'NVWebDriverLibValFix' in the output of `sudo dmesg` and find 'com.apple.kext.mologie.NVWebDriverLibValFix' in the output of `kextstat`.

The patches take effect immediately, but you should log out and back in to make sure that all processes benefit from them.

Removal
-------

`sudo ./install.sh uninstall`, which runs `kextunload` to unload the extension and deletes it from disk.

Bug Reports
-----------

This software comes with no support or implied warrenty. I welcome pull requests that fix bugs should you have found any. You may also submit pull requests for adding information to this readme file, or contact me privately via the email address on my GitHub profile.

Legal
-----

Copyright 2017 Oliver Kuckertz; released under the [MIT license](LICENSE).
