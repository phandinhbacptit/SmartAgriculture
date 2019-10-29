#ifndef _STM32_DELAY_H_
#define _STM32_DELAY_H_

#include "stm32f10x.h"

void sys_tick_init(void);
void _delay_ms(uint32_t ms);

#endif /*_STM32_DELAY_H_*/
