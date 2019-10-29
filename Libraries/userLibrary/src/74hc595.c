#include "74HC595.h"
#include "stm32_delay.h"
#include "user_parameter.h"


uint8_t sevenSegCodePos[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92,0x82, 0xF8, 0x80, 0x90}; //ma led 7
uint8_t seven_seg_code_new[10] = {0xc0, 0xf9, 0xa4, 0xa8, 0x99, 0x8a, 0x82, 0xf8, 0x80, 0x88};
uint8_t sevenSegDotCodePos[10] = {0x40, 0x79, 0x24, 0x30, 0x19, 0x12,0x02, 0x78, 0x00, 0x10}; //ma led 7
uint8_t sevenSegCALPos[3] = {0xC6, 0x88, 0xC7}; //ma led 7
uint8_t sevenSegRUNPos[3] = {0x88, 0xC1, 0xC8}; //ma led 7

void hc595_count_down(uint8_t _time);
void hc595_display_integer_numbers(uint16_t k);
void hc595_display_real_numbers(float k);

void delay_us(unsigned int time)
{
	 while(time --);
}

void hc595_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
		GPIO_InitStructure.GPIO_Pin = HC595_LATCH;
   	GPIO_Init(HC595_PORT, &GPIO_InitStructure);
		
	  GPIO_InitStructure.GPIO_Pin = HC595_DATA;
   	GPIO_Init(HC595_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = HC595_SCK;
   	GPIO_Init(HC595_PORT, &GPIO_InitStructure);
	
		GPIO_WriteBit(HC595_PORT, HC595_DATA, Bit_RESET);	
		GPIO_WriteBit(HC595_PORT, HC595_LATCH, Bit_SET);	
		GPIO_WriteBit(HC595_PORT, HC595_SCK, Bit_RESET);	
	
//		hc595_display_integer_numbers(0);
//		hc595_display_real_numbers(0.0);
//		hc595_count_down(3);
}
  
static void on_pin(uint8_t pin)
{
	switch (pin) {
			case DATA:
				GPIO_WriteBit(HC595_PORT, HC595_DATA, Bit_SET);		
						break;
			case SCLK:
						GPIO_WriteBit(HC595_PORT, HC595_SCK, Bit_SET);		
						break;		
			case LATCH:
						GPIO_WriteBit(HC595_PORT, HC595_LATCH, Bit_SET);		
						break;
			default:
						break;
		}
}

static void off_pin(uint8_t pin)
{
	switch (pin) {
			case DATA:
				GPIO_WriteBit(HC595_PORT, HC595_DATA, Bit_RESET);		
						break;
			case SCLK:
						GPIO_WriteBit(HC595_PORT, HC595_SCK, Bit_RESET);		
						break;		
			case LATCH:
						GPIO_WriteBit(HC595_PORT, HC595_LATCH, Bit_RESET);		
						break;
			default:
						break;
		}
}

void hc595_put_data(uint8_t x)
{
		uint8_t k = 0, i = 0;
		
		for(i = 0; i < 8; i++) {
			k = x;
			k = k & 0x80;
			if(k == 0x80)
				on_pin(DATA);
			else 
				off_pin(DATA);
			
			on_pin(SCLK);
			delay_us(5);
			off_pin(SCLK);
			x = x * 2;
		}
}

void hc595_count_down(uint8_t _time)
{
	uint8_t i = 0;
	
  for(i = _time; i > 0; i--)
  {
    hc595_display_integer_numbers(i);
    _delay_ms(1);
  }
}

void hc595_display_run(void)
{
  hc595_put_data(sevenSegRUNPos[2]);
  hc595_put_data(sevenSegRUNPos[1]);
  hc595_put_data(sevenSegRUNPos[0]);
  off_pin(LATCH);
	delay_us(5);
  on_pin(LATCH);
}

void hc595_display_cal(void)
{
  hc595_put_data(sevenSegCALPos[2]);
  hc595_put_data(sevenSegCALPos[1]);
  hc595_put_data(sevenSegCALPos[0]);
  off_pin(LATCH);
	delay_us(5);
  on_pin(LATCH);
}

void hc595_display_integer_numbers(uint16_t k)
{
  hc595_put_data(seven_seg_code_new[k % 10]);
  hc595_put_data(seven_seg_code_new[k / 10]);
  off_pin(LATCH);
	delay_us(5);
  on_pin(LATCH);
}

void hc595_display_text(uint8_t text1, uint8_t text2)
{
  hc595_put_data(text2);
  hc595_put_data(text1);
  off_pin(LATCH);
	delay_us(5);
  on_pin(LATCH);
}

void hc595_display_real_numbers(float k)
{
  uint16_t sumNum = k * 10;

  uint8_t intNum = sumNum / 10;
  uint8_t realNum = sumNum % 10;

  uint8_t tenth = intNum / 10;
  uint8_t unit = intNum % 10;
  
  hc595_put_data(sevenSegCodePos[realNum]);
  hc595_put_data(sevenSegDotCodePos[unit]);
  hc595_put_data(sevenSegCodePos[tenth]);
  off_pin(LATCH);
  delay_us(5);
  on_pin(LATCH);
}

