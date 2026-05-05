///////////////////////////////////////////////
// File: note_period.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//
///////////////////////////////////////////////

#ifndef NOTE_PERIOD_H_
#define NOTE_PERIOD_H_

#ifndef CPU_SPEED
#error "CPU_SPEED must be defined before including note_period.h"
#endif

#define HERTZ_PRESCALER     0       // written to TAPR register
#define HERTZ_DIVISOR       2       // actual clock divisor (TAPR + 1)

#define HERTZ_TO_TICKS(h) ((uint32_t)((CPU_SPEED / HERTZ_DIVISOR / (h)) + 0.5))

#define _RE     0

#define _C3         HERTZ_TO_TICKS(130.813)
#define _CS3        HERTZ_TO_TICKS(138.591)
#define _D3         HERTZ_TO_TICKS(146.832)
#define _DS3        HERTZ_TO_TICKS(155.563)
#define _E3         HERTZ_TO_TICKS(164.814)
#define _F3         HERTZ_TO_TICKS(174.614)
#define _FS3        HERTZ_TO_TICKS(184.997)
#define _G3         HERTZ_TO_TICKS(195.998)
#define _GS3        HERTZ_TO_TICKS(207.652)
#define _A3         HERTZ_TO_TICKS(220)
#define _AS3        HERTZ_TO_TICKS(233.082)
#define _B3         HERTZ_TO_TICKS(246.942)

#define _C4         HERTZ_TO_TICKS(261.626)
#define _CS4        HERTZ_TO_TICKS(277.183)
#define _D4         HERTZ_TO_TICKS(293.665)
#define _DS4        HERTZ_TO_TICKS(311.127)
#define _E4         HERTZ_TO_TICKS(329.628)
#define _F4         HERTZ_TO_TICKS(349.228)
#define _FS4        HERTZ_TO_TICKS(369.994)
#define _G4         HERTZ_TO_TICKS(391.995)
#define _GS4        HERTZ_TO_TICKS(415.305)
#define _A4         HERTZ_TO_TICKS(440)
#define _AS4        HERTZ_TO_TICKS(466.164)
#define _B4         HERTZ_TO_TICKS(493.883)

#define _C5         HERTZ_TO_TICKS(523.251)
#define _CS5        HERTZ_TO_TICKS(554.365)
#define _D5         HERTZ_TO_TICKS(587.33)
#define _DS5        HERTZ_TO_TICKS(622.254)
#define _E5         HERTZ_TO_TICKS(659.255)
#define _F5         HERTZ_TO_TICKS(698.456)
#define _FS5        HERTZ_TO_TICKS(739.989)
#define _G5         HERTZ_TO_TICKS(783.991)
#define _GS5        HERTZ_TO_TICKS(830.609)
#define _A5         HERTZ_TO_TICKS(880)
#define _AS5        HERTZ_TO_TICKS(932.328)
#define _B5         HERTZ_TO_TICKS(987.767)

#define _C6         HERTZ_TO_TICKS(1046.502)
#define _CS6        HERTZ_TO_TICKS(1108.731)
#define _D6         HERTZ_TO_TICKS(1174.659)
#define _DS6        HERTZ_TO_TICKS(1244.508)
#define _E6         HERTZ_TO_TICKS(1318.51)
#define _F6         HERTZ_TO_TICKS(1396.913)
#define _FS6        HERTZ_TO_TICKS(1479.978)
#define _G6         HERTZ_TO_TICKS(1567.982)
#define _GS6        HERTZ_TO_TICKS(1661.219)
#define _A6         HERTZ_TO_TICKS(1760)
#define _AS6        HERTZ_TO_TICKS(1864.655)
#define _B6         HERTZ_TO_TICKS(1975.533)

#endif /* NOTE_PERIOD_H_ */
