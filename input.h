///////////////////////////////////////////////
// File: joystick.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//
#ifndef INPUT_H_
#define INPUT_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct RawInput {
    bool pb1;
    bool pb2;
    uint32_t thumbh;
    uint32_t thumbv;

} Input;

extern Input input;

extern void inputInit(void);
extern void inputTick(void);

#endif /* INPUT_H_ */
