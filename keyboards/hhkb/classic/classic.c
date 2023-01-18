// SPDX-License-Identifier: MIT
#include "classic.h"

extern uint16_t actuation_point[MATRIX_COLS][MATRIX_ROWS];

void keyboard_pre_init_kb(void) {
    /* debug_enable=true; */
    setPinInput(C4);
    setPinOutputPushPull(C6);
    setPinOutputPushPull(C7);
    setPinOutputPushPull(C8);
    setPinOutputPushPull(C9);
    setPinOutputPushPull(C14);
    setPinInput(A3);
    setPinInput(A4);
    setPinInput(A5);
    setPinInput(A6);
    setPinInput(A7);
    setPinInput(A10);
    setPinOutputPushPull(B0);
    setPinOutputPushPull(B1);
    setPinOutputPushPull(B2);
    setPinOutputPushPull(B6);
    setPinOutputPushPull(B10);
    setPinOutputPushPull(B11);
    setPinOutputPushPull(B12);
    setPinOutputPushPull(B13);
    setPinOutputPushPull(B14);
    setPinOutputPushPull(B15);
    setPinOutputPushPull(A8);
    setPinOutputPushPull(A9);
    setPinOutputPushPull(D2);
    setPinInput(B7);
    setPinInput(B8);
    setPinInput(B9);
    setPinOutputPushPull(B3);
    setPinOutputPushPull(B4);
    setPinOutputPushPull(B5);
    setPinOutputPushPull(B6);
    setPinOutputPushPull(A15);

    // leds
    writePinLow(A8);
    writePinLow(A9);

    // bluetooth board?
    writePinHigh(A15);
    writePinHigh(B3);
    writePinHigh(B4);
    writePinHigh(B5);
    writePinHigh(B6);
    //
    writePinHigh(C14);
    writePinHigh(C7);
    writePinHigh(B6);

    palSetLineMode(A1, PAL_MODE_INPUT_ANALOG);
    /* rccResetADC1(); */
    /* rccEnableADC1(true); */

    rccEnableADC1(true);

    if (READ_BIT(ADC1->ISR, ADC_ISR_ADRDY))
        SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);
    if (READ_BIT(ADC1->CR, ADC_CR_ADEN))
        SET_BIT(ADC1->CR, ADC_CR_ADDIS);

    while (READ_BIT(ADC1->CR, ADC_CR_ADEN))
        ;
    MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, 0);
    MODIFY_REG(ADC1->CR, ADC_CR_ADVREGEN, ADC_CR_ADVREGEN);
    wait_ms(2);

    /* SET_BIT(ADC->CCR, ADC_CCR_LFMEN); */

    /* MODIFY_REG(ADC1->CFGR2, ADC_CFGR2_CKMODE, ADC_CFGR2_CKMODE_0); */
    MODIFY_REG(ADC1->CFGR2, ADC_CFGR2_CKMODE, ADC_CFGR2_CKMODE_0|ADC_CFGR2_CKMODE_1);

    SET_BIT(ADC1->CR, ADC_CR_ADCAL);
    while (READ_BIT(ADC1->CR, ADC_CR_ADCAL))
        ;

    SET_BIT(ADC1->ISR, ADC_ISR_ADRDY);
    SET_BIT(ADC1->CR, ADC_CR_ADEN);
    while (!READ_BIT(ADC1->ISR, ADC_ISR_ADRDY))
        ;

    MODIFY_REG(ADC1->CFGR1, ADC_CFGR1_EXTEN, 0);

    CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_CONT);

    MODIFY_REG(ADC1->SMPR, ADC_SMPR_SMP, 0);

    ADC1->CHSELR = 1UL<<1;

    if (actuation_point[0][0] == 0) {
        const uint16_t *eeprom_calibration0 = (void *)0x08081100;
        const uint16_t *eeprom_calibration1 = (void *)0x08081200;
        for (uint8_t i = 0; i < 0x80; i++) {
            uint8_t col = i / 8;
            uint8_t row = i % 8;
            if (row < MATRIX_ROWS && col < MATRIX_COLS)
                actuation_point[col][row] = eeprom_calibration0[i] + eeprom_calibration1[i];
        }
    }
    if (actuation_point[0][0] == 0) {
        // fallback if eeprom didn't contain the calibration
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
                actuation_point[col][row] = 1375;
            }
        }
    }

    keyboard_pre_init_user();
}
