///////////////////////////////////////////////
// File: music.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#define TRACE
#include "common.h"

#ifdef TRACE
#include "portbits.h"
#endif

#include "music.h"

#include "note_period.h"


PRIVATE volatile unsigned int Music_Enabled = 0;
PRIVATE volatile unsigned int SoundFX_Playing = 0;

// Music globals for interrupts

PRIVATE volatile unsigned int indexHigh = 0;
PRIVATE volatile unsigned int indexLow = 0;

// For ~30ms polyphony slot at 80MHz:
//#define SIM_NOTE_DURATION   (CPU_SPEED / 64)   // ~32 slots/sec
#define SIM_NOTE_DURATION   (CPU_SPEED / 128)   // ~32 slots/sec

// DEFINES FOR THE MUSIC

//#define BPM_TO_MS       125   //500
#define BPM_TO_MS       250   //500
#define MS_CYCLES       (CPU_SPEED / 1000)   // 80 MHz -> 80,000 cycles/ms

#define BEAT_DURATION   (BPM_TO_MS * MS_CYCLES)  // = 40,000,000 ticks

#define QUARTER_NOTE        (BEAT_DURATION / 4) // 0.25
#define HALF_NOTE           (BEAT_DURATION / 2) // 0.5
#define FULL_NOTE            BEAT_DURATION      // 1
#define FULL_HALF_NOTE      (BEAT_DURATION + (BEAT_DURATION / 2)) // 1.5
#define DOUBLE_NOTE         (BEAT_DURATION * 2)  // 2
#define QUADRUPLE_NOTE      (BEAT_DURATION * 4)  // 4

#define MAX_LOW_INDEX  ARRAY_SIZE(Note_Low)
#define MAX_HIGH_INDEX ARRAY_SIZE(Note_High)

// MUSIC DATA ARRAYS //

// THIS ARRAY HOLDS ALL THE HIGH NOTES TO BE PLAYED
PRIVATE uint32_t Note_High[] =
{
    _E6, _B5, _C6, _D6, _E6, _D6, _C6, _B5, _A5, _A5,
    _C6, _E6, _D6, _C6, _B5, _C6, _D6, _E6, _C6, _A5,
    _A5, _RE, _D6, _F6, _A6, _G6, _F6, _E6, _C6, _E6,
    _D6, _C6, _B5, _B5, _C6, _D6, _E6, _C6, _A5, _A5,
    _RE, _E5, _C5, _D5, _B4, _C5, _A4, _GS4, _B4, _E5,
    _C5, _D5, _B4, _C5, _E5, _A5, _GS5
};

// THIS HOLDS THE DURATIONS OF THE HIGH NOTES
PRIVATE uint32_t Time_High[] =
{
    FULL_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, QUARTER_NOTE, QUARTER_NOTE, HALF_NOTE, HALF_NOTE, FULL_NOTE, HALF_NOTE,
    HALF_NOTE, FULL_NOTE, HALF_NOTE, HALF_NOTE, FULL_HALF_NOTE, HALF_NOTE, FULL_NOTE, FULL_NOTE, FULL_NOTE, FULL_NOTE,
    DOUBLE_NOTE, HALF_NOTE, FULL_NOTE, HALF_NOTE, FULL_NOTE, HALF_NOTE, HALF_NOTE, FULL_HALF_NOTE, HALF_NOTE, FULL_NOTE,
    HALF_NOTE, HALF_NOTE, FULL_NOTE, HALF_NOTE, HALF_NOTE, FULL_NOTE, FULL_NOTE, FULL_NOTE, FULL_NOTE, FULL_NOTE,
    FULL_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE,
    DOUBLE_NOTE, DOUBLE_NOTE, DOUBLE_NOTE, FULL_NOTE, FULL_NOTE, DOUBLE_NOTE, QUADRUPLE_NOTE
};

// THIS ARRAY HOLDS ALL THE LOW NOTES TO BE PLAYED
PRIVATE uint32_t Note_Low[] =
{
    _E3, _E4, _E3, _E4, _E3, _E4, _E3, _E4, _A3, _A4,
    _A3, _A4, _A3, _A4, _A3, _A4, _GS3, _GS4, _GS3, _GS4,
    _E3, _E4, _E3, _E4, _A3, _A4, _A3, _A4, _A3, _A4, _B3,
    _C4, _D4, _D3, _RE, _D3, _RE, _D3, _A3, _F3, _C3, _C4,
    _RE, _C4, _C3, _G3, _RE, _G3, _B3, _B4, _RE, _B4, _RE,
    _E4, _RE, _GS4, _A3, _E4, _A3, _E4, _A3, _RE, _A3, _E4,
    _A3, _E4, _A3, _E4, _A3, _E4, _GS3, _E4, _GS3, _E4, _GS3,
    _E4, _GS3, _E4, _A3, _E4, _A3, _E4, _A3, _E4, _A3, _E4,
    _GS3, _E4, _GS3, _E4, _GS3, _E4, _GS3, _E4, _A3, _E4, _A3,
    _E4, _A3, _E4, _A3, _E4, _GS3, _E4, _GS3, _E4, _GS3, _E4,
    _GS3, _E4, _A3, _E4, _A3, _E4, _A3, _E4, _A3, _E4, _GS3,
    _E4, _GS3, _E4, _GS3, _E4, _GS3, _E4
};

// THIS HOLDS THE DURATIONS OF THE LOW NOTES
PRIVATE uint32_t Time_Low[] =
{
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, FULL_NOTE, FULL_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE,
    HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE, HALF_NOTE
};

// THIS HOLDS ALL THE NOTES REQUIRED TO PLAY THE SFX
#define SFX_NOTE_ARRAY_ROWS      13
#define SFX_NOTE_ARRAY_COLS      25

PRIVATE uint32_t Note_Sfx[SFX_NOTE_ARRAY_ROWS][SFX_NOTE_ARRAY_COLS] =
{
 // Filler to use up [0][X]
/* 0 */    {VOID, VOID},

// 1st line cleared
/* 1 */    {_GS5, _GS5, VOID},

// 2nd line cleared
/* 2 */    {_CS6, _CS6, VOID},

// 3rd line cleared
/* 3 */    {_FS6, _FS6, VOID},

// 4th line cleared
/* 4 */    {_AS6,_AS6, _GS6,_GS6,_DS6,_DS6,_AS5,_AS5,_DS6,_DS6, VOID},

// rotate
/* 5 */    {_CS4, _CS4, VOID},

// hit the bottom
/* 6 */    {_DS5, _DS5, VOID},

// GAME OVER
/* 7 */    {_B4,_B4,_AS4,_AS4,_B4,_B4,_AS4,_AS4,_B4,_B4,_C5,_C5,_B4,_B4,_AS4,_AS4,_B4,_B4,_B4,_B4,_B4,_B4,_B4,_B4, VOID},
/* 8 */    {_CS5,_CS5,_C5,_C5,_CS5,_CS5,_D5,_D5,_CS5,_CS5,_CS5,_CS5,VOID},
/* 9 */    {_FS5,_FS5,_F5,_F5,_FS5,_FS5,_G5,_G5,_FS5,_FS5,_FS5,_FS5,VOID},
/* 10 */    {_AS5,_AS5,_A5,_A5,_AS5,_AS5,_B5,_B5,_AS5,_AS5,_AS5,_AS5,VOID},

// new high score sound effect
/* 11 */    {_AS4,_AS4,_CS5,_CS5,_DS5,_DS5,_FS5,_FS5,_GS5,_GS5,_AS5,_AS5,_CS6,_CS6,_AS5,_AS5,_CS6,_CS6,_AS5,_AS5,_CS6,_CS6,_CS6,_CS6,VOID},

// old high score sound effect
/* 12 */    {_DS6,_DS6,_D6,_D6,_DS6,_DS6,_E6,_E6,_E6,_E6,_E6,_E6,_DS6,_DS6,_DS6,_DS6,_DS6,_DS6,_DS6,_DS6,VOID}
};


#define BUZZER_PORT GPIOF
#define BUZZER_PIN BIT2

#define _1MS   ((int)CPU_SPEED / 1000) // Cycles per ms for 80 MHz CPU clock
#define _500MS ((int)CPU_SPEED / 8)    // Cycles per 500ms for 80 MHz CPU clock


PUBLIC void Music_Initialize(void)
{
    // Setup the pin which will output the PWM signal controlling the buzzer.
    //
    BUZZER_PORT->DIR   |= BUZZER_PIN;           // Buzzer port to output.
    BUZZER_PORT->DEN   |= BUZZER_PIN;           // Buzzer port digital enable.

//    BUZZER_PORT->AFSEL |= BUZZER_PIN;           // Buzzer port alternate function select.
//    BUZZER_PORT->PCTL = (BUZZER_PORT->PCTL & ~GPIO_PCTL_PF2_M) | GPIO_PCTL_PF2_T1CCP0; // Connect to timer 1A output.

    // This timer generates the note being played.  (PWM timer directly connected to pin).
    //
    TIMER1->CTL  &= ~TIMER_CTL_TAEN;             // ensure timer is disabled.
    TIMER1->CFG   =  TIMER_CFG_32_BIT_TIMER;    // set to 32-bit mode.
//    TIMER1->CTL  |=  TIMER_CTL_TAPWML;           // set output level such that higher Match value gives greater duty cycle
//    TIMER1->TAMR  =  TIMER_TAMR_TAAMS | TIMER_TAMR_TAMR_PERIOD; // set to periodic mode, count down, PWM mode
    TIMER1->TAMR  =  TIMER_TAMR_TAMR_PERIOD; // set to periodic mode, count down, PWM mode

//    TIMER1->TAPR  = 0;                          // Setup pre-scaler.
    TIMER1->TAILR = _1MS;                       // set the interval for 1ms
//    TIMER1->TAMATCHR = TIMER1->TAILR / 2;       // set to 50% duty cycle
    TIMER1->ICR   =  TIMER_ICR_TATOCINT;        // clear the time out interrupt flag.
    TIMER1->IMR  |=  TIMER_IMR_TATOIM;          // enable Timer A time-out interrupt.

    // This timer controls when the next note should be played.
    //
    TIMER0->CTL  &= ~TIMER_CTL_TAEN;            // ensure timer is disabled.
    TIMER0->CFG   =  TIMER_CFG_32_BIT_TIMER;    // set to 32-bit mode.
    TIMER0->TAMR  =  TIMER_TAMR_TAMR_PERIOD;    // set to periodic mode.
    TIMER0->TAILR =  _500MS;                    // set note duration to 0.5 seconds.
    TIMER0->ICR   =  TIMER_ICR_TATOCINT;        // clear the time out interrupt flag.
    TIMER0->IMR  |=  TIMER_IMR_TATOIM;          // enable Timer A time-out interrupt.

    // This timer controls the polyphony.
    //
    TIMER3->CTL  &= ~TIMER_CTL_TAEN;            // ensure time is disabled.
    TIMER3->CFG   =  TIMER_CFG_32_BIT_TIMER;    // set to 32-bit mode.
    TIMER3->TAMR  =  TIMER_TAMR_TAMR_PERIOD;    // set to periodic mode.
    TIMER3->TAILR =  _1MS;                      // set note duration to 0.5 seconds.
    TIMER3->ICR   =  TIMER_ICR_TATOCINT;        // clear the time out interrupt flag.
    TIMER3->IMR  |=  TIMER_IMR_TATOIM;          // enable Timer A time-out interrupt.
}

PRIVATE void set_note_period(uint32_t note)
{
    static unsigned int last_note = 0;

    if (note != last_note)
    {
        last_note = note;

        if (note == 0)
        {
            TIMER1->CTL &= ~TIMER_CTL_TAEN;  // silence
        }
        else
        {
            TIMER1->CTL &= ~TIMER_CTL_TAEN;

            TIMER1->TAILR = note;
//            TIMER1->TAPR     = (note >> 16) & 0xFF;
//            TIMER1->TAILR    = note & 0xFFFF;

//            int duty = note / 2;     // Always a 50% duty cycle.

//            TIMER1->TAPMR    = (duty>> 16) & 0xFF;
//            TIMER1->TAMATCHR = duty & 0xFFFF;

            TIMER1->CTL |= TIMER_CTL_TAEN;
        }
    }
}

PUBLIC void Music_On(int m)
//
// Enable music timers.
{
    Music_Enabled = m;

    indexHigh = 0;
    indexLow = 0;

    // TIMER1->CTL |= TIMER_CTL_TAEN;      // Start note oscillator.
    TIMER0->CTL |= TIMER_CTL_TAEN;      // Start note timer.

    TIMER3->TAILR = SIM_NOTE_DURATION;
    TIMER3->CTL |= TIMER_CTL_TAEN;      // Start polyphony timer.
}


PRIVATE void note_off(void)
{
    TIMER1->CTL &= ~TIMER_CTL_TAEN;      // Stop note.
}


PUBLIC void Music_Off(void)
//
// Disable music timers.
{
    SoundFX_Playing = DISABLED;
    note_off();
    TIMER0->CTL &= ~TIMER_CTL_TAEN;      // Stop note timer.
    TIMER3->CTL &= ~TIMER_CTL_TAEN;      // Stop polyphony timer.
}


PUBLIC void SoundFX_Wait()
//
// Wait for any previous sfx to finish.
{
    if(Music_Enabled == OFF) return;
    while (SoundFX_Playing) { continue; }
}


PUBLIC void SoundFX_Start(int x)
//
// Start playing music or sound FX.
{
    if(Music_Enabled == OFF) return;
    SoundFX_Playing = x;
}


PUBLIC void SoundFX_Play(int x)
//
// Wait for current music of sfx to complete
//   then start the next one.
{
    if(Music_Enabled == OFF) return;
    SoundFX_Wait();
    SoundFX_Start(x);
}

PRIVATE void set_next_note_time(uint32_t next)
{
    TIMER0->TAILR = TIMER0->TAV = next;
//    TIMER0->TAILR = next;
}


PUBLIC void Timer1_ISR(void)
//
// Called for note oscillator.
{
    TIMER1->ICR = TIMER_ICR_TATOCINT;       // clear interrupt flag.
    BUZZER_PORT->DATA ^= BUZZER_PIN;        // Toggle buzzer port pin.
}


PUBLIC void Timer0_ISR(void)
//
// Called to change to the next note.
{
    static int32_t current_time = HALF_NOTE;
    static int32_t timeleft_high = HALF_NOTE;
    static int32_t timeleft_low = FULL_NOTE;

    TIMER0->ICR = TIMER_ICR_TATOCINT;       // clear interrupt flag.

    timeleft_high -= current_time;          // decrement the time for the high note.

    if (timeleft_high <= 0)                 // check if the high note has finished playing.
    {
        if (++indexHigh >= MAX_HIGH_INDEX)  // increment the note to be played.
        {                                   // check if we have run out of notes to play.
            indexHigh = 0;                  // loop back to the first note.
        }
        timeleft_high = Time_High[indexHigh];   // set the duration of the new note.
    }

    timeleft_low -= current_time;           // decrement the time for the low note.

    if (timeleft_low <= 0)                  // check if the low note has finished playing.
    {
        if (++indexLow >= MAX_LOW_INDEX)    // increment the note to be played.
        {                                   // check if we have run out of notes to play.
            indexLow = 0;                   // loop back to the first note0.
        }

        timeleft_low = Time_Low[indexLow]; // set the duration of the new note
    }

    current_time = MIN(Time_Low[indexLow], Time_High[indexHigh]); // set the time at which the next interrupt will occur the be the lowest duration of the two notes

    set_next_note_time(current_time);
}

#define POLY_HIGH 0
#define POLY_LOW  1
#define POLY_SFX  2


PUBLIC void Timer3A_ISR(void)
//
// Called for poly-phony.
{
    SET_TEST_BIT();

    static int phase = 0;    // this holds the count, so we know which note to play for the next cycle of polyphony
    static int indexSfx = 0; // this hold the position of the note were playing for the SFX

    TIMER3->ICR = TIMER_ICR_TATOCINT;        // clear the time out interrupt flag.

    if (Music_Enabled != OFF)   // check if the music is supposed to be playing
    {
        unsigned int note = 0;

        if (SoundFX_Playing && (phase == POLY_SFX))
        {
            note = Note_Sfx[SoundFX_Playing][indexSfx];

            if (!note)        // check if we have reached the end of the SFX
            {
                indexSfx = 0;           // reset the index
                SoundFX_Playing = 0;    // reset the flag indicating a sfx is playing
            }
            else
            {
                indexSfx++;  // increment the index
            }
        }
        else if (Music_Enabled == ALL)
        {
                 if (phase == POLY_HIGH) { note = Note_High[indexHigh]; }
            else if (phase == POLY_LOW)  { note = Note_Low[indexLow];   }
        }

        set_note_period(note);
    }
    else // if no music is supposed to be playing, disable all the music
    {
        Music_Off();
    }

    int next = (!SoundFX_Playing)     ?  SIM_NOTE_DURATION
             : (Music_Enabled == ALL) ? (SIM_NOTE_DURATION / 2) * 3
             : (Music_Enabled == SFX) ?  SIM_NOTE_DURATION * 2 : SIM_NOTE_DURATION;

    TIMER3->TAILR = next;

    // check if count needs to be reset.  only two slots if sfx not playing.
    static int last_phases = 2;
    int phases = SoundFX_Playing ? 3 : 2;

    if (phases != last_phases)
    {
        phase = 0;
        last_phases = phases;
    }

    if (++phase >= phases) { phase = 0; }

    CLR_TEST_BIT();
}


// end file: music.c


