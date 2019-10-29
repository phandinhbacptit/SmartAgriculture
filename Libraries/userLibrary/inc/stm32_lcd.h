#include "stm32f10x.h"
#include <stm32f10x_gpio.h>


#define RS_PORT     GPIOC
#define RS_CLK      RCC_APB2Periph_GPIOC 
#define RS_PIN      GPIO_Pin_12

#define RW_PORT     GPIOC
#define RW_CLK      RCC_APB2Periph_GPIOC 
#define RW_PIN      GPIO_Pin_11

#define EN_PORT     GPIOC
#define EN_CLK      RCC_APB2Periph_GPIOC 
#define EN_PIN      GPIO_Pin_10

#define SET_RS()	GPIO_SetBits(RS_PORT, RS_PIN)
#define CLR_RS()	GPIO_ResetBits(RS_PORT, RS_PIN)
#define SET_RW()	GPIO_SetBits(RW_PORT, RW_PIN)
#define CLR_RW()	GPIO_ResetBits(RW_PORT, RW_PIN)
#define SET_EN()	GPIO_SetBits(EN_PORT, EN_PIN)
#define CLR_EN()	GPIO_ResetBits(EN_PORT, EN_PIN)

#define LCD_D4      GPIO_Pin_0
#define LCD_D5      GPIO_Pin_1
#define LCD_D6      GPIO_Pin_2
#define LCD_D7      GPIO_Pin_3
#define DATA_PINS   (LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define DATA_PORT 	GPIOC
#define DATA_CLK	  RCC_APB2Periph_GPIOC


/*
extern int  lcd_putc(int ch);

struct __FILE 
{
  int handle;                 // Add whatever you need here 
};
FILE __stdout;
FILE __stdin;


int fputc(int ch, FILE *f) 
{
    return (lcd_putc(ch));
}
*/


void delay(long int t)
{
	long i = 0;
	for(i = (10100 * t); i!= 0; i--)
	{}
}

void lcd_write_4bit(unsigned char data)
{
	// Write only the highest 4 bits!
	SET_EN() ;// Disable LCD 
	GPIO_Write(DATA_PORT, ( GPIO_ReadOutputData(DATA_PORT) & 0xFFF0 ) |(u16) ((data >> 4)&0x000F));
	CLR_EN() ; // Enable LCD, Create a falling edge
	delay(2);
	SET_EN() ;// Disable LCD 
	GPIO_Write(DATA_PORT, (GPIO_ReadInputData(DATA_PORT)&0xFFF0) |(u16)(data&0x000F));
	CLR_EN() ; // Enable LCD, create a falling edge
	delay(1);
}

/* GPIO configuration mode */
typedef enum 
{
   	Input = 0, Output=1
} DATA_mode;


void lcd_data_mode(DATA_mode IO_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;										  	
																			
	/* Enable the LCD Data line Clock */											
  	RCC_APB2PeriphClockCmd(DATA_CLK, ENABLE);					
																			
	/* Configure the LCD Data lines */											
  	GPIO_InitStructure.GPIO_Pin = DATA_PINS;  	 					
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	if( IO_mode == Input )
		{							
       	/* Configure LCD D0~D7 lines as Input */									
            	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;				
      	}																		
    else if ( IO_mode == Output)						
		{																		
            /* Configure LCD D0~D7 lines in Output Push-Pull mode */					
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						
      	}																		
																			
  	GPIO_Init(DATA_PORT, &GPIO_InitStructure);	 					
}

void lcd_command(uint8_t command)
{
	CLR_RW();
	CLR_RS();
	lcd_data_mode(Output);
	lcd_write_4bit(command);
	lcd_data_mode(Input);
	delay(1);
}

void lcd_putc(char ascii)
{
	CLR_RW();
	SET_RS();
	lcd_data_mode(Output);
	lcd_write_4bit(ascii);
	lcd_data_mode(Input);
	delay(1);
}

void lcd_puts(char *lcd_string)
{
	while(*lcd_string)
	{
		lcd_putc(*lcd_string++);
		delay(1);
	}
}

void lcd_init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clocks for LCD control pins */
	RCC_APB2PeriphClockCmd(RS_CLK | RW_CLK | EN_CLK, ENABLE);

	/* initialize LCD control lines to output */
	GPIO_InitStructure.GPIO_Pin = RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = RW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RW_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(EN_PORT, &GPIO_InitStructure); 

	/* Enable GPIO clocks for LCD data pins */
	RCC_APB2PeriphClockCmd(DATA_CLK, ENABLE);

	/* initialize LCD data port to output */
	GPIO_InitStructure.GPIO_Pin = DATA_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA_PORT, &GPIO_InitStructure); 	

	lcd_command(0x33);
	delay(10);
	lcd_command(0x32);//Dua con tro ve dau dong
	delay(10);
	lcd_command(0x28);
	lcd_command(0x0C);
	lcd_command(0x01);
}

void lcd_gotoxy(char x, char y)
{
	char xy; 
	if(x==1)
	{
		xy=y|0x80;
		lcd_command(xy);
	}
	if(x==2)
	{
		xy=y|0xC0;
		lcd_command(xy);
	}
}

void lcd_putchar(unsigned char c)
{
	lcd_putc(c);
}
void lcd_display_integer(unsigned int k)
{
	if(k<10)
	{
		lcd_putchar(0+0x30);
		lcd_putchar(k+0x30);
	}
	else if(k<100)
	{
		lcd_putchar(k/10+0x30); 
		lcd_putchar(k%10+0x30);	
	}
	else if(k<1000)
	{
		lcd_putchar(k/100+0x30); 
		lcd_putchar((k%100)/10+0x30);	
		lcd_putchar((k%100)%10+0x30);	
	}
	else if(k<10000)
	{
		lcd_putchar(k/1000+0x30); 
		lcd_putchar((k%1000)/100+0x30);	
		lcd_putchar(((k%1000)%100)/10+0x30);
		lcd_putchar(((k%1000)%100)%10+0x30);	
	}
	else
	{}		
}

void lcd_display_real(float r)
{
	unsigned int x,y;
	
	if(r>0)
	{
		x=r/1;
		y=(r-x)*1000/1;
	}
	
	else
	{
		x=r/1;
		y=((r-x)*1000/1)*(-1);
	}
	
	lcd_display_integer(x);
	lcd_puts(".");
	lcd_display_integer(y);
}

/*

int main()
{
	uint8_t k=0;
	float x;
	lcd_init();
	while(1)
	{
		lcd_gotoxy(1,0);
		lcd_puts("Nhuong Trieu");
		lcd_gotoxy(2,0);
		lcd_puts("HYPro.......");
		lcd_command(0x01);
		lcd_gotoxy(1,0);
		lcd_puts("So lan");
		lcd_gotoxy(1,7);
		lcd_display_integer(k);
		k++;
		if(k==100)k=0;
		lcd_gotoxy(2,0);
		lcd_puts("So lan");
		lcd_gotoxy(2,8);
		lcd_display_real(x);
		x=x+2.54;
		if(x>100)x=0;

	}
}
*/
