//
//  NVWebDriverLibValFix.c
//  NVWebDriverLibValFix
//
//  Created by Oliver Kuckertz on 18.07.17.
//  Copyright © 2017 Oliver Kuckertz. All rights reserved.
//

// TODO make this use Lilu for kernel patching
// TODO don't assume that the first 16 bytes of csfg_get_platform_binary are complete and safe to relocate

#include <mach/mach_types.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <IOKit/IOLib.h> // for IOLog
#include <i386/proc_reg.h> // for get_cr0/set_cr0

#define dbgprintf(STR, ...) \
    IOLog("NVWebDriverLibValFix: " STR "\n", ## __VA_ARGS__);

kern_return_t NVWebDriverLibValFix_start(kmod_info_t * ki, void *d);
kern_return_t NVWebDriverLibValFix_stop(kmod_info_t *ki, void *d);

#define kTrampolinePtrOffset 6
static const uint8_t trampoline[14] = { 0xFF,0x25,0x00,0x00,0x00,0x00 /* JMP [RIP+0] */, 0 };

static const char* kNvidiaTeamId = "6KR3T733EC";

struct fileglob; // transparent

int csfg_get_platform_binary(struct fileglob *fg);
const char *csfg_get_teamid(struct fileglob *fg);

static
int cli(void)
{
    unsigned long flags;
    asm volatile ("pushf; pop %0; cli;" : "=r" (flags));
    return !!(flags & EFL_IF);
}

static
void sti(void)
{
    asm volatile ("sti; nop;");
}

size_t safePrefixLen = 16; // equally unsafe, should be determined at run-time
uint8_t real_csfg_get_platform_binary[16 + sizeof(trampoline)] __attribute__((section("__TEXT,__text")));

static
int my_csfg_get_platform_binary(struct fileglob *fg)
{
    if (((int(*)(struct fileglob *fg))real_csfg_get_platform_binary)(fg)) {
        return 1;
    }

    const char *teamId = csfg_get_teamid(fg);
    if (teamId && strcmp(teamId, kNvidiaTeamId) == 0) {
        //dbgprintf("platform binary override for %s", kNvidiaTeamId);
        return 1;
    }

    return 0;
}

kern_return_t NVWebDriverLibValFix_start(kmod_info_t * ki, void *d)
{
    dbgprintf("enabling patches");

    int intrflag = cli();
    uintptr_t cr0 = get_cr0();
    set_cr0(cr0 & ~CR0_WP);

    // assemble real_csfg_get_platform_binary
    // note that this is unsafe if the first 14 bytes of the target function contain a relative jump. ew.
    assert(safePrefixLen >= sizeof(trampoline));
    uint8_t* p = (uint8_t *)real_csfg_get_platform_binary;
    memcpy(p, (void *)csfg_get_platform_binary, safePrefixLen);
    p += safePrefixLen;
    memcpy(p, trampoline, sizeof(trampoline));
    *(uintptr_t *)(p + kTrampolinePtrOffset) = (uintptr_t)csfg_get_platform_binary + safePrefixLen;

    // replace csfg_get_platform_binary
    uint8_t tbuf[sizeof(trampoline)];
    memcpy(tbuf, trampoline, sizeof(trampoline));
    *(uintptr_t *)(tbuf + kTrampolinePtrOffset) = (uintptr_t)my_csfg_get_platform_binary;
    memcpy((void *)csfg_get_platform_binary, tbuf, sizeof(trampoline));

    set_cr0(cr0);
    if (intrflag)
        sti();

    return KERN_SUCCESS;
}

kern_return_t NVWebDriverLibValFix_stop(kmod_info_t *ki, void *d)
{
    dbgprintf("removing patches (unload)");

    int intrflag = cli();
    uintptr_t cr0 = get_cr0();
    set_cr0(cr0 & ~CR0_WP);

    memcpy((void *)csfg_get_platform_binary, (void *)real_csfg_get_platform_binary, sizeof(trampoline));

    set_cr0(cr0);
    if (intrflag)
        sti();

    return KERN_SUCCESS;
}
