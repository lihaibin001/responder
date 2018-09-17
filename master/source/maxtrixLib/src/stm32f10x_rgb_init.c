/**
  ******************************************************************************
  * @file    stm32f10x_rgb_init.c
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   ������ʾ����ʼ���ļ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x_rgb_init.h"

/*
 *
 */
void setupRGBMatrixPorts(void)
{
	//LED��ʼ�� led OE
	GPIO_InitTypeDef GPIO_InitStructure;
	/* enable GPIO port Clock */
	// enable a,b,c,d,e  RCC CLK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);          // enable LED GPIO port GPIOA
	// porta gpio init
	// NULL
	// portb gpio init
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = MTX_PR1 | MTX_PG1 | MTX_PB1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// portc gpio init null
	// portd gpio init
	GPIO_InitStructure.GPIO_Pin = MTX_PR0 | MTX_PG0 | MTX_PB0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// porte gpio init
	GPIO_InitStructure.GPIO_Pin = MTX_PA | MTX_PB | MTX_PC | MTX_PD | MTX_PSTB | MTX_POE | MTX_PCLK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* EXTI line gpio config(PC13) */
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);*/

	/********************************************************************/
	GPIO_ResetBits(MTX_PORTE, MTX_PA | MTX_PB | MTX_PC | MTX_PD); // A  B  C D
	GPIO_ResetBits(MTX_PORTD, MTX_PR0);     // R0
	GPIO_ResetBits(MTX_PORTB, MTX_PR1);     // R1
	GPIO_ResetBits(MTX_PORTD, MTX_PG0);      // G0
	GPIO_ResetBits(MTX_PORTB, MTX_PG1);      // G1
	GPIO_ResetBits(MTX_PORTD, MTX_PG0);     // B0
	GPIO_ResetBits(MTX_PORTB, MTX_PG1);     // B1

	GPIO_ResetBits(MTX_PORTE, MTX_PCLK | MTX_PSTB);  // SCK LE ��ʼ��
	/*****************  LED  ********************************************/
	GPIO_SetBits(BOARD_LED_PORT, BOARD_LED);        // LED   H �ر�led ��ʾ
	//GPIO_SetBits(LED_PORT, LED_R | LED_G | LED_B);  // LED_RED   H
	//GPIO_SetBits(GPIO_PORT, GPIO_6 |GPIO_7);        // GPIO_6 GPIO_7    H

	//GPIO_ResetBits(GPIO_PORT,GPIO_Pin_6);

}

/*
 * ��������NVIC_Configuration
 * ����  ������Ƕ�������жϿ�����NVIC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* ����P[A|B|C|D|E]0Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the EXIT6 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM2_Configuration(u8 EN, u16 TIME_period, u16 TIME_perescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = TIME_period - 1;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler = (TIME_perescaler - 1);				   /* ʱ��Ԥ��Ƶ�� 72M/72 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	//TIM_Cmd(TIM2, DISABLE);
	/* ����ʱ�� */
	if(EN == ENABLE)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		/*�ȹرյȴ�ʹ��*/
	else
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);		/*�ȹرյȴ�ʹ��*/
}


/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM3_Configuration(u8 EN)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = TIME_NUM - 1;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 1000*/
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler = (48000 - 1);				 /* ʱ��Ԥ��Ƶ�� 72M/72 72-1*/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	if(EN == ENABLE)
		TIM_Cmd(TIM3, ENABLE);																		/* ����ʱ�� */
	else
		TIM_Cmd(TIM3, DISABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/
}

/*TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms*/
void TIM4_Configuration(u8 EN)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 2;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 1000*/
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler = (36000 - 1);				 /* ʱ��Ԥ��Ƶ�� 72M/72 72-1*/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	if(EN == ENABLE)
		TIM_Cmd(TIM4, ENABLE);																		/* ����ʱ�� */
	else
		TIM_Cmd(TIM4, DISABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);		/*�ȹرյȴ�ʹ��*/
}

/*
 * ��������EXTI_PB0_Config
 * ����  ������ PB0 Ϊ���жϿڣ��������ж����ȼ�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC13) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //RCC_APB2Periph_GPIOB |
	//	/* config the NVIC(PC13) */
	//	NVIC_Configuration();
	/* EXTI line(PC13) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}



/*********************************************END OF FILE**********************/
