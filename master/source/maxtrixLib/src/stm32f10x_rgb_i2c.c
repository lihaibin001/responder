/**
  ******************************************************************************
  * @file    i2c.c
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   ���i2c �����ļ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x_rgb_i2c.h"

#define I2C_Speed              400000
#define I2C_PageSize           8			/* AT24C02ÿҳ��8���ֽ� */
void delay_us(u8 j)
{        
   u8 i=150; //delay
   while(i) 
   { 
     i--; 
   } 
}
/*
 * ��������I2C_GPIO_Config
 * ����  ��I2C1 I/O����
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void I2C_Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SCL|I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	I2C_SCL_H;
	I2C_SDA_H;
}

void I2C_SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	GPIO_InitStructure.GPIO_Pin=I2C_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//������ʼ�ź�
void I2C_Start(void)
{
    I2C_SDA_OUT();
	
	I2C_SDA_H;
	I2C_SCL_H;
	delay_us(5);
	I2C_SDA_L;
	delay_us(6);
	I2C_SCL_L;
}

//����ֹͣ�ź�
void I2C_Stop(void)
{
   I2C_SDA_OUT();

   I2C_SCL_L;
   I2C_SDA_L;
   I2C_SCL_H;
   delay_us(6);
   I2C_SDA_H;
   delay_us(6);
}

//��������Ӧ���ź�ACK
void I2C_Ack(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_L;
   delay_us(2);
   I2C_SCL_H;
   delay_us(5);
   I2C_SCL_L;
}

//����������Ӧ���ź�NACK
void I2C_NAck(void)
{
   I2C_SCL_L;
   I2C_SDA_OUT();
   I2C_SDA_H;
   delay_us(2);
   I2C_SCL_H;
   delay_us(5);
   I2C_SCL_L;
}
//�ȴ��ӻ�Ӧ���ź�
//����ֵ��1 ����Ӧ��ʧ��
//		  0 ����Ӧ��ɹ�
u8 I2C_Wait_Ack(void)
{
	u8 tempTime=0;

	I2C_SDA_IN();

	I2C_SDA_H;
	delay_us(1);
	I2C_SCL_H;
	delay_us(1);

	while(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
	{
		tempTime++;
		if(tempTime>250)
		{
			I2C_Stop();
			return 1;
		}	 
	}

	I2C_SCL_L;
	return 0;
}
//I2C ����һ���ֽ�
void I2C_Send_Byte(u8 txd)
{
	u8 i=0;

	I2C_SDA_OUT();
	I2C_SCL_L;//����ʱ�ӿ�ʼ���ݴ���

	for(i=0;i<8;i++)
	{
		if((txd&0x80)>0) //0x80  1000 0000
			I2C_SDA_H;
		else
			I2C_SDA_L;

		txd<<=1;
		I2C_SCL_H;
		delay_us(2); //��������
		I2C_SCL_L;
		delay_us(2);
	}
}

//I2C ��ȡһ���ֽ�

u8 I2C_Read_Byte(u8 ack)
{
   u8 i=0,receive=0;

   I2C_SDA_IN();
   for(i=0;i<8;i++)
   {
   		I2C_SCL_L;
		delay_us(2);
		I2C_SCL_H;
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIO_I2C,I2C_SDA))
		   receive++;
		delay_us(1);	
   }

   	if(ack==0)
	   	I2C_NAck();
	else
		I2C_Ack();

	return receive;
}

u8 AT24Cxx_ReadOneByte(u16 addr)
{
	u8 temp=0;

	I2C_Start();
	
	if(EE_TYPE>AT24C02)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Start();
	I2C_Send_Byte(0xA1);
	I2C_Wait_Ack();

	temp=I2C_Read_Byte(0); //  0   ���� NACK
	I2C_Stop();
	
	return temp;	
}

u16 AT24Cxx_ReadTwoByte(u16 addr)
{
	u16 temp=0;

	I2C_Start();
	
	if(EE_TYPE>AT24C02)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Start();
	I2C_Send_Byte(0xA1);
	I2C_Wait_Ack();

	temp=I2C_Read_Byte(1); //  1   ���� ACK
	temp<<=8;
	temp|=I2C_Read_Byte(0); //  0  ���� NACK

	I2C_Stop();
	
	return temp;	
}


void AT24Cxx_WriteOneByte(u16 addr,u8 dt)
{
	I2C_Start();

	if(EE_TYPE>AT24C02)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Send_Byte(dt);
	I2C_Wait_Ack();
	I2C_Stop();

	Delay_us(10);
}

void AT24Cxx_WriteTwoByte(u16 addr,u16 dt)
{
	I2C_Start();

	if(EE_TYPE>AT24C02)
	{
		I2C_Send_Byte(0xA0);
		I2C_Wait_Ack();
		I2C_Send_Byte(addr>>8);	//�������ݵ�ַ��λ
	}
	else
	{
	   I2C_Send_Byte(0xA0+((addr/256)<<1));//������ַ+���ݵ�ַ
	}

	I2C_Wait_Ack();
	I2C_Send_Byte(addr%256);//˫�ֽ������ݵ�ַ��λ		
							//���ֽ������ݵ�ַ��λ
	I2C_Wait_Ack();

	I2C_Send_Byte(dt>>8);
	I2C_Wait_Ack();

	I2C_Send_Byte(dt&0xFF);
	I2C_Wait_Ack();

	I2C_Stop();

	Delay_us(10);
}


/******************** (C) COPYRIGHT 2011 ����Ƕ��ʽ���������� ********************/
