///////////////////////////////////////////////
// File: joystick.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include <input.h>
#include "common.h"
#include "portbits.h"

PUBLIC struct RawInput input = {
    .pb1 = false,
    .pb2 = false,
    .thumbh = 0,
    .thumbv = 0,
};

PUBLIC void GPIOD_ISR(void)
//
// ISR called on falling or rising edge for pushbutton A or pushbutton B.
{
    if (PB_PORT->MIS & PB1_PIN)
    {
        // Read input
        input.pb1 = (PB_PORT->DATA & PB1_PIN) == 0;
        // Clear interrupt
        PB_PORT->ICR = PB1_PIN;
    }

    if (PB_PORT->MIS & PB2_PIN)
    {
        // Read input
        input.pb2 = (PB_PORT->DATA & PB2_PIN) == 0;
        // Clear interrupt
        PB_PORT->ICR = PB2_PIN;
    }
}

PUBLIC void inputTick(void)
//
// Called by systick.
{
    ADC0->PSSI = ADC_PSSI_SS2; // Sample
}


PUBLIC void ADC0_ISR(void)
//
// called under interrupt.
{
    input.thumbh = ADC0->SSFIFO2;        // Retrieve x position.
    input.thumbv = ADC0->SSFIFO2;        // Retrieve y position.

    ADC0->ISC = ADC_ISC_IN2;   // Clear ADC SS2 flag.
}


PUBLIC void inputInit(void)
{
    //
    // Thumbstick
    //
    JOYSTICK_X_PORT->DEN   &= ~JOYSTICK_X_PIN;         // Disable den.
    JOYSTICK_X_PORT->DIR   &= ~JOYSTICK_X_PIN;         // Set to inputs.
    JOYSTICK_X_PORT->AMSEL |=  JOYSTICK_X_PIN;         // Set to analog mode.
    JOYSTICK_X_PORT->AFSEL |=  JOYSTICK_X_PIN;         // Set to alternate function.

    JOYSTICK_X_PORT->PCTL  &= ~GPIO_PCTL_PB5_M;
//    JOYSTICK_X_PORT->PCTL  |=  GPIO_PCTL_PB5_AIN11;     // Connect pin to A/D converter.

    JOYSTICK_Y_PORT->DEN   &= ~JOYSTICK_Y_PIN;         // Disable den.
    JOYSTICK_Y_PORT->DIR   &= ~JOYSTICK_Y_PIN;         // Set to inputs.
    JOYSTICK_Y_PORT->AMSEL |=  JOYSTICK_Y_PIN;         // Set to analog mode.
    JOYSTICK_Y_PORT->AFSEL |=  JOYSTICK_Y_PIN;         // Set to alternate function.

    JOYSTICK_Y_PORT->PCTL &= ~GPIO_PCTL_PD3_M;
    JOYSTICK_Y_PORT->PCTL  |= GPIO_PCTL_PD3_AIN4;      // Connect pin to A/D converter.

    ADC0->ACTSS &= ~ADC_ACTSS_ASEN2;                    // Disable sample sequencer 2 during setup
    ADC0->SAC = ADC_SAC_AVG_16X;                        // Set ADC hardware averaging.
    ADC0->EMUX = ADC_EMUX_EM2_PROCESSOR;                // Configure sample sequencer 2 to use software trigger

    ADC0->SSMUX2 = (JOYSTICK_Y_CHAN << ADC_SSMUX2_MUX1_S)    // Second channel to sample.
                 | (JOYSTICK_X_CHAN << ADC_SSMUX2_MUX0_S);   // First channel to sample.

    ADC0->SSCTL2 = ADC_SSCTL2_IE1 | ADC_SSCTL2_END1;    // Enable interrupt and mark end of sequence for SS2

    ADC0->ISC = ADC_ISC_IN2;                            // Clear any pending interrupts.
    ADC0->IM |= ADC_IM_MASK2;                           // SS2 can generate interrupts.

    ADC0->ACTSS |= ADC_ACTSS_ASEN2;                     // Enable sample sequencer 2

    //
    // Push buttons
    //
    PB_PORT->LOCK = GPIO_LOCK_KEY;
    PB_PORT->CR |= PB1_PIN | PB2_PIN;

    PB_PORT->DIR &= ~(PB1_PIN | PB2_PIN);     // Push buttons to input.
    PB_PORT->DEN |=  PB1_PIN | PB2_PIN;     // Set switches as digital enables.
    PB_PORT->PUR |=  PB1_PIN | PB2_PIN;     // Sets a pull up resistor for the switches.
    PB_PORT->IBE |=  PB1_PIN | PB2_PIN;     // Generate interrupt both edges.

    PB_PORT->ICR = PB1_PIN | PB2_PIN;       // Clear the interrupt flag ("W1C" -> write one to clear).
    PB_PORT->IM |= PB1_PIN | PB2_PIN;       // Set local interrupt enable.
}


// End file: joystick.c
