#ifndef __74HC595_H__
#define __74HC595_H__

#include "stm32f10x.h"

void hc595_init(void);
void hc595_display_run(void);
void hc595_display_cal(void);
void hc595_display_integer_numbers(uint16_t k);
void hc595_display_real_numbers(float k);
void hc595_put_data(uint8_t x);
void Show(unsigned char x);
void hc595_display_text(uint8_t text1, uint8_t text2);

#endif

