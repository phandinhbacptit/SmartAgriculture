#include "stm32_io.h"
#include "user_parameter.h"

void gpio_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	
	  /******Init Input********/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
		GPIO_InitStructure.GPIO_Pin = INPUT1;
		GPIO_Init(IN_PORT1, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT2;
		GPIO_Init(IN_PORT2, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT3;
		GPIO_Init(IN_PORT3, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT4;
		GPIO_Init(IN_PORT4, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT5;
		GPIO_Init(IN_PORT5, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT6;
		GPIO_Init(IN_PORT6, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT7;
		GPIO_Init(IN_PORT7, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT8;
		GPIO_Init(IN_PORT8, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT9;
		GPIO_Init(IN_PORT9, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = INPUT10;
		GPIO_Init(IN_PORT10, &GPIO_InitStructure);

		
		 /********Init pin for keyboard **********/
		GPIO_InitStructure.GPIO_Pin = MODE_PIN;
		GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = ENTER_PIN;
		GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = UP_PIN;
		GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DOWN_PIN;
		GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);
		
	
	  /******Init Output********/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		
	  GPIO_InitStructure.GPIO_Pin = VAN1_PIN;
		GPIO_Init(VAN1_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = VAN2_PIN;
		GPIO_Init(VAN2_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = BOM1_PIN;
		GPIO_Init(BOM1_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = BOM2_PIN;
		GPIO_Init(BOM2_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DC_PIN;
		GPIO_Init(DC_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DP1_PIN;
		GPIO_Init(DP1_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DP2_PIN;
		GPIO_Init(DP2_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DP3_PIN;
   	GPIO_Init(DP3_PORT, &GPIO_InitStructure);
		
}
