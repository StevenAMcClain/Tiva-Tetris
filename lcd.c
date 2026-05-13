///////////////////////////////////////////////
// File: lcd.c
// Description: Module to control the color 128x128 TFT LCD display on Educational BoosterPack MKII
//
//  Originally by: Robert Trost  Feb 20, 2016
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"
#include "lcd.h"

#include "portbits.h"


// ST7735 LCD controller Command Set (copied from TI sample code)
#define CM_NOP             0x00
#define CM_SWRESET         0x01
#define CM_RDDID           0x04
#define CM_RDDST           0x09
#define CM_SLPIN           0x10
#define CM_SLPOUT          0x11
#define CM_PTLON           0x12
#define CM_NORON           0x13
#define CM_INVOFF          0x20
#define CM_INVON           0x21
#define CM_GAMSET          0x26
#define CM_DISPOFF         0x28
#define CM_DISPON          0x29
#define CM_CASET           0x2A
#define CM_RASET           0x2B
#define CM_RAMWR           0x2C
#define CM_RGBSET          0x2d
#define CM_RAMRD           0x2E
#define CM_PTLAR           0x30
#define CM_MADCTL          0x36
#define CM_COLMOD          0x3A
#define CM_SETPWCTR        0xB1
#define CM_SETDISPL        0xB2
#define CM_FRMCTR3         0xB3
#define CM_SETCYC          0xB4
#define CM_SETBGP          0xb5
#define CM_SETVCOM         0xB6
#define CM_SETSTBA         0xC0
#define CM_SETID           0xC3
#define CM_GETHID          0xd0
#define CM_SETGAMMA        0xE0
#define CM_MADCTL_MY       0x80
#define CM_MADCTL_MX       0x40
#define CM_MADCTL_MV       0x20
#define CM_MADCTL_ML       0x10
#define CM_MADCTL_BGR      0x08
#define CM_MADCTL_MH       0x04

// Correction factors for display offsets (deduced from TI sample code, but not completely sure why)
#define X_CORRECTION_OFFSET 2
#define Y_CORRECTION_OFFSET 1

// define # CPU cycles for delays
#define _1ms (80000)
#define _10ms (10 * _1ms)
#define _50ms (50 * _1ms)
#define _120ms (120 * _1ms)
#define _200ms (200 * _1ms)

// Argument for isData in lcdWrite function
//
#define isCMD  0
#define isDATA 1


///////////////////////////////////////////////////////////////////////
// lcdWrite - Sends a command/data byte to the LCD.
// Arguments: byte - value to be sent to the LCD
//            isData - true if byte is data, false if byte is a command
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdWrite(unsigned char byte, int isData);


///////////////////////////////////////////////////////////////////////
// lcdInit - Initializes ports, SPI settings and LCD.
// Arguments: none
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdInit()
{
    // enable LCD_CS, LCD_DC & LCD_RST as digital outputs
    // (note LCD_RST bit requires unlock, refer to lab 1)

    // CS

    GPIOA->DIR |= LCD_CS;
    GPIOA->DEN |= LCD_CS;

    // MOSI and CLK

    GPIOB->DEN   |= LCD_MOSI | LCD_SCK;
    GPIOB->AFSEL |= LCD_MOSI | LCD_SCK;

    GPIOB->PCTL  &= ~(GPIO_PCTL_PB7_M | GPIO_PCTL_PB4_M);
    GPIOB->PCTL  |= GPIO_PCTL_PB7_SSI2TX | GPIO_PCTL_PB4_SSI2CLK;

    // DC and RST

    GPIOF->LOCK = GPIO_LOCK_KEY;
    GPIOF->CR |= LCD_RST;

    GPIOF->DIR |= (LCD_RST | LCD_DC);
    GPIOF->DEN |= (LCD_RST | LCD_DC);

    SSI2->CR1 &= ~SSI_CR1_SSE;

    // configure the SPI interface (data bits, polarity, phase) to match the LCD
    // use an 8 MHz SPI clock

    SSI2->CC |= SSI_CC_CS_SYSPLL;
    SSI2->CR0 &= ~(SSI_CR0_SCR_M | SSI_CR0_SPO | SSI_CR0_SPH);
    SSI2->CR0 |= (4 << SSI_CR0_SCR_S) | SSI_CR0_DSS_8 | SSI_CR0_FRF_MOTO;

    // enable MOSI & SCK signals on SSI2
    SSI2->CR1 &= ~SSI_CR1_MS;

    SSI2->CPSR = 2;
    SSI2->CR1 |= SSI_CR1_SSE;

    // activate LCD_CS (active low, and can remain active as there is only one slave)
    GPIOA->DATA &= ~LCD_CS;

    // Assert LCD reset signal
    GPIOF->DATA &= ~LCD_RST;
    __delay_cycles(_50ms);
    GPIOF->DATA |= LCD_RST;
    __delay_cycles(_120ms);

    // Exit sleep mode
    lcdWrite(CM_SLPOUT, isCMD);
    __delay_cycles(_120ms);

    // Set the Gamma
    lcdWrite(CM_GAMSET, isCMD);
    lcdWrite(0x04, isDATA);

    // Set power control (GVDD / AVDD)
    lcdWrite(CM_SETPWCTR, isCMD);
    lcdWrite(0x0A, isDATA);
    lcdWrite(0x14, isDATA);

    // Set source driver bias current
    lcdWrite(CM_SETSTBA, isCMD);
    lcdWrite(0x0A, isDATA);
    lcdWrite(0x00, isDATA);

    // Set colour mode: 0x05 = 16-bit RGB565
    lcdWrite(CM_COLMOD, isCMD);
    lcdWrite(0x05, isDATA);
    __delay_cycles(_10ms);

    // Set memory access / scan direction
    lcdWrite(CM_MADCTL, isCMD);
    lcdWrite(CM_MADCTL_BGR, isDATA);

    // Normal display mode on
    lcdWrite(CM_NORON, isCMD);

    // clear the display
    __delay_cycles(_10ms);
    lcdClear(0);

    // turn on the display
    __delay_cycles(_10ms);
    lcdWrite(CM_DISPON, isCMD);
}


///////////////////////////////////////////////////////////////////////
// lcdClear - Clears the entire LCD display
// Arguments: colour - A 16 bit value to represent the RGB colour to be
//                     clear all pixels to
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdClear(unsigned int colour)
{
	// set x range
	lcdWrite(CM_CASET, isCMD);
	lcdWrite(0, isDATA);
	lcdWrite(X_CORRECTION_OFFSET, isDATA);
	lcdWrite(0, isDATA);
	lcdWrite(LCD_MAX_X + X_CORRECTION_OFFSET, isDATA);

	// set y range
    lcdWrite(CM_RASET, isCMD);
	lcdWrite(0, isDATA);
	lcdWrite(Y_CORRECTION_OFFSET, isDATA);
	lcdWrite(0, isDATA);
	lcdWrite(LCD_MAX_Y + Y_CORRECTION_OFFSET, isDATA);

	// clear all pixels in range
    lcdWrite(CM_RAMWR, isCMD);
    int row, col;
    for (row = 0; row <= LCD_MAX_Y; row++) {
        for (col = 0; col <= LCD_MAX_X; col++) {
        	// clear to desired colour
	        lcdWrite(colour >> 8, isDATA);
            lcdWrite(colour, isDATA);
        }
    }
}

///////////////////////////////////////////////////////////////////////
// lcdSetPixel - Sets the pixel colour at the specified display location.
//               The bottom left corner of the LCD is (0,0)
// Arguments: x - The horizontal location of the pixel
//            y - The vertical location of the pixel
//            colour - A 16 bit value to represent the RGB colour to be
//                     output at the pixel location
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdSetPixel(int x, int y, unsigned int colour) {

	// set x co-ordinate for the pixel
	lcdWrite(CM_CASET, isCMD);
	lcdWrite(0, isDATA);
	lcdWrite(x + X_CORRECTION_OFFSET, isDATA);
	lcdWrite(0, isDATA);
	lcdWrite(x + X_CORRECTION_OFFSET, isDATA);

	// set y co-ordinate for the pixel
    lcdWrite(CM_RASET, isCMD);
	lcdWrite(0, isDATA);
	lcdWrite(y + Y_CORRECTION_OFFSET, isDATA);
	lcdWrite(0, isDATA);
	lcdWrite(y + Y_CORRECTION_OFFSET, isDATA);

	// output colour to the pixel location
    lcdWrite(CM_RAMWR, isCMD);
    lcdWrite(colour >> 8, isDATA);
    lcdWrite(colour, isDATA);
}

///////////////////////////////////////////////////////////////////////
// lcdWrite - Sends a command/data byte to the LCD.
// Arguments: byte - value to be sent to the LCD
//            isData - true if byte is data, false if byte is a command
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdWrite(unsigned char byte, int isData)
{
    // wait until SPI finished transmitting previous byte
    // (check SSI Busy Bit in status register)
     while (SSI2->SR & SSI_SR_BSY) { continue; }

    // set Data/Command pin
     if (isData) { GPIOF->DATA |=  LCD_DC;  }
     else        { GPIOF->DATA &= ~LCD_DC; }

    // Transmit data/command
     SSI2->DR = byte;
}


///////////////////////////////////////////////////////////////////////
// lcdFillRect - Fills a rectangular region with a solid colour.
// Arguments: x      - left edge of the rectangle (0-based)
//            y      - top edge of the rectangle (0-based)
//            w      - width in pixels
//            h      - height in pixels
//            colour - 16-bit RGB565 colour value
// Return Value: none
///////////////////////////////////////////////////////////////////////
void lcdFillRect(int x, int y, int w, int h, unsigned int colour)
{
    // set x window
    lcdWrite(CM_CASET, isCMD);
    lcdWrite(0, isDATA);
    lcdWrite(x + X_CORRECTION_OFFSET, isDATA);
    lcdWrite(0, isDATA);
    lcdWrite(x + w - 1 + X_CORRECTION_OFFSET, isDATA);

    // set y window
    lcdWrite(CM_RASET, isCMD);
    lcdWrite(0, isDATA);
    lcdWrite(y + Y_CORRECTION_OFFSET, isDATA);
    lcdWrite(0, isDATA);
    lcdWrite(y + h - 1 + Y_CORRECTION_OFFSET, isDATA);

    // stream all pixels in one burst
    lcdWrite(CM_RAMWR, isCMD);
    int i;
    for (i = 0; i < w * h; i++)
    {
        lcdWrite(colour >> 8, isDATA);
        lcdWrite(colour,      isDATA);
    }
}


// end file: lcd.c
