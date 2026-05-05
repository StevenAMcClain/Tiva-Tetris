///////////////////////////////////////////////
// File: pushbuttons.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef PUSHBUTTONS_H_
#define PUSHBUTTONS_H_

extern volatile bool PB1_pressed;
extern volatile bool PB2_pressed;            // True when pushbutton two is pressed.

extern void pushbutton_initialize(void);

//extern void test_pushbuttons(void);

#endif /* PUSHBUTTONS_H_ */
