#include <stdio.h>
#include <string.h>
#include "stm32_usart.h"
#include "stm32_io.h"
#include "stm32_delay.h"
#include "stm32_timer.h"
#include "control.h"
#include "save_time_run_dc.h"
#include "user_parameter.h"
#include "74HC595.h"

//static void test_input(void);
extern uint8_t disable_text;

uint8_t time_set = 0;
extern uint32_t time_run;
/*
* Brief		 : Init watchdog reset system  when a problem occurs. The system will reset after 10 second hall system.
						The time_out is caculated: time_out = (4 * IWDG_RLR * 2^PR) / 40000 (40KHz-LSI)
*						 IWDG_Prescaler_4            => PR = 0x00
*            IWDG_Prescaler_8            => PR = 0x01
*            IWDG_Prescaler_16           => PR = 0x02
*            IWDG_Prescaler_32           => PR = 0x03
*            IWDG_Prescaler_64           => PR = 0x04
*            IWDG_Prescaler_128          => PR = 0x05
*            IWDG_Prescaler_256          => PR = 0x06

						Incase, timout = 4 * 1563 * 2^6  / 40000 = ~ 10 seconds
* Parameter: None
* Ret			 : None            
*/
void init_watchdog(void)
{
		/* Enable write access to IWDG_PR and IWDG_RLR registers */
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		
		/* IWDG counter clock: 40KHz(LSI) / 256 = ~156.25 Hz */
		IWDG_SetPrescaler(IWDG_Prescaler_256);
		
		/* Set counter reload value to 1563 */
		IWDG_SetReload(1563);
		
		/* Reload IWDG counter */
		IWDG_ReloadCounter();
		
		/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
		IWDG_Enable();
}

int count = 0;
int main(void)
{
	  sys_tick_init();
		//init_watchdog();
		usart1_init(GPIOA, GPIO_Pin_9, GPIO_Pin_10, 115200);
		timer4_init();
		gpio_init(); 
		hc595_init();
	  time_set = get_time();
	  stop_working();
	  //printf("\n Start");
		while(1) {	
			if (GPIO_ReadInputDataBit(MODE_PORT, M_AUTO) == 0) {
					auto_mode(&time_set);
				  if (disable_text == 0)
						hc595_display_text(A, U);
			}
			else if (GPIO_ReadInputDataBit(MODE_PORT, M_MANUAL) == 0) {
					manual_mode();
					hc595_display_text(M, M);
			}
			else {
					system_stop();
					hc595_display_text(STOP, STOP);
			}
			delay_ms(50000);
			//IWDG_ReloadCounter(); 
		}
}




//static void test_input(void)
//{
//			  if (GPIO_ReadInputDataBit(WATER_PORT, W_LEVEL_A) == 0) {
//						_delay_ms(1);
//					  printf (" \n W_LEVEL_A");
//		    }
//				if (GPIO_ReadInputDataBit(WATER_PORT, W_LEVEL_B) == 0) {
//						_delay_ms(1);
//					  printf (" \n W_LEVEL_B");
//		    }			  
//				if (GPIO_ReadInputDataBit(MODE_PORT, M_AUTO) == 0) {
//						_delay_ms(1);
//					  printf (" \n M_AUTO");
//		    }			 
//				if (GPIO_ReadInputDataBit(MODE_PORT, M_MANUAL) == 0) {
//						_delay_ms(1);
//					  printf (" \n M_MANUAL");
//		    }			  
//				if (GPIO_ReadInputDataBit(HUMID_PORT, IN_HUMID) == 0) {
//						_delay_ms(1);
//					  printf (" \n IN_HUMID");
//		    }			  
//				if (GPIO_ReadInputDataBit(THRESH_A_PORT, IN_A) == 0) {
//						_delay_ms(1);
//					  printf (" \n IN_A");
//		    }			  
//				if (GPIO_ReadInputDataBit(THRESH_B_PORT, IN_B) == 0) {
//						_delay_ms(1);
//					  printf (" \n IN_B");
//		    }			  
//				if (GPIO_ReadInputDataBit(M1_PORT, IN_M1) == 0) {
//						_delay_ms(1);
//					  printf (" \n IN_M1");
//		    }			  
//				if (GPIO_ReadInputDataBit(M2_PORT, IN_M2) == 0) {
//						_delay_ms(1);
//					  printf (" \n IN_M2");
//		    }			  
//				if (GPIO_ReadInputDataBit(IN_PORT6, INPUT6) == 0) {
//						_delay_ms(1);
//					  printf (" \n INPUT6");
//				}		

//				
//				if (GPIO_ReadInputDataBit(KEYBOARD_PORT, MODE_PIN) == 0) {
//						_delay_ms(1);
//					  printf (" \n MODE_PIN");
//		    }			  
//				if (GPIO_ReadInputDataBit(KEYBOARD_PORT, ENTER_PIN) == 0) {
//						_delay_ms(1);
//					  printf (" \n ENTER_PIN");
//		    }
//			 	if (GPIO_ReadInputDataBit(KEYBOARD_PORT, UP_PIN) == 0) {
//						_delay_ms(1);
//					  printf (" \n UP_PIN");
//		    }			  
//				if (GPIO_ReadInputDataBit(KEYBOARD_PORT, DOWN_PIN) == 0) {
//						_delay_ms(1);
//					  printf (" \n DOWN_PIN");
//		    }
//			  _delay_ms(500);
//}
