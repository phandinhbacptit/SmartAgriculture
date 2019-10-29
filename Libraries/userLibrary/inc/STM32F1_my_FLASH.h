#ifndef __STM32F1_my_FLASH_H__
#define __STM32F1_my_FLASH_H__

#include "stm32f10x.h"


#define FLASH_PAGE_SIZE  ((uint16_t)0x800)

#define CHIP_ID_ADDRESS  0x1FFFF7E8         //Read page 1079 of the STM32 Reference Manual

void Flash_Erase(uint32_t StartEraseAddress, uint32_t EndEraseAddress);
void Flash_WriteByte(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint8_t FLS_Data);
void Flash_WriteHalfWord(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint16_t FLS_Data);
void Flash_WriteWord(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint32_t FLS_Data);
uint8_t Flash_ReadByte(uint32_t StartReadAddress);
uint16_t Flash_ReadHalfWord(uint32_t StartReadAddress);
uint32_t Flash_ReadWord(uint32_t StartReadAddress);
char readFlash(uint8_t add, uint32_t address);
void writeFlash(uint8_t add, uint8_t val, uint32_t address);

#endif
