#include "stm32_usart.h"

/*----------------------------------------------------------------------------
  external functions
 *----------------------------------------------------------------------------*/
extern int  usart1_send_data(int ch);
extern int  usart1_get_data(void);
extern int  usart2_send_data(int ch);
extern int  usart2_get_data(void);
extern int  usart3_send_data(int ch);
extern int  usart3_get_data(void);

struct __FILE 
{
  int handle;                 // Add whatever you need here 
};
FILE __stdout;
FILE __stdin;


GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;


/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) 
{
	
    return (usart1_send_data(ch));
}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) 
{
    return (usart1_send_data(usart1_get_data()));
}

void delay_ms(unsigned long int t)
{
	unsigned long int i;
	for(i=0;i<t;i++)
	{}
}

/********************************************************************
	Ham nhan 1 ki tu !!! 
********************************************************************/
int usart1_send_data (int ch)  
{
	while (!(USART1->SR & USART_FLAG_TXE));
	USART1->DR = (ch & 0x1FF);

	return (ch);
}
/********************************************************************
	Ham nhan 1 ki tu !!! 
********************************************************************/
int usart1_get_data(void)  
{
	while (!(USART1->SR & USART_FLAG_RXNE));   // Kiem tra co nhan cho den khi nhan xong
	return ((int)(USART1->DR & 0x1FF));		   // Nhan du lieu tu cong USART1
}

int usart2_send_data (int ch)  
{
	while (!(USART2->SR & USART_FLAG_TXE));
	USART2->DR = (ch & 0x1FF);

	return (ch);
}
/********************************************************************
	Ham nhan 1 ki tu !!! 
********************************************************************/
int usart2_get_data(void)  
{
	while (!(USART2->SR & USART_FLAG_RXNE));   // Kiem tra co nhan cho den khi nhan xong
	return ((int)(USART2->DR & 0x1FF));		   // Nhan du lieu tu cong USART1
}

int usart3_send_data (int ch)  
{
	while (!(USART3->SR & USART_FLAG_TXE));
	USART3->DR = (ch & 0x1FF);

	return (ch);
}
/********************************************************************
	Ham nhan 1 ki tu !!! 
********************************************************************/
int usart3_get_data(void)  
{
	while (!(USART3->SR & USART_FLAG_RXNE));   // Kiem tra co nhan cho den khi nhan xong
	return ((int)(USART3->DR & 0x1FF));		   // Nhan du lieu tu cong USART1
}


/*********************************************************************************
Ham khoi tao USART voi cac cong USART1, USART2, USART3

USART1: TX PA9 -- RX PA10
USART2: TX PA2 -- RX PA3
USART3: TX PB10 -- RX PB11
*********************************************************************************/
void usart1_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE); // Clock GPIOA, GPIOB


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Configure PAx for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = Pin_TX;			   // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   // Set up output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // Speed at GPIO_Pin
	GPIO_Init(GPIO_x, &GPIO_InitStructure);			   // define Portx
	

	/* Configure PAx for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = Pin_RX;				  // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Set up input
	GPIO_Init(GPIO_x, &GPIO_InitStructure);				  // define Portx
														  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ClearFlag(USART1, USART_FLAG_RXNE);

	/* Enable USARTy Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);
}

void usart2_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE); // Clock GPIOA, GPIOB


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Configure PAx for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = Pin_TX;			   // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   // Set up output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // Speed at GPIO_Pin
	GPIO_Init(GPIO_x, &GPIO_InitStructure);			   // define Portx
	

	/* Configure PAx for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = Pin_RX;				  // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Set up input
	GPIO_Init(GPIO_x, &GPIO_InitStructure);				  // define Portx
														  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);

	/* Enable USARTy Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);
}

void usart3_init(GPIO_TypeDef* GPIO_x, int Pin_TX, int Pin_RX, long baudrate)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE); // Clock GPIOA, GPIOB
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

	/* Configure PAx for USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = Pin_TX;			   // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   // Set up output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // Speed at GPIO_Pin
	GPIO_Init(GPIO_x, &GPIO_InitStructure);			   // define Portx
	

	/* Configure PAx for USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = Pin_RX;				  // Pin_TX cua USARTx (x=1,2,3)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Set up input
	GPIO_Init(GPIO_x, &GPIO_InitStructure);				  // define Portx
														  
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* USART configuration */
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ClearFlag(USART3, USART_FLAG_RXNE);

	/* Enable USARTy Receive and Transmit interrupts */
	USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
}


void usart_send_data(USART_TypeDef* USARTx, char data)
{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){;}
		USART_SendData(USARTx, (uint16_t)data);
}

void usart1_send_string(const char* str)
{
		while(*str != '\0')
		{
				usart_send_data(USART1, *str++);
		}
}

void usart2_send_string(const char* str)
{
		while(*str != '\0')
		{
				usart_send_data(USART2, *str++);
		}
}

void usart3_send_string(const char* str)
{
		while(*str != '\0')
		{
				usart_send_data(USART3, *str++);
		}
}



