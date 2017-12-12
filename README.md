NVWebDriverLibValFix.kext
=========================

This macOS kernel exstension modifies macOS Sierra to recognize NVIDIA's web drivers as platform binaries. This resolves the issue with transparent windows without content, which appear for applications that use Metal and have Library Validation enabled. Common affected applications are iBooks and Little Snitch Network Monitor, though this patch is universal and fixes them all.

**Users of macOS 10.13 High Sierra no longer need this patch,** because the underlying issue that it tackled was solved. You can uninstall this extension before or after upgrading, it won't break things. 🎉

Installation
------------

The latest release and installation instructions  are available on the [Releases Page](https://github.com/mologie/NVWebDriverLibValFix/releases).

All releases are unsigned and require kernel code signing to be disabled. Hackintoshs tend to have kernel code signing disabled by default. On Apple Macs, follow Apple's [guide to using csrutil](https://developer.apple.com/library/content/documentation/Security/Conceptual/System_Integrity_Protection_Guide/ConfiguringSystemIntegrityProtection/ConfiguringSystemIntegrityProtection.html), but enter the command `csrutil enable --without kext` instead.

Installation (from source)
--------------------------

It is recommended that you install from the 'Releases' page like described in the previous section. Install from source only if you wish to make modifications to this driver.

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
