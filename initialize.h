///////////////////////////////////////////////
// File: initialize.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

extern void configure_all(void);

// Return number of sys ticks since last call to SysTick_check()
extern uint32_t SysTick_check();

#endif /* INITIALIZE_H_ */
