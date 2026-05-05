///////////////////////////////////////////////
// File: common.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef COMMON_H_
#define COMMON_H_

#include <tiva.h>

#include <stdbool.h>

#include "tetris.h"

#define CPU_SPEED 80000000      // 80 MHz

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

#define PUBLIC            // Just a label to show symbol is public.
#define PRIVATE static

#define FOREVER true

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// define TRACE before including "common.h" to enable tracing.

#ifdef TRACE
#define T(x) x
#else
#define T(x)
#endif

#endif /* COMMON_H_ */
