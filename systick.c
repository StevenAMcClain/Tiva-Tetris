///////////////////////////////////////////////
// File: systick.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"
#include "systick.h"

#include "joystick.h"
#include "portbits.h"


PUBLIC volatile bool Frame_Tick = false;


PUBLIC void SysTick_Initialize(void)
{
    // Reload = (SystemClock / 20) - 1
    NVIC_ST_RELOAD_R = 2000000 - 1;   // 25 ms at 80 MHz
    NVIC_ST_CURRENT_R = 0;            // clear current
    NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC
                   | NVIC_ST_CTRL_INTEN
                   | NVIC_ST_CTRL_ENABLE;            // enable SysTick, interrupt, system clock
}


PUBLIC void SysTick_ISR()
{
    ADC_Start();

    Frame_Tick = true;
}


// end file: systick.c
