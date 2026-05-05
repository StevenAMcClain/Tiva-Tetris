///////////////////////////////////////////////
// File: initialize.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"
#include "initialize.h"

#include <interrupt.h>

#include "eeprom.h"
#include "joystick.h"
#include "lcd.h"
#include "music.h"
#include "portbits.h"
#include "pushbuttons.h"
#include "systick.h"


//#include <stdint.h>
//#include "tm4c123gh6pm.h"


PRIVATE void clock_80MHz(void)
{
    SYSCTL->RCC2 |= SYSCTL_RCC2_USERCC2;                                         // 1. Use RCC2 for advanced features
    SYSCTL->RCC2 |= SYSCTL_RCC2_BYPASS2;                                         // 2. Bypass PLL while initializing
    SYSCTL->RCC2 &= ~SYSCTL_RCC2_OSCSRC2_M;                                      // 3. Select main oscillator (MOSC) as clock source
    SYSCTL->RCC   = (SYSCTL->RCC & ~SYSCTL_RCC_XTAL_M) | SYSCTL_RCC_XTAL_16MHZ;    // 4. Configure crystal value for 16 MHz external crystal
    SYSCTL->RCC2 &= ~SYSCTL_RCC2_PWRDN2;                                         // 5. Activate PLL by clearing PWRDN
    SYSCTL->RCC2 |= SYSCTL_RCC2_DIV400;                                          // 6. Use 400 MHz PLL (DIV400 = 1)
    SYSCTL->RCC2  = (SYSCTL->RCC2 & ~SYSCTL_RCC2_SYSDIV2_M)
                  | (4 << SYSCTL_RCC2_SYSDIV2_S);                                // 7. Set system divider for 80 MHz: 400 MHz / 2.5 = 80 MHz
                                                                                       // SYSDIV2 = 4 -> divide by 2.5
    while((SYSCTL->RIS & SYSCTL_RIS_PLLLRIS) == 0) { continue; }                 // 8. Wait for PLL to lock
    SYSCTL->RCC2 &= ~SYSCTL_RCC2_BYPASS2;                                        // 9. Enable PLL by clearing BYPASS
}


//PRIVATE void InitClock_80MHz(void)
//{
//    SYSCTL->RCC2 |=  SYSCTL_RCC2_USERCC2;                   // Use RCC2 register
//    SYSCTL->RCC2 |=  SYSCTL_RCC2_BYPASS2;                   // Bypass PLL while initializing
//
//    SYSCTL->RCC  = (SYSCTL->RCC & ~SYSCTL_RCC_XTAL_M)       // Set crystal value and oscillator source
//                 |  SYSCTL_RCC_XTAL_16MHZ;
//
//    SYSCTL->RCC2 = (SYSCTL->RCC2 & ~SYSCTL_RCC2_OSCSRC2_M)
//                 |  SYSCTL_RCC2_OSCSRC2_MO;                 // Use main oscillator
//
//    SYSCTL->RCC2 &= ~SYSCTL_RCC2_PWRDN2;                    // Clear PWRDN to activate PLL
//    SYSCTL->RCC2 |=  SYSCTL_RCC2_DIV400;                    // Use 400 MHz PLL output
//
//    SYSCTL->RCC2 = (SYSCTL->RCC2 & ~SYSCTL_RCC2_SYSDIV2_M)
//                 | (4 << SYSCTL_RCC2_SYSDIV2_S)
//                 | ;   // 400 / (4 + 0.5 + 0.5) = 80 MHz
//
//    while ((SYSCTL->RIS & SYSCTL_RIS_PLLLRIS) == 0);    // Wait for the PLL to lock
//
//    SYSCTL->RCC2 &= ~SYSCTL_RCC2_BYPASS2;               // Enable use of PLL by clearing BYPASS
//}


PRIVATE void Enable_Device_Clocks(void)
{
    // Enable GPIO clocks.

    SYSCTL->RCGC2 |= ( SYSCTL_RCGC2_GPIOA         // activate clock for GPIO Port A
                     | SYSCTL_RCGC2_GPIOB         // activate clock for GPIO Port B
                     | SYSCTL_RCGC2_GPIOD         // activate clock for GPIO Port D
                     | SYSCTL_RCGC2_GPIOF);       // activate clock for GPIO Port F

    // Enable ADC clocks.

    SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;         // Enable clock for ADC0

    // Enable SSI2

    SYSCTL->RCGCSSI |= SYSCTL_RCGCSSI_R2;         // Enable clock for SSI2

    // Enable Timers

    SYSCTL->RCGCTIMER |= ( SYSCTL_RCGCTIMER_R0     // activate clock for Timer 0 Module
                         | SYSCTL_RCGCTIMER_R1     // activate clock for Timer 1 Module
                         | SYSCTL_RCGCTIMER_R2     // activate clock for Timer 2 Module
                         | SYSCTL_RCGCTIMER_R3);   // activate clock for Timer 3 Module

//    SYSCTL->RCGCWTIMER |= SYSCTL_RCGCWTIMER_R0;

    // Enable EEPROM system.

    SYSCTL->RCGCEEPROM |= SYSCTL_RCGCEEPROM_R0;

    __delay_cycles(5);   // Small delay for clocks to stabilize.
}


///////////////////////////////////////////////////////////////////////
// configure_all - configure everything we need
// Arguments: none
// Return Value: none
///////////////////////////////////////////////////////////////////////

PUBLIC void configure_all(void)
{
    clock_80MHz();
//    InitClock_80MHz();

    Enable_Device_Clocks();

    EEPROM_Initialize();

// Setup joy-stick
    JoyStick_Initialize();

// Configure Push buttons
    pushbutton_initialize();

    TEST_PORT->DEN  |= TEST_PIN;
    TEST_PORT->DIR  |= TEST_PIN;
    TEST_PORT->DATA &= ~TEST_PIN;

    TEST2_PORT->DEN  |= TEST2_PIN;
    TEST2_PORT->DIR  |= TEST2_PIN;
    TEST2_PORT->DATA &= ~TEST2_PIN;

// Setup music player
    Music_Initialize();

// Configure LCD
    lcdInit(); // initialize the lcd

// Setup system tick.
    SysTick_Initialize();

    // For push-buttons
    IntEnable(INT_GPIOD);       // Enable GPIO D interrupt in NVIC
    IntEnable(INT_TIMER2A);     // Enable Timer 2A interrupt in NVIC
    IntEnable(INT_TIMER2B);     // Enable Timer 2B interrupt in NVIC

    // For joy-stick.
    IntEnable(INT_ADC0SS2);     // Enable ADC0 Seq2 interrupt in NVIC

    // For music / sfx
    IntEnable(INT_TIMER0A);     // Enable Timer 0A interrupt in NVIC
    IntEnable(INT_TIMER1A);     // Enable Timer 0A interrupt in NVIC
    IntEnable(INT_TIMER3A);     // Enable Timer 3A interrupt in NVIC

    IntMasterEnable();          // Global Interrupt enable
}

