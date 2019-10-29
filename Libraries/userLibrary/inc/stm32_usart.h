#ifndef _STM32_USART_H_
#define _STM32_USART_H_

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include <stdio.h>

void delay_ms(unsigned long int t);

int usart1_send_data (int ch);
int usart1_get_data(void); 

int usart2_send_data (int ch);
int usart2_get_data(void)  ;
 
int usart3_send_data (int ch);
int usart3_get_data(void);

void usart1_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate);
void usart2_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate);
void usart3_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate);

#endif

