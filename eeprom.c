///////////////////////////////////////////////
// File: joystick.c
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//

#include "common.h"
#include "eeprom.h"


PUBLIC void EEPROM_Initialize(void)
{
    while ((SYSCTL->PREEPROM & SYSCTL_PREEPROM_R0) == 0) { continue; }  // wait for peripheral ready
    while (EEPROM->EEDONE & EEPROM_EEDONE_WORKING) { continue; }        // wait for init to complete
}


PUBLIC void EEPROM_Write(uint16_t address, uint32_t data)
{
    if (address <= EEPROM_MAX_ADDRESS)
    {
        EEPROM->EEBLOCK = address >> 4;
        EEPROM->EEOFFSET = address & 0xF;
        EEPROM->EERDWR = data;

        while (EEPROM->EEDONE & EEPROM_EEDONE_WORKING) { continue; }
    }
}


PUBLIC uint32_t EEPROM_Read(uint16_t address)
{
    EEPROM->EEBLOCK = address >> 4;
    EEPROM->EEOFFSET = address & 0xF;

    return EEPROM->EERDWR;
}


// End file: eeprom.c
