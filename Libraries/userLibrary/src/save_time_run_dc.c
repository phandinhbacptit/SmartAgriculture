#include "save_time_run_dc.h"
#include "stm32_delay.h"
#include "STM32F1_my_FLASH.h"

uint32_t startAddress = 0x800F000;     //start address to write Temperature threshold top-below
uint32_t startAddressTimeRelay1 = 0x800F400; // start address to write new password

void save_time (uint8_t time)
{
	FLASH_Unlock();  //unlock flash writing
	FLASH_ErasePage(startAddress);
	
	writeFlash(0, time, startAddress);									                
	_delay_ms(200);
	
	FLASH_Lock();   //lock the flash for writing
}

char get_time(void)
{
	return (readFlash(0, startAddress));
}
