#ifndef _STM32_IO_H_
#define _STM32_IO_H_

#include "stm32f10x.h"

void gpio_init(void);
void turn_on(uint8_t device);
void turn_off(uint8_t device);


#endif /* _STM32_IO_H_ */
