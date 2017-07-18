//
//  NVWebDriverLibValFix.cpp
//  NVWebDriverLibValFix
//
//  Created by Oliver Kuckertz on 18.07.17.
//  Copyright © 2017 Oliver Kuckertz. All rights reserved.
//

#include <IOKit/IOLib.h>
#include "NVWebDriverLibValFix.h"

OSDefineMetaClassAndStructors(com_mologie_NVWebDriverLibValFix, IOService)

// This service does not implement any methods. Its sole purpose is to have IOKit load our driver on boot.
