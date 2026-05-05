///////////////////////////////////////////////
// File: portbits.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#ifndef PORTBITS_H_
#define PORTBITS_H_

// Test pin.

#define TEST_PORT GPIOB
#define TEST_PIN BIT3

#define SET_TEST_BIT() T(TEST_PORT->DATA |=  TEST_PIN)
#define CLR_TEST_BIT() T(TEST_PORT->DATA &= ~TEST_PIN)

#define TEST2_PORT GPIOF
#define TEST2_PIN BIT3

#define SET_TEST2_BIT() T(TEST2_PORT->DATA |=  TEST2_PIN)
#define CLR_TEST2_BIT() T(TEST2_PORT->DATA &= ~TEST2_PIN)

// Pushbutton Ports and pins.

#define PB_PORT GPIOD
#define PB1_PIN BIT6          // PF6 - Pushbutton 1
#define PB2_PIN BIT7          // PF7 - Pushbutton 2

// Joy-stick.

#define JOYSTICK_X_PORT GPIOB
#define JOYSTICK_X_PIN  BIT5     // PB5 Joy-stick X
#define JOYSTICK_X_CHAN 11

#define JOYSTICK_Y_PORT GPIOD
#define JOYSTICK_Y_PIN  BIT3     // PD3 Joy-stick Y
#define JOYSTICK_Y_CHAN 4

// LCD pins
#define LCD_SCK           BIT4  // PB4 for SSI2 SCK connects to LCD SCK
#define LCD_MOSI          BIT7  // PB7 for SSI2 MOSI connects to LCD SDA
#define LCD_RST           BIT0  // PF0 connects to LCD RST pin
#define LCD_CS            BIT4  // PA4 connects to LCD CS pin
#define LCD_DC            BIT4  // PF4 connects to LCD DC pin

#endif /* PORTBITS_H_ */
