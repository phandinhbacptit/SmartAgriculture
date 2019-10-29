#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "STM32F1_my_FLASH.h"

void Flash_Erase(uint32_t StartEraseAddress, uint32_t EndEraseAddress)
{
		volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
		uint32_t NbrOfPage = 0x00;
		uint32_t EraseCounter = 0;
		
		FLASH_Unlock();
		NbrOfPage = (EndEraseAddress - StartEraseAddress)/FLASH_PAGE_SIZE;
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
		for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
		{
				FLASHStatus = FLASH_ErasePage(StartEraseAddress + (FLASH_PAGE_SIZE * EraseCounter));
		}
		FLASH_Lock();
}

void Flash_WriteByte(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint8_t FLS_Data)
{
		volatile FLASH_Status FLASHStatus  = FLASH_COMPLETE;
		
		FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);

		while((StartWriteAddress < EndWriteAddress) && (FLASHStatus == FLASH_COMPLETE))
		{
				FLASHStatus = FLASH_ProgramWord(StartWriteAddress, FLS_Data);
				StartWriteAddress = StartWriteAddress + 1;
		}
		FLASH_Lock();
}

void Flash_WriteHalfWord(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint16_t FLS_Data)
{
		volatile FLASH_Status FLASHStatus  = FLASH_COMPLETE;
		
		FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);

		while((StartWriteAddress < EndWriteAddress) && (FLASHStatus == FLASH_COMPLETE))
		{
				FLASHStatus = FLASH_ProgramWord(StartWriteAddress, FLS_Data);
				StartWriteAddress = StartWriteAddress + 2;
		}
		FLASH_Lock();
}

void Flash_WriteWord(uint32_t StartWriteAddress, uint32_t EndWriteAddress, uint32_t FLS_Data)
{
		volatile FLASH_Status FLASHStatus  = FLASH_COMPLETE;
		
		FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);

		while((StartWriteAddress < EndWriteAddress) && (FLASHStatus == FLASH_COMPLETE))
		{
				FLASHStatus = FLASH_ProgramWord(StartWriteAddress, FLS_Data);
				StartWriteAddress = StartWriteAddress + 4;
		}
		FLASH_Lock();
}

uint8_t Flash_ReadByte(uint32_t StartReadAddress)
{
		uint8_t temp;  
		FLASH_Unlock();
		temp = *(uint8_t*)(StartReadAddress);
		FLASH_Lock();
		return temp;
}

uint16_t Flash_ReadHalfWord(uint32_t StartReadAddress)
{
		uint16_t temp;  
		FLASH_Unlock();
		temp = *(uint16_t*)(StartReadAddress);
		FLASH_Lock();
		return temp;
}

uint32_t Flash_ReadWord(uint32_t StartReadAddress)
{
		uint32_t temp;  
		FLASH_Unlock();
		temp = *(uint32_t*)(StartReadAddress);
		FLASH_Lock();
		return temp;
}

char readFlash(uint8_t add, uint32_t address)
{
  char x;	
	uint16_t raw;	      
       raw = *(uint16_t *)(address + add*2);
	     x = (char) raw;
	return x;
}

void writeFlash(uint8_t add, uint8_t val, uint32_t address)
{
  uint16_t val16;
	val16 =  val;
	
       FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
       FLASH_ProgramHalfWord((address + add*2),val16); 
}	
