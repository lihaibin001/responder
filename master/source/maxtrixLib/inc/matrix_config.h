/**
  ******************************************************************************
  * @file    matrix_config.h
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   全彩点阵显示屏配置文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __MATIRX_CONFIG_H
#define __MATIRX_CONFIG_H

/*
 *  PD3   R1  | G1   PD4
 *  PD7   B1  | GND  GND
 
 *  PB5   R2  | G2   PB6
 *  PB7   B2  | GND   /
 
 *  PE0   A   | B    PE1
 *  PE2   C   | D    PE2
 
 *  PE4  CLK  | LE   PE5
 *  PE6   OE  | GND
 
 *
 *
 *  LED_R | PC12
 *  LED_G | PC8
 *  LED_B | PC9
 *
 *
	 *  DHT11 | PA8
	 *
	 *  GPIO1 | PB6
	 *  GPIO2 | PB7
 * 
 */
 
/* 状态指示灯 */
#define LED_PORT 	GPIOA
#define LED_R			GPIO_Pin_12
#define LED_G			GPIO_Pin_11
#define LED_B			GPIO_Pin_9
#define LED_RCCPB	RCC_APB2Periph_GPIOA


/* 板子内部指示灯 */
#define BOARD_LED_PORT 	GPIOA
#define BOARD_LED			  GPIO_Pin_6
#define BOARD_LED_RCCPB	RCC_APB2Periph_GPIOA

/*#define GPIO_PORT 	GPIOB
#define GPIO_6			GPIO_Pin_6
#define GPIO_7      GPIO_Pin_7
#define GPIO_RCCPB	RCC_APB2Periph_GPIOB
#define GPIO6_HIGH  GPIO_PORT->BSRR = GPIO_6
#define GPIO6_LOW   GPIO_PORT->BRR = GPIO_6
#define GPIO7_HIGH  GPIO_PORT->BSRR = GPIO_7
#define GPIO7_LOW   GPIO_PORT->BSRR = GPIO_7*/

#define MTX_PORTB	GPIOB  //当使能GPIOB的时钟时，也要使能GPIOC时钟 因为ABC 挂载GPIOC上
#define MTX_PORTC   GPIOC
#define MTX_PORTD   GPIOD

#define MTX_RCCPB	RCC_APB2Periph_GPIOB
// rgb define 
#define MTX_PR0		GPIO_Pin_3   //PD3 
#define MTX_PG0		GPIO_Pin_4   //PD4 
#define MTX_PB0		GPIO_Pin_7   //PD7

#define MTX_PR1		GPIO_Pin_5  //PB5 
#define MTX_PG1		GPIO_Pin_6  //PB6
#define MTX_PB1		GPIO_Pin_7   //PB7

#define MTX_PORTE GPIOE
// a b c d define 
#define MTX_PA		GPIO_Pin_0   //PE0
#define MTX_PB		GPIO_Pin_1   //PE1
#define MTX_PC		GPIO_Pin_2   //PE2
#define MTX_PD		GPIO_Pin_3   //PE3

// define   control port 
#define MTX_PSTB	GPIO_Pin_5   //PE5  lat 
#define MTX_POE		GPIO_Pin_6   //PE6  oe 
#define MTX_PCLK	GPIO_Pin_4   //PE4  clk 


#define DISP_ON         MTX_PORTE->BRR = MTX_POE                            // OE 
#define DISP_OFF        MTX_PORTE->BSRR = MTX_POE
#define CLK_TOGGLE      MTX_PORTE->BSRR = MTX_PCLK; MTX_PORTE->BRR = MTX_PCLK  // CLK 
#define STROBE          MTX_PORTE->BSRR = MTX_PSTB; MTX_PORTE->BRR = MTX_PSTB  // lLE 

#define MATRIX_WIDTH    64
#define MATRIX_HEIGHT   32
#define MATRIX_MODULE   32

#define MATRIX_SIZE     MATRIX_WIDTH*MATRIX_HEIGHT


#define REFISH_60HZ    55  //60.6Hz  6.90ms  16.5ms
#define REFISH_70HZ    48  //69.4Hz  4.80ms  14.4ms
#define REFISH_90HZ    35  //95.2Hz  0.98ms  10.5ms


#define LED_RED_ON     LED_PORT->BRR  = LED_R  //GPIO_ResetBits(LED_PORT, LED_R);
#define LED_RED_OFF    LED_PORT->BSRR = LED_R  //GPIO_SetBits(LED_PORT, LED_R);
#define LED_GREEN_ON   LED_PORT->BRR  = LED_G  //LED_GREEN_ON
#define LED_GREEN_OFF  LED_PORT->BSRR = LED_G  //LED_GREEN
#define LED_BLUE_ON    LED_PORT->BRR  = LED_B
#define LED_BLUE_OFF   LED_PORT->BSRR = LED_B

#define DISP_ZISE(a)   ((sizeof(a) / sizeof(*(a)))-1) //缓存大小

//#define FREQUENCY   72
#define FREQUENCY   120000000
//#define FREQUENCY   720000000
//#define FREQUENCY   72

#endif
