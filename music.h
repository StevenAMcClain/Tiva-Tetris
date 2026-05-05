///////////////////////////////////////////////
// File: music.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef MUSIC_H_
#define MUSIC_H_

#define SOUND_EFFECT_1      1
#define SOUND_EFFECT_2      2
#define SOUND_EFFECT_3      3
#define SOUND_EFFECT_4      4
#define SOUND_EFFECT_5      5
#define SOUND_EFFECT_6      6
#define SOUND_EFFECT_7      7
#define SOUND_EFFECT_8      8
#define SOUND_EFFECT_9      9
#define SOUND_EFFECT_10     10
#define SOUND_EFFECT_11     11
#define SOUND_EFFECT_12     12

extern void Music_Initialize();
extern void Music_On(int m);
extern void Music_Off(void);

extern void SoundFX_Wait();
extern void SoundFX_Start(int x);
extern void SoundFX_Play(int x);

#endif /* MUSIC_H_ */
