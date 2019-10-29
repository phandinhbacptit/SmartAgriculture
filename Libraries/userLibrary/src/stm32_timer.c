#include "stm32_timer.h"


void timer4_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	 /* Enable the TIM2 gloabal Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel                      = TIM4_IRQn;	 // Khoi tao vecto ngat timer2
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 1;			     // Muc uu tien cua ngat
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;		   // Cho phep ngat
  	NVIC_Init(&NVIC_InitStructure);

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	 // Enable Clock Timer2 on APB1 with frequency 36Mhz

	  /* -----------------------------------------------------------------------
	  APB1 prescaler = 36 # 1 => TIM2CLK = PCLK1x2 = 2 MHz, Prescaler = 0x0 => TIM2 counter clock = 2 MHz
   	----------------------------------------------------------------------- */

  	/* Time base configuration 1s */
  	TIM_TimeBaseStructure.TIM_Period        = 2000 - 1;   
  	TIM_TimeBaseStructure.TIM_Prescaler     = 36000 - 1; 
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;	 
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  	TIM_Cmd(TIM4, ENABLE);						  
}
