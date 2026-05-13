/*
 * PortBits.h
 *
 *  Created on: Jun. 3, 2025
 *      Author: Steven
 */

#ifndef PORTBITS_H_
#define PORTBITS_H_

#define LED_RED_PIN    BIT1
#define LED_GREEN_PIN  BIT3
#define LED_BLUE_PIN   BIT2

#define ALL_LED_PINS (LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN)

#define LED_BASE_RGB_PORT   GPIOF

#define LED_BOOSTER_RED_PORT   GPIOF
#define LED_BOOSTER_RED_PIN    BIT3

#define LED_BOOSTER_GREEN_PORT GPIOB
#define LED_BOOSTER_GREEN_PIN BIT3

#define LED_BOOSTER_BLUE_PORT GPIOC
#define LED_BOOSTER_BLUE_PIN BIT4

#endif /* PORTBITS_H_ */
