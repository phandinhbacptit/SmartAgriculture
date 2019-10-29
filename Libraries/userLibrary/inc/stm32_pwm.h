#include "main.h"
#include "stm32f10x.h"
//#include "stm32f10x_adc.h"
//#include <my_library/stm32_lcd.h>
//#include <my_library/stm32_adc.h>


/****************************************************************************

	Ham cau hinh cac kenh cua bo Timer:
	Timer1 :				Timer2 : 			Timer3 : 			Timer4 :
	CH1 : PA8				CH1 : PA0 		CH1 : PA6			CH1 : PB6
	CH2 : PA9				CH2 : PA1			CH2 : PA7			CH2 :	PB7
	CH3 : PA10			CH3 : PA2			CH3 : PB0			CH3 : PB8
	CH4 : PA11 			CH4 : PA3			CH4 : PB1			CH4 : PB9
	
	voi GPIO_Pin = GPIO_Pin_x|GPIO_Pin_y|GPIO_Pin_z|GPIO_Pin_t;

****************************************************************************/

void GPIO_Configuration(GPIO_TypeDef* GPIOx, int GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/* -----------------------------------------------------------------------------------------------------
	 *
	 * TIMx Configuration: generate 1 PWM signals:
	 * APB1 prescaler = 72 # 1 => TIMxCLK = PCLK1x2 = 2 MHz, Prescaler = 0x0 => TIMx counter clock = 2 MHz
	 * TIMx ARR Register = 4096 => TIMx Frequency = TIMx counter clock/(ARR + 1)
	 * TIMx Frequency = 488.1620698Hz.
	 * TIMx Channel2 duty cycle = (TIMx_CCR2/ TIMx_ARR)* 100 = 50%	
	 * TIMx Frequency = TIMx counter clock/(TIM_Period + 1)
	 * Prescaler la gia tri de chia tu tan so he thong thanh gia tri can lay dua vao co timer
	 * Period la gia tri cua 100% xung PWM tao ra duoc va ti le phan tram xung (%) = (gia tri tu thanh ghi CCR)/Period
------------------------------------------------------------------------------------------------------ */

void PWM_timer_init(TIM_TypeDef* TIMx, int Period, int Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;   // Gia tri nap vao thanh ghi CCR
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	TIM_OC3Init(TIMx, &TIM_OCInitStructure);
	TIM_OC4Init(TIMx, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);		
	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);		

	TIM_ARRPreloadConfig(TIMx, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIMx, ENABLE);
}
void PWM_CH1(TIM_TypeDef* TIMx, uint16_t duty_cycle)
{
	TIMx->CCR1 = duty_cycle;	
}

void PWM_CH2(TIM_TypeDef* TIMx, uint16_t duty_cycle)
{
	TIMx->CCR2 = duty_cycle;	
}

void PWM_CH3(TIM_TypeDef* TIMx, uint16_t duty_cycle)
{
	TIMx->CCR3 = duty_cycle;	
}

void PWM_CH4(TIM_TypeDef* TIMx, uint16_t duty_cycle)
{
	TIMx->CCR4 = duty_cycle;	
}


/*
int main()
{
	uint16_t adc_value;
	uint8_t clr_screen;
	float cycle_percent;
	adc_init(GPIOA, GPIO_Pin_1);
	lcd_init();	
	GPIO_Configuration(GPIOA, GPIO_Pin_6|GPIO_Pin_7);
  PWM_timer_init(TIM3, 4096, 72);
	while(1)
	{
		adc_value = adc_read(1);
		PWM_CH2(TIM3, adc_value);
				
		cycle_percent = (adc_value*100)/4096;
		lcd_gotoxy(1,0);
		lcd_puts("ADC: ");
		lcd_gotoxy(1,5);
		lcd_display_real(adc_value);
		lcd_gotoxy(2,0);
		lcd_puts("Percent: ");
		lcd_gotoxy(2,9);
		lcd_display_real(cycle_percent);
		
		clr_screen++;
		if(clr_screen==1)
		{
			clr_screen=0;
			lcd_command(0x01);
		}	
	}
}
*/
