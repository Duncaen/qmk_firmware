// SPDX-License-Identifier: MIT
#include "bootloader.h"
#include "ch.h"
#include "hal.h"

void bootloader_jump(void) {
    RTC->BKP0R = 0x544F4F42UL;
    NVIC_SystemReset();
}

void mcu_reset(void) {
    NVIC_SystemReset();
}

void enter_bootloader_mode_if_requested(void) {}
