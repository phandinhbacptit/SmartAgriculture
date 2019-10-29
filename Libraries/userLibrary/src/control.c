#include "control.h"
#include "user_parameter.h"
#include "stm32_delay.h"
#include "save_time_run_dc.h"
#include "stdio.h"
#include "74hc595.h"

extern uint8_t system_state;
extern uint32_t time_run;
extern uint32_t time_check;
uint8_t disable_text = 0;

/*
* Brief		 : Turn off all device in system
* Parameter: None
* Return   : None          
*/
void stop_working(void)
{
		GPIO_WriteBit(VAN1_PORT, VAN1_PIN, Bit_RESET);
		GPIO_WriteBit(BOM1_PORT, BOM1_PIN, Bit_RESET);
		GPIO_WriteBit(VAN2_PORT, VAN2_PIN, Bit_RESET);
		GPIO_WriteBit(BOM2_PORT, BOM2_PIN, Bit_RESET);
		GPIO_WriteBit(DC_PORT,   DC_PIN,   Bit_RESET);
		GPIO_WriteBit(DP1_PORT,  DP1_PIN, Bit_RESET);
		GPIO_WriteBit(DP2_PORT,  DP2_PIN, Bit_RESET);
		GPIO_WriteBit(DP3_PORT,  DP3_PIN, Bit_RESET);
}

/*
* Brief		 : Turn on device selected
* Parameter: device - Device user want to turn on
* Return   : None          
*/
void turn_on(uint8_t device)
{
		switch (device) {
			case VAN1:
						GPIO_WriteBit(VAN1_PORT, VAN1_PIN, Bit_SET);
						break;
			case VAN2:
						GPIO_WriteBit(VAN2_PORT, VAN2_PIN, Bit_SET);		
						break;
			case BOM1:
						GPIO_WriteBit(BOM1_PORT, BOM1_PIN, Bit_SET);		
						break;
			case BOM2:
						GPIO_WriteBit(BOM2_PORT, BOM2_PIN, Bit_SET);		
						break;
			case DC:
						GPIO_WriteBit(DC_PORT,   DC_PIN,   Bit_SET);		
						break;
			case DP1:
						GPIO_WriteBit(DP1_PORT,  DP1_PIN, Bit_SET);		
						break;
			case DP2:
						GPIO_WriteBit(DP2_PORT,  DP2_PIN, Bit_SET);		
						break;
			case DP3:
						GPIO_WriteBit(DP3_PORT,  DP3_PIN, Bit_SET);			
						break;
			default: 
						break;
		}
}

/*
* Brief		 : Turn off device selected
* Parameter: device - Device user want to turn off
* Return   : None          
*/
void turn_off(uint8_t device)
{
	switch (device) {
			case VAN1:
				GPIO_WriteBit(VAN1_PORT, VAN1_PIN, Bit_RESET);		
						break;
			case VAN2:
						GPIO_WriteBit(VAN2_PORT, VAN2_PIN, Bit_RESET);		
						break;		
			case BOM1:
						GPIO_WriteBit(BOM1_PORT, BOM1_PIN, Bit_RESET);		
						break;
			case BOM2:
						GPIO_WriteBit(BOM2_PORT, BOM2_PIN, Bit_RESET);		
						break;
			case DC:
						GPIO_WriteBit(DC_PORT,   DC_PIN,   Bit_RESET);		
						break;
			case DP1:
						GPIO_WriteBit(DP1_PORT,  DP1_PIN, Bit_RESET);		
						break;
			case DP2:
						GPIO_WriteBit(DP2_PORT,  DP2_PIN, Bit_RESET);		
						break;
			case DP3:
						GPIO_WriteBit(DP3_PORT,  DP3_PIN, Bit_RESET);			
						break;
			default:
						break;
		}
}

/*
* Brief		 : Get action when system in ideal state
* Parameter: None
* Return   : None          
*/
void ideal_state(void)
{
			stop_working();
	
		 if (GPIO_ReadInputDataBit(M2_PORT, IN_M2) == 0) {
			  _delay_ms(1);
        system_state = FILL_WATER;
		 }
		 
		 if (GPIO_ReadInputDataBit(KEYBOARD_PORT, MODE_PIN) == 0) {
			  _delay_ms(1);
				system_state = TIMESET;
		 }
				 
}

/*
* Brief		 : Auto pouring water for plants depend on  humidity
* Parameter: None
* Return   : None          
*/
void auto_pour_water(void)
{
		 if (GPIO_ReadInputDataBit(HUMID_PORT, IN_HUMID) == 0) {
			  _delay_ms(1);
		 		turn_on(VAN1);
		 		turn_on(BOM1);
		 }
		 else {
		 		turn_off(VAN1);
		 		turn_off(BOM1);
		 }
		 
		 if (GPIO_ReadInputDataBit(M1_PORT, IN_M1) == 0) {
			  _delay_ms(1);
			  system_state = IDEAL_STATE;
			  turn_off(VAN1);
		 		turn_off(BOM1);
		 }
}

/*
* Brief		 : Fill water in jar to threshold B level
* Parameter: None
* Return   : None          
*/
void fill_water(void)
{
		 turn_on(BOM2);
		 turn_off(VAN2);
	  
		 if (GPIO_ReadInputDataBit(WATER_PORT, W_LEVEL_B) == 0) {
			  _delay_ms(1);
				system_state = IDEAL_STATE;
			 	turn_off(BOM2);
		 }
}


/*
* Brief		 : Setting up time rotate DC motor  
* Parameter: *count - pointer increase value when press up button
											pointer decrease value when press down button
* Ret			 : None
*/

void time_dc_run_setup (uint8_t *count, uint8_t max_val)
{
		if (GPIO_ReadInputDataBit(KEYBOARD_PORT, UP_PIN) == 0) {
				_delay_ms(200); 
				(*count)++;
		}
		else if(GPIO_ReadInputDataBit(KEYBOARD_PORT, DOWN_PIN) == 0) {
				_delay_ms(200);
			  (*count)--;
		}
	  printf("\n %d", *count);
		if(*count <= 0) 
				*count = 99;	
		else if(*count >= max_val) 
				*count = 0;
		disable_text = 2;
	  hc595_display_integer_numbers((*count));
	  if (GPIO_ReadInputDataBit(KEYBOARD_PORT, ENTER_PIN) == 0) {
				_delay_ms(200);
				system_state = RUNMOTOR;
				save_time(*count);
				printf("\n Run ");
		}
}

/*
* Brief		 : Running DC following time setup 
* Parameter: time_on - Time running DC
* Ret			 : None            
*/
void running_dc(uint32_t time_on)
{
	  if (time_on > 0) { 
			hc595_display_integer_numbers(((time_on / 60) + 1));
//				if (time_on < 60)
//						 hc595_display_integer_numbers(time_on);
//				else 
				
			turn_on(DC);
		}
		else {
			  disable_text = 0;
			  turn_off(DC);
				system_state = NUTRIENT;
			  _delay_ms(3000);
		}
}
/*
* Brief		 : Watering nutrients for plants
* Parameter: None
* Return   : None          
*/

void pour_nutrient(void)
{
			turn_on(VAN2);
			turn_on(BOM1);
			printf(" %d", time_check);
			if (GPIO_ReadInputDataBit(WATER_PORT, W_LEVEL_A) == 1) {
					_delay_ms(1);
				  if (time_check <=0) {
							turn_off(VAN2);
							turn_off(BOM1);
							system_state = WATER;
					}
			}
			else {
					time_check = TIME_CHECK;
			}
}
/*
* Brief		 : System working in Auto mode when switch in "auto"  _______auto     off    manual_____
*																																					\*			
*																																						\*			
*																																							\*	
*																																								\*
*																																								Switch
* Parameter: *time: user setup to run DC
* Return   : None          
*/
void auto_mode (uint8_t *time)
{
		switch (system_state) {
			case IDEAL_STATE: {
					//printf("\n IDEAL");
					ideal_state();
					break;
			}
			case TIMESET: {
					//printf("\n TIMESET");
					time_dc_run_setup(time, MAX_TIME);
					time_run = (*time) * 60;
					break;
			}
			case FILL_WATER: {
					//printf("\n FILL_WATER");
					fill_water();
					break;
			}
			case RUNMOTOR: {
					//printf("\n RUNMOTOR");
					running_dc(time_run);
					break;
			}
			case NUTRIENT: {
					//printf("\n NUTRIENT");
					pour_nutrient();
					break;
			}
			default: {
					//printf("\n AUTO");
					auto_pour_water();
					break;
			}
			
		}
}

/*
* Brief		 : System none working when switch in "auto"  _______auto     off    manual_____
*																																				|*
*																																				|*	
*																																				|*
*																																				|*
*																																			Switch
* Parameter: None
* Return   : None          
*/
void system_stop(void)
{
		stop_working();
}

/*
* Brief		 : System none working when switch in "auto"  _______auto     off    manual_____
*																																				      */
/*																																				   */	
/*																																				  */
/*																																				 */
/*																																			 Switch
* Parameter: None
* Return   : None          
*/
void manual_mode(void)
{
		if (GPIO_ReadInputDataBit(M_VAN1_PORT, M_VAN1_PIN) == 0) 
				turn_on(VAN1);
		else 
			  turn_off(VAN1);
		
		if (GPIO_ReadInputDataBit(M_VAN2_PORT, M_VAN2_PIN) == 0) 
				turn_on(VAN2);
		else 
			  turn_off(VAN2);
		
		if (GPIO_ReadInputDataBit(M_BOM1_PORT, M_BOM1_PIN) == 0) 
				turn_on(BOM1);
		else 
			  turn_off(BOM1);
		
		if (GPIO_ReadInputDataBit(M_BOM2_PORT, M_BOM2_PIN) == 0) 
				turn_on(BOM2);
		else 
			  turn_off(BOM2);
		
		if (GPIO_ReadInputDataBit(M_DC_PORT, M_DC_PIN) == 0) 
				turn_on(DC);
		else 
			  turn_off(DC);	
}
