/**
  ******************************************************************************
  * @file    IR.c
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   Maktrix ������ʾ����
  ******************************************************************************
  * @attention
  * ���߲μ� matrix_config.h �����ļ�
	* �õ���Դ��ADC �������������
	* 
	* ��������ס����������������Լ����
	* 			��������ģʽ�°��������л���ʾģʽ
  ******************************************************************************
  */

#include "stm32f10x_rgb_IR.h"
#include "matrix_config.h"
#include "stm32f10x_rgb_matrix.h"

__asm int delay_us_private(u32 usec)
{
     ALIGN
     PUSH.W {r1}  //2ʱ������
     MOV r1,#20  //1ʱ������
     MUL r0,r1  //1ʱ������
     SUB r0,#3  //1ʱ������
loop
     SUBS r0,#1   //1ʱ������
     BNE loop   //�����ת��Ϊ3�����ڣ�������ֻ��1������
     POP {r1}   //2ʱ������
     BX lr    //3��ʱ������
                  //�ܹ���������Ϊ(usec*4)-4,�˴���4��Ҫ���ڵ������ô˺���������ʱ�����ڣ�����1ʱ�ӣ�BLX��ת3ʱ�ӣ�
}

u8 hongwai(void){
	#define ERROR 0XFF
	u8 key=0;
	u8 LOW_TIME=0,HIGHTIME=0;
	LED_BLUE_ON;
	Display_Enable(DISABLE);
	TIM4_Configuration(DISABLE);
	while(GPIO_IR==0){
		delay_us_private(100);
		LOW_TIME++;
	}
	while(GPIO_IR==1){
		delay_us_private(100);
		HIGHTIME++;
	}
	if((LOW_TIME>125)&&(LOW_TIME<135)&&(HIGHTIME>25)&&(HIGHTIME<35)){
		//?????
		u8 i;
		LED_BLUE_ON;
		HIGHTIME=LOW_TIME=0;
		for(i=0;i<6;i++){
			while(GPIO_IR==0)
				HIGHTIME=0;
			while(GPIO_IR==1){
				delay_us_private(100);
				HIGHTIME++; 
				if(HIGHTIME>=250)
					return ERROR;
			}
			if((HIGHTIME<11)&&(HIGHTIME>9)){
				key=key&0x7f;
			}
			else if((HIGHTIME<33)&&(HIGHTIME>22)){
				key=key|0x80;
			}
		}
		key=key<<2;
	}
	LED_BLUE_OFF;
	TIM4_Configuration(ENABLE);
	Display_Enable(ENABLE);
	return key;
}
