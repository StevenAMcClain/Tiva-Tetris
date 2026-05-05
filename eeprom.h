///////////////////////////////////////////////
// File: eeprom.h
//
//  Project: TETRIS
//  Originally by: Aleksander Olesevitch  (Oct 7, 2017)
//  Restructured and
//    updated for Tiva by: Steven McClain (Apr. 12, 2026)
//
#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_MAX_ADDRESS 0x7FF        // 2047

extern void EEPROM_Initialize(void);

extern void EEPROM_Write(uint16_t /*address*/, uint32_t /*data*/);

extern uint32_t EEPROM_Read(uint16_t /*address*/);

#endif /* EEPROM_H_ */
