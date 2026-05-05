///////////////////////////////////////////////
// File: pushbuttons.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"
#include "pushbuttons.h"

#include "portbits.h"


PUBLIC volatile bool PB1_pressed = false;   // True when pushbutton one is pressed.
PUBLIC volatile bool PB2_pressed = false;   // True when pushbutton two is pressed.

PRIVATE volatile bool PB1_Down = false;     // true while button is down.
PRIVATE volatile bool PB2_Down = false;     // true while button is down.


#define BOTH_PINS (PB1_PIN | PB2_PIN)

#define PB1_PRESSED() ( !(PB_PORT->DATA & PB1_PIN) )
#define PB2_PRESSED() ( !(PB_PORT->DATA & PB2_PIN) )

#define DEBOUNCE_TIME  6500     // ~10mS - assuming 80mHz, 127 pre-scaler.



PUBLIC void pushbutton_initialize(void)
{
    PB_PORT->LOCK = GPIO_LOCK_KEY;
    PB_PORT->CR |= BOTH_PINS;

    PB_PORT->DIR &= ~BOTH_PINS;     // Push buttons to input.
    PB_PORT->DEN |=  BOTH_PINS;     // Set switches as digital enables.
    PB_PORT->PUR |=  BOTH_PINS;     // Sets a pull up resistor for the switches.
    PB_PORT->IBE |=  BOTH_PINS;     // Generate interrupt both edges.

    PB_PORT->ICR = BOTH_PINS;       // Clear the interrupt flag ("W1C" -> write one to clear).
    PB_PORT->IM |= BOTH_PINS;       // Set local interrupt enable.

    TIMER2->CTL &= ~(TIMER_CTL_TAEN | TIMER_CTL_TBEN); // Ensure both timers are disabled.

    TIMER2->CFG = TIMER_CFG_16_BIT;                    // Set to 16-bit mode

    TIMER2->TAMR = TIMER_TAMR_TAMR_1_SHOT;             // Set timer A to one-shot mode.
    TIMER2->TBMR = TIMER_TBMR_TBMR_1_SHOT;             // Set timer B to one-shot mode.

    TIMER2->TAPR = TIMER2->TBPR = 127;

    TIMER2->TAILR = TIMER2->TBILR = DEBOUNCE_TIME;     // De-bounce time to ~10ms.

    TIMER2->ICR = TIMER_ICR_TATOCINT | TIMER_ICR_TBTOCINT;   // Clear the time out interrupt flags
    TIMER2->IMR |= TIMER_IMR_TATOIM | TIMER_IMR_TBTOIM;      // Enable Timer A and B time-out interrupt
}


PUBLIC void GPIOD_ISR(void)
//
// ISR called on falling or rising edge for pushbutton A or pushbutton B.
{
    if (PB_PORT->MIS & PB1_PIN)
    {
        if (PB1_Down)
        {
            if (!PB1_PRESSED())
            {
                TIMER2->CTL |= TIMER_CTL_TAEN; // start timer
                PB_PORT->IM &= ~PB1_PIN;       // Clear local interrupt enable.
            }
        }
        else if (PB1_PRESSED())
        {
            TIMER2->CTL |= TIMER_CTL_TAEN; // start timer
            PB_PORT->IM &= ~PB1_PIN;       // Clear local interrupt enable.
        }

        PB_PORT->ICR = PB1_PIN;
    }

    if (PB_PORT->MIS & PB2_PIN)
    {
        if (PB2_Down)
        {
            if (!PB2_PRESSED())
            {
                TIMER2->CTL |= TIMER_CTL_TBEN; // start timer
                PB_PORT->IM &= ~PB2_PIN;       // Clear local interrupt enable.
            }
        }
        else if (PB2_PRESSED())
        {
            TIMER2->CTL |= TIMER_CTL_TBEN; // start timer
            PB_PORT->IM &= ~PB2_PIN;       // Clear local interrupt enable.
        }

        PB_PORT->ICR = PB2_PIN;
    }
}


PUBLIC void Timer2A_ISR(void)
//
// Called after pushbutton A de-bounce time.
{
    PB_PORT->ICR = PB1_PIN;
    TIMER2->ICR = TIMER_ICR_TATOCINT;   // clear the time out interrupt flags

    if (PB1_Down)
    {
        if (!PB1_PRESSED())
        {
            PB1_Down = false;
        }
    }
    else if (PB1_PRESSED())
    {
        PB1_pressed = PB1_Down = true;                 // signal pushbutton one press.
    }

    PB_PORT->IM |= PB1_PIN;       // Re-enable  local interrupt enable.
}


PUBLIC void Timer2B_ISR(void)
//
// Called after pushbutton B de-bounce time.
{
    PB_PORT->ICR = PB2_PIN;
    TIMER2->ICR = TIMER_ICR_TBTOCINT;   // clear the time out interrupt flags

    if (PB2_Down)
    {
        if (!PB2_PRESSED())
        {
            PB2_Down = false;
        }
    }
    else if (PB2_PRESSED())
    {
        PB2_pressed = PB2_Down = true;                 // signal pushbutton two press.
    }

    PB_PORT->IM |= PB2_PIN;       // Re-enable  local interrupt enable.
}


#ifdef TEST
void test_pushbuttons(void)
//
// Use this to test pushbutton operation.
{
    int pb1=0;
    int pb2=0;

    printf("Start\n");

    while (1)
    {
        if (PB1_pressed)
        {
            PB1_pressed = false;         // acknowledge pushbutton one press.
            printf("press one %d\n", ++pb1);
        }

        if (PB2_pressed)
        {
            PB2_pressed = false;         // acknowledge pushbutton two press.
            printf("press two %d\n", ++pb2);
        }
    }
}
#endif // TEST


// end file: pushbuttons.c
