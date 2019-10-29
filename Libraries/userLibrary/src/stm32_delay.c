#include "stm32_delay.h"

long sys_time = 0;
//double angle[];


/*Ham khoi tao de lay system clock*/
void sys_tick_init(void)
{
		if (SysTick_Config(SystemCoreClock / 1000)) // Tan so 1kHz cu sau 1ms thi tran 1 lan
		{ 		
				while (1)
				{}
		}
}

uint32_t milis(void)
{
		uint32_t ms;
		ms = sys_time;
		return ms;
}

void _delay_ms(uint32_t ms)
{
    uint32_t now = milis();
    while (milis() - now < ms);
}

