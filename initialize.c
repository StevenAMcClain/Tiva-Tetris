///////////////////////////////////////////////
// File: initialize.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include <input.h>
#include "common.h"
#include "initialize.h"

#include "eeprom.h"
#include "lcd.h"
#include "music.h"
#include "portbits.h"

PRIVATE uint32_t Ticks = 0;

#include <stdio.h>

PRIVATE uint32_t IntMasterEnable() {
    __asm("    mrs     r0, PRIMASK\n"
          "    cpsie   i\n"
          "    bx      lr\n");

    return 0;
}

PRIVATE void IntEnable(size_t num) {
  size_t base_num = num - 16;
  size_t reg = base_num / 32;
  size_t mask = 1 << (base_num & 31);

  switch (reg) {
  case 0:
    NVIC_EN0_R = mask;
    break;
  case 1:
    NVIC_EN1_R = mask;
    break;
  case 2:
    NVIC_EN2_R = mask;
    break;
  case 3:
    NVIC_EN3_R = mask;
    break;
  case 4:
    NVIC_EN4_R |= mask;
    break;
  default:
    break;
  }
}

// Set up system clock to the fastest available rate and
// return the rate in Hz
PRIVATE uint32_t setupSysClock(void)
{
    uint32_t rcc_val = SYSCTL->RCC;
    // Set sysdiv to minimum for this processor
    rcc_val &= ~SYSCTL_RCC_SYSDIV_M;
    uint32_t minsysdiv = (SYSCTL->DC1 & SYSCTL_DC1_MINSYSDIV_M) >> 12;
    rcc_val |= minsysdiv << 12;

    //Use sysdiv
    rcc_val |= SYSCTL_RCC_USESYSDIV;

    //Use 16MHz xtal
    rcc_val &= ~SYSCTL_RCC_OSCSRC_M;
    rcc_val |= 0x16 << 4;

    SYSCTL->RCC = rcc_val;

    //Turn on pll, wait for lock, then use it
    SYSCTL->RCC &= ~SYSCTL_RCC_PWRDN;
    while((SYSCTL->RIS & SYSCTL_RIS_PLLLRIS) == 0) { continue; }
    SYSCTL->RCC &= ~SYSCTL_RCC_BYPASS;

    // PLL is 400MHz, divide it by chosen divider and return
    return 400000000 / (1 + minsysdiv);
}

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

PUBLIC uint32_t SysTick_check() {
    uint32_t result = Ticks;
    Ticks = 0;
    return result;
}

PRIVATE void SysTick_Initialize(uint32_t clock_rate)
{
    SysTick->CTRL = 0; // Disable systick
    SysTick->LOAD = clock_rate / 50; // 50 times / second
    SysTick->CTRL = NVIC_ST_CTRL_CLK_SRC // Use main clock
            | NVIC_ST_CTRL_INTEN // Enable interrupt
            | NVIC_ST_CTRL_ENABLE; // Enable systick
    SysTick->VAL = SysTick->LOAD;
    //printf(
    //        "Hello:%ld\n", SysTick->LOAD);
    //fflush(stdout);
}

PUBLIC void SysTick_ISR(void) {
    Ticks += 1;
    inputTick();
}


///////////////////////////////////////////////////////////////////////
// configure_all - configure everything we need
// Arguments: none
// Return Value: none
///////////////////////////////////////////////////////////////////////

PUBLIC void configure_all(void)
{
    uint32_t clock_rate = setupSysClock();

    Enable_Device_Clocks();

    SysTick_Initialize(clock_rate);

    EEPROM_Initialize();

    // Setup input
    inputInit();

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

    // For push-buttons
    IntEnable(INT_GPIOD);       // Enable GPIO D interrupt in NVIC

    // For joy-stick.
    IntEnable(INT_ADC0SS2);     // Enable ADC0 Seq2 interrupt in NVIC

    // For music / sfx
    IntEnable(INT_TIMER0A);     // Enable Timer 0A interrupt in NVIC
    IntEnable(INT_TIMER1A);     // Enable Timer 0A interrupt in NVIC
    IntEnable(INT_TIMER3A);     // Enable Timer 3A interrupt in NVIC

    IntMasterEnable();          // Global Interrupt enable
}

