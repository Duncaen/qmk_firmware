// SPDX-License-Identifier: MIT

#pragma once

/* Bootloader */
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP TRUE

/* key matrix */
#define MATRIX_ROWS 4
#define MATRIX_COLS 15

#define DIP_SWITCH_PINS { A3, A4, A5, A6, A7, C4 }

#define LED_CAPS_LOCK_PIN A8

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#define ADC_RESOLUTION ADC_CFGR1_RES_12BIT

/* #define DEBUG_MATRIX_SCAN_RATE */

#define WAIT_US_TIMER           GPTD3

/* #define DEBUG_ADC */
