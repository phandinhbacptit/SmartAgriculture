#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"

void stop_working(void);
void turn_on(uint8_t device);
void turn_off(uint8_t device);
void ideal_state(void);
void auto_pour_water(void);
void fill_water(void);
void time_dc_run_setup (uint8_t *count, uint8_t max_val);
void running_dc(uint32_t time_on);
void pour_nutrient(void);


void auto_mode (uint8_t *time);
void system_stop(void);
void manual_mode(void);

#endif /* _STM32_TIMER_H */
