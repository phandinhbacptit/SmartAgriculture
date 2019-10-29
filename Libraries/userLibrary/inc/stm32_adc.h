#include "main.h"
#include "stm32f10x.h"
//#include <stdio.h>
//#include <my_library/stm32_lcd.h>


GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

void Delay(__IO uint32_t num)
{
	__IO uint32_t index = 0;

	/* default system clock is 72MHz */
	for(index = (720000 * num); index != 0; index--)
	{
	}
}
/*****************************************************************
Ham khoi tao cho bo ADC voi cac tham so sau:

ADCx : chon bo ADCx (x=1,2,3)
GPIO_x : chon dau vao tren Port_x (GPIOA, GPIOB, GPIOC...)
Pin_adc: chon dau vao tai chan so may (GPIO_Pin_1, GPIO_Pin_2....)
channel_x: chon kenh adc tren bo ADCx duoc chon
rank : chon vi tri cua adc

*****************************************************************/


void adc_init(GPIO_TypeDef* GPIO_x, int Pin_adc)
{
	/* Configure PA.01 (ADC Channel2) as analog input -------------------------*/
  	GPIO_InitStructure.GPIO_Pin = Pin_adc;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  	GPIO_Init(GPIO_x, &GPIO_InitStructure);

	/* Configure clocks for ADC and GPIO PORT */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* ADCx configuration ------------------------------------------------------*/
  	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfChannel = 1;
  	ADC_Init(ADC1, &ADC_InitStructure);	
	
  	/* Enable ADC1 */
  	ADC_Cmd(ADC1, ENABLE);

	ADC_TempSensorVrefintCmd(ENABLE);

	/* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);

    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1)); 
}

uint16_t adc_read(uint8_t channel)
{
	/* ADC1 Regular Channel 1 Configuration */
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_55Cycles5);	
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}

/*
int main()
{
	char clr_screen=0;
	float adc_value = 0, adc_value_1=0;
	float V_in=0;
	
	lcd_init();
	adc_init(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	while(1)
	{
		adc_value = adc_read(2);
		adc_value_1 = adc_read(1);

		V_in = (adc_value*3.3)/4095;

		lcd_gotoxy(1,0);
		printf("ADC_2: %0.0f",adc_value);
		lcd_gotoxy(2,0);
		printf("ADC_1: %0.0f",adc_value_1);

		clr_screen++;
		if(clr_screen==3)
		{
			clr_screen=0;
			lcd_command(0x01);
		}
	}
}
*/

