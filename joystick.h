///////////////////////////////////////////////
// File: joystick.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//
#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define MENU_SHIFT_DOWN     1800        // Threshold for DOWN.
#define MENU_SHIFT_UP       2200        // Threshold for UP.



extern void JoyStick_Initialize(void);

extern int JoyStick_Read_X(void);
extern int JoyStick_Read_Y(void);

extern void ADC_Start(void);

#endif /* JOYSTICK_H_ */
