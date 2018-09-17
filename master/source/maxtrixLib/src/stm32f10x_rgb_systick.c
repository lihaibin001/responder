/**
  ******************************************************************************
  * @file    stm32f10x_rgb_systick.c
  * @author  fire
  * @version V2.0
  * @date    2013-xx-xx
  * @brief   STM32 RGB������ʱͷ�ļ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x_rgb_systick.h"

static __IO u32 TimingDelay;
__IO u32 Timing;

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ���� 
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(72000000 / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}


/*
 * ��������Delay_us
 * ����  ��us��ʱ����,1msΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1ms
 *       ���ⲿ���� 
 */

void Delay_us(__IO u32 nTime)
{ 
  //TimingDelay = nTime;

  //while(TimingDelay != 0);
	
   u16 i=0;  
   while(nTime--)
   {
      i=13350;
      while(i--) ;    
   }
}

void delay(__IO u32 nTime)
{
	TIM4_Configuration(ENABLE);
  TimingDelay = nTime;

  while(TimingDelay != 0);
	TIM4_Configuration(DISABLE);

}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{
	/*Timing++;  //�ܼ���ֵ
	if(Timing==0xffffffff)
	{
		Timing=0;
	}*/
  Get_Flag(); //�ж�flag
  /*if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }*/
}

void delay_interupt(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
/******************* (C) COPYRIGHT 2016 *****END OF FILE****/
