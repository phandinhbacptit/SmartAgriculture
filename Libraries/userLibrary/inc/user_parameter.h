#ifndef _USER_PARAMETER_H_
#define _USER_PARAMETER_H_

/**************** General Define *****************************/
#define HIGH 											1
#define LOW												0
#define TIME_CHECK								10
/******************** Define Input ************************/
#define IN_PORT1	  							GPIOA
#define INPUT1		  							GPIO_Pin_7

#define IN_PORT2	  							GPIOB
#define INPUT2		  							GPIO_Pin_0

#define IN_PORT3	  							GPIOB
#define INPUT3		  							GPIO_Pin_1

#define IN_PORT4	  							GPIOB
#define INPUT4		  							GPIO_Pin_10

#define IN_PORT5	  							GPIOB
#define INPUT5		  							GPIO_Pin_11

#define IN_PORT6	  							GPIOA
#define INPUT6		  							GPIO_Pin_8

#define IN_PORT7	  							GPIOB
#define INPUT7		  							GPIO_Pin_15

#define IN_PORT8	  							GPIOB
#define INPUT8		  							GPIO_Pin_14

#define IN_PORT9	  							GPIOB
#define INPUT9		  							GPIO_Pin_13

#define IN_PORT10	  							GPIOB
#define INPUT10		  							GPIO_Pin_12

/************Define input in Auto mode *********************/							
#define MODE_PORT 								IN_PORT10
#define M_AUTO	    							INPUT9
#define M_MANUAL									INPUT10
							
#define WATER_PORT  							IN_PORT8
#define W_LEVEL_A   							INPUT7
#define W_LEVEL_B   							INPUT8
				
//#define THRESH_A_PORT 						IN_PORT6
//#define IN_A 											INPUT6
//				
//#define THRESH_B_PORT 						IN_PORT5
//#define IN_B 											INPUT5
				
#define M1_PORT										IN_PORT4
#define IN_M1											INPUT4
				
#define M2_PORT										IN_PORT3	
#define IN_M2 										INPUT3

#define HUMID_PORT  							IN_PORT1
#define IN_HUMID   								INPUT1
				

/** Keyboard **/

#define KEYBOARD_PORT				      GPIOA
#define MODE_PIN									GPIO_Pin_6
#define ENTER_PIN									GPIO_Pin_5
#define UP_PIN			  						GPIO_Pin_4
#define DOWN_PIN									GPIO_Pin_3		


/************Define input in Manual mode *******************/
#define M_VAN1_PORT								IN_PORT2
#define M_VAN1_PIN						    INPUT2

#define M_VAN2_PORT								IN_PORT3
#define M_VAN2_PIN						    INPUT3

#define M_BOM1_PORT								IN_PORT4
#define M_BOM1_PIN						    INPUT4

#define M_BOM2_PORT								IN_PORT5
#define M_BOM2_PIN						    INPUT5

#define M_DC_PORT									IN_PORT6
#define M_DC_PIN						    	INPUT6

/***************** Define Output ***************************/

#define VAN1_PORT									GPIOA
#define VAN1_PIN									GPIO_Pin_15
					
#define VAN2_PORT									GPIOB
#define VAN2_PIN									GPIO_Pin_3
				
#define BOM1_PORT									GPIOB
#define BOM1_PIN									GPIO_Pin_4
							
#define BOM2_PORT									GPIOB
#define BOM2_PIN									GPIO_Pin_5
							
#define DC_PORT										GPIOB
#define DC_PIN										GPIO_Pin_6
							
#define DP1_PORT									GPIOB
#define DP1_PIN										GPIO_Pin_7
							
#define DP2_PORT									GPIOB
#define DP2_PIN										GPIO_Pin_8
							
#define DP3_PORT									GPIOB
#define DP3_PIN										GPIO_Pin_9

///** Led 7 Segment ****/

#define HC595_PORT 								GPIOA
#define HC595_SCK									GPIO_Pin_2
#define HC595_LATCH								GPIO_Pin_1
#define HC595_DATA								GPIO_Pin_0

/********************** Define Device *****************************/
#define VAN1											1
#define VAN2 											2
#define BOM1											3
#define BOM2											4
#define DC												5
#define DP1												6
#define DP2												7
#define DP3												8
								
#define MAX_TIME 								  99   // minutes units

/********************* Define mode opreation of system *************************/
//#define IDEAL_STATE								0
//#define AUTO_MODE									1
//#define MANUAL_MODE 							2


#define WATER											0
#define IDEAL_STATE								1
#define TIMESET										2
#define FILL_WATER								3
#define RUNMOTOR									4
#define NUTRIENT  								5   // Tuoi chat dinh duong

/**************Define symbol function of HC595 *********************/
#define DATA											1
#define SCLK											2
#define LATCH											3

/***********************Define  Led display***************************************/

#define A													0x90
#define U													0xc1
#define M	   											0xd0
#define STOP 											0xbf
#endif
