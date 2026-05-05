///////////////////////////////////////////////
// File: systick.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef SYSTICK_H_
#define SYSTICK_H_

extern volatile bool Frame_Tick;

extern void SysTick_Initialize(void);

#endif /* SYSTICK_H_ */
