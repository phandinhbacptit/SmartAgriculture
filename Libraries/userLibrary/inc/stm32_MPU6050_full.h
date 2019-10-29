#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include <math.h>


#define MPU6050_ADDR 0xD0
#define MPU6050_I2C I2C1


#define	SMPLRT_DIV		0x19	// Sample Rate Divider
#define	CONFIG				0x1A	// Configuration
#define	GYRO_CONFIG		0x1B	// Gyroscope Configuration
#define	ACCEL_CONFIG	0x1C  // Accelerometer Configuration
#define	PWR_MGMT_1    0x6B	// thanh ghi cai dat cac che do nhu sleep, chon tan so dao dong ... cua MPU

/*
#define	ACCEL_XOUT_H	0x3B    // thanh ghi du lieu cua truc X lay tu bo cam bien gia toc trong MPU 8bit cao
#define	ACCEL_XOUT_L	0x3C    // thanh ghi du lieu cua truc X lay tu bo cam bien gia toc trong MPU 8bit thap
#define	ACCEL_YOUT_H	0x3D    // thanh ghi du lieu cua truc Y lay tu bo cam bien gia toc trong MPU 8bit cao
#define	ACCEL_YOUT_L	0x3E    // thanh ghi du lieu cua truc Y lay tu bo cam bien gia toc trong MPU 8bit thap
#define	ACCEL_ZOUT_H	0x3F    // thanh ghi du lieu cua truc Z lay tu bo cam bien gia toc trong MPU 8bit cao
#define	ACCEL_ZOUT_L	0x40    // thanh ghi du lieu cua truc Z lay tu bo cam bien gia toc trong MPU 8bit thap
#define	TEMP_OUT_H		0x41    // thanh ghi du lieu cua nhiet do ben trong MPU 8 bit cao
#define	TEMP_OUT_L		0x42    // thanh ghi du lieu cua nhiet do ben trong MPU 8 bit thap
#define	GYRO_XOUT_H		0x43    // thanh ghi du lieu cua truc X lay tu bo cam bien van toc goc trong MPU 8bit cao
#define	GYRO_XOUT_L		0x44    // thanh ghi du lieu cua truc X lay tu bo cam bien van toc goc trong MPU 8bit thap
#define	GYRO_YOUT_H		0x45    // thanh ghi du lieu cua truc Y lay tu bo cam bien van toc goc trong MPU 8bit cao
#define	GYRO_YOUT_L		0x46    // thanh ghi du lieu cua truc Y lay tu bo cam bien van toc goc trong MPU 8bit thap
#define	GYRO_ZOUT_H		0x47    // thanh ghi du lieu cua truc Z lay tu bo cam bien van toc goc trong MPU 8bit cao
#define	GYRO_ZOUT_L		0x48    // thanh ghi du lieu cua truc Z lay tu bo cam bien van toc goc trong MPU 8bit thap
*/

int16_t gyro_value[8], gyro_zero[8];
int32_t gyro_x, gyro_y, gyro_z, acc_x, acc_y, acc_z;
int32_t gyro_x_zero, gyro_y_zero, gyro_z_zero;

float temp;

uint8_t w_data[1];
u8 clr_screen;

void delay_us(uint16_t millis)
{
	long i;
	i = millis * 10100;
	while(i)
	{
		i--;
	}
}

void GPIO_conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void MPU6050_I2C_Init()
{
	  I2C_InitTypeDef  I2C_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* Enable I2C and GPIO clocks */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	  /* Configure I2C pins: SCL and SDA */
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7; 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	      
	  /* I2C configuration */
	  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	  I2C_InitStructure.I2C_OwnAddress1 = 0xD0; // MPU6050 7-bit adress = 0x68, 8-bit adress = 0xD0; 
	  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	  I2C_InitStructure.I2C_ClockSpeed = 300000;
	  
	  /* Apply I2C configuration after enabling it */
	  I2C_Init(MPU6050_I2C, &I2C_InitStructure);
	  /* I2C Peripheral Enable */  
	  I2C_Cmd(MPU6050_I2C, ENABLE);
}

void MPU6050_I2C_ByteWrite(u8* pBuffer, u8 writeAddr)
{
  /* Send START condition */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);

  /* Test on EV5 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for write */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Send the MPU6050's internal address to write to */
  	I2C_SendData(MPU6050_I2C, writeAddr);

  /* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  	I2C_SendData(MPU6050_I2C, *pBuffer);

  /* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STOP condition */
  	I2C_GenerateSTOP(MPU6050_I2C, ENABLE);

 // EXT_CRT_SECTION();

}

void MPU6050_I2C_BufferRead(uint8_t* pBuffer, u8 readAddr, u16 NumByteToRead)
{
  /* While the bus is busy */
  	while(I2C_GetFlagStatus(MPU6050_I2C, I2C_FLAG_BUSY));

  /* Send START condition */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);

  /* Test on EV5 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for write */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Transmitter); 

  /* Test on EV6 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  /* Clear EV6 by setting again the PE bit */
  	I2C_Cmd(MPU6050_I2C, ENABLE);

  /* Send the MPU6050's internal address to write to */
  	I2C_SendData(MPU6050_I2C, readAddr);

  /* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send STRAT condition a second time */
  	I2C_GenerateSTART(MPU6050_I2C, ENABLE);

  /* Test on EV5 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MPU6050 address for read */
  	I2C_Send7bitAddress(MPU6050_I2C, MPU6050_ADDR, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  	while(!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  /* While there is data to be read */
  	while(NumByteToRead)
  	{
    	if(NumByteToRead == 1)
    	{
      		/* Disable Acknowledgement */
      		I2C_AcknowledgeConfig(MPU6050_I2C, DISABLE);

      		/* Send STOP Condition */
      		I2C_GenerateSTOP(MPU6050_I2C, ENABLE);
    	}

    	/* Test on EV7 and clear it */
    	if(I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    	{
      		/* Read a byte from the MPU6050 */
      		*pBuffer = I2C_ReceiveData(MPU6050_I2C);

      		/* Point to the next location where the byte read will be saved */
      		pBuffer++;

      		/* Decrement the read bytes counter */
      		NumByteToRead--;
    	}
  	}
  	/* Enable Acknowledgement to be ready for another reception */
  	I2C_AcknowledgeConfig(MPU6050_I2C, ENABLE);
}

void MPU6050_init()
{
	w_data[0]=0x80;
	MPU6050_I2C_ByteWrite(w_data, PWR_MGMT_1);
	delay_us(5);

	w_data[0]=0x00;
	MPU6050_I2C_ByteWrite(w_data, PWR_MGMT_1);
	delay_us(5);
	w_data[0]=0x00;
	MPU6050_I2C_ByteWrite(w_data, SMPLRT_DIV);
	delay_us(5);
	w_data[0]=0x07;
	MPU6050_I2C_ByteWrite(w_data, CONFIG);
	delay_us(5);
	w_data[0]=0x18;
	MPU6050_I2C_ByteWrite(w_data, GYRO_CONFIG);
	delay_us(5);
	w_data[0]=0x10;
	MPU6050_I2C_ByteWrite(w_data, ACCEL_CONFIG);
	delay_us(5);
}


void MPU6050_get_gyro_acc(int16_t* AccelGyro) 
{
    uint8_t tmpBuffer[14]; 
		int i;
    MPU6050_I2C_BufferRead(tmpBuffer, 0x3B, 14); 
    /* Get accelation */
    for(i=0; i<3; i++) 
    AccelGyro[i]=(tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1];
		/* Temperature MPU*/
		AccelGyro[3]=((tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1])/340 + 36.53;
    /* Get Gyro Rate */
    for(i=4; i<7; i++)
    AccelGyro[i]=(tmpBuffer[2*i] << 8) + tmpBuffer[2*i+1];        
}

void MPU6050_gyro_offset()
{
	u8 i=0;
	//gyro_x_zero=0;gyro_y_zero=0;gyro_z_zero=0;
	for(i=0;i<50;i++)
	{
		MPU6050_get_gyro_acc(gyro_zero); 

		gyro_x_zero+=gyro_zero[4];
		gyro_y_zero+=gyro_zero[5];
		gyro_z_zero+=gyro_zero[6];
	}
	gyro_x_zero/=50;
	gyro_y_zero/=50;
	gyro_z_zero/=50;
}

void MPU6050_get_value()
{
	u8 i=0;
	//acc_x=0;acc_y=0;acc_z=0;
	//temp=0;
	//gyro_x=0;gyro_y=0;gyro_z=0;
	for(i=0;i<50;i++)
	{
		MPU6050_get_gyro_acc(gyro_value); 
	
		acc_x+=gyro_value[0];
		acc_y+=gyro_value[1];
		acc_z+=gyro_value[2];
		//temp+=gyro_value[3];
		gyro_x+=gyro_value[4];
		gyro_y+=gyro_value[5];
		gyro_z+=gyro_value[6];
	}
	acc_x/=50;
	acc_y/=50;
	acc_z/=50;
	//temp/=50;
	gyro_x/=50;
	gyro_y/=50;
	gyro_z/=50;
}

