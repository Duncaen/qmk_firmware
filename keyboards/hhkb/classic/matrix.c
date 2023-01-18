/*
Copyright 2011 Jun Wako <wakojun@gmail.com>
Copyright 2020 Kan-Ru Chen <kanru@kanru.info>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "quantum.h"

#ifdef DEBUG_ADC
static bool debug_adc = false;
#endif

#ifdef DEBUG_ADC_MIN_MAX
static uint16_t calibration_min[MATRIX_COLS][MATRIX_ROWS] = {0};
static uint16_t calibration_max[MATRIX_COLS][MATRIX_ROWS] = {0};
#endif

int16_t adc_read(void) {
    while (ADC1->CR & ADC_CR_ADSTP)
        ;
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC))
        ;
    return ADC1->DR & 0b111111111111;
}

uint16_t actuation_point[MATRIX_COLS][MATRIX_ROWS] = {0};

#ifdef DEBUG_ADC_MIN_MAX
void matrix_init_custom(void) {
    memset(calibration_min, -1, sizeof(calibration_min));
    memset(calibration_max, 0, sizeof(calibration_max));
}
#endif

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;
    // Row drive: B0,B1,B2,B10
    static const uint16_t row_select[MATRIX_ROWS] = {0x1, 0x2, 0x4, 0x400};
    // Col select: B12,B13,B14
    static const uint32_t col_select[MATRIX_COLS] = {
        0x70000000,
        0x60001000,
        0x50002000,
        0x40003000,
        0x30004000,
        0x20005000,
        0x10006000,
        0x7000,
        0x70000000,
        0x60001000,
        0x50002000,
        0x40003000,
        0x30004000,
        0x20005000,
        0x10006000,
    };

    pal_lld_setport(GPIOB, 0x407);

    // power switch board
    writePinHigh(C14);
    /* writePinLow(D2); */

    // enable OpAmp
    writePinHigh(C7);

    writePinHigh(C6); // U1
    writePinLow(B15); // U2

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        if (col == 8) {
            writePinLow(C6);   // U1
            writePinHigh(B15); // U2
        }
        GPIOB->BSRR.W = col_select[col];
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            matrix_row_t last_row_value = current_matrix[row];


            wait_us(2);
            // Discharge capacitor
            writePinHigh(C8);
            wait_us(2);
            writePinLow(C8);

            pal_lld_clearport(GPIOB, row_select[row]);
            wait_us(2);

            uint16_t val = adc_read();
#ifdef DEBUG_ADC
            if (debug_adc)
                uprintf("col: %2u, row: %2u, val: %4u\n", row, col, val);
#endif
            /* val = 0; */
            if (val > actuation_point[col][row]) {
                current_matrix[row] |= (1 << col);
            } else {
                current_matrix[row] &= ~(1 << col);
            }
#ifdef DEBUG_ADC_MIN_MAX
            if (val < calibration_min[col][row]) {
                calibration_min[col][row] = val;
            } else if (val > calibration_max[col][row]) {
                calibration_max[col][row] = val;
            }
#endif
            if (current_matrix[row] ^ last_row_value) {
                matrix_has_changed   = true;
            }

            pal_lld_setport(GPIOB, row_select[row]);
        }
    }
    // Disable OpAmp
    writePinLow(C7);

    return matrix_has_changed;
}

#ifdef DEBUG_ADC
bool command_extra(uint8_t code) {
    switch (code) {
        case KC_A:
            debug_adc = !debug_adc;
            return true;
        case KC_Q:
            uprintf("actuation_point:");
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uprintf("\n  ");
                for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                    uprintf("%04u ", actuation_point[col][row]);
                }
            }
            uprintf("\n");
#ifdef DEBUG_ADC_MIN_MAX
            uprintf("calibration_min:");
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uprintf("\n  ");
                for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                    uprintf("%04u ", calibration_min[col][row]);
                }
            }
            uprintf("\n");
            uprintf("calibration_max:");
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uprintf("\n  ");
                for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                    uprintf("%04u ", calibration_max[col][row]);
                }
            }
            uprintf("\n");
#endif
            return true;
        default:
            return false;
    }
}
#endif
