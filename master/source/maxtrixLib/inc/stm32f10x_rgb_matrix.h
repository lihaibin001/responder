/**
  ******************************************************************************
  * @file    Matrix.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2016
  * @brief   Matrix ����ͷ�ļ�
  *
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __MATRIX_H
#define	__MATRIX_H

#include "stm32f10x.h"
#include "matrix_config.h"
#include "stm32f10x_rgb_Fonts.h"
#include "stm32f10x_rgb_rtc.h"
#include "stm32f10x_rgb_systick.h"
#include "stm32f10x_rgb_init.h"

#include "stm32f10x_rgb_uart.h"
#include "stm32f10x_rgb_gfx.h"
#include "stm32f10x_rgb_spi_flash.h"
#include "stm32f10x_rgb_Colors.h"

#define LIFE 1
#define RIGHT 2
#define GET_ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))


extern const bitmap_font apple3x5;
extern const bitmap_font apple5x7;
extern const bitmap_font apple6x10;
extern const bitmap_font apple8x13;
/* exact-width signed integer types */
typedef u32 rgb24;

typedef enum rotationDegrees {
		rotation0,
		rotation90,
	  rotation180,
		rotation270
} rotationDegrees;

typedef enum fontChoices {
    font3x5,
    font5x7,
    font6x10,
    font8x13,
} fontChoices;

typedef enum ScrollMode {
    wrapForward,    //��ǰ
    bounceForward,  //�� ��
    bounceReverse,  //�� ��
	  showtime,
    stopped,        //ֹͣ
    off             //�ر�
} ScrollMode;

extern u8 hour,min,sec,w_month,w_date,week;//Сʱ�����ӣ��룬ȫ�ֱ���

void Matrix_Init(rotationDegrees rotation,u8 enable);  //��ʼ������
void Timer_Move_interrupt(void);
void display_PWM(void);
void setRow(u8);
//void setRGB(void);
void showLine(u16);

void Send_RGB_Module(u8,u8);
void Send_RGB_PWM(u8 row, u8 num, u8 s);                 //
/*
void Display_MOVE(u8, u8, u8, u8);                       //�ƶ��ַ��������ַ�
void Display_MOVE_Cache(u8,u8, u8,u8);
void Show_Ascii_Print_16_8(u8, u8, char *,u8);              //��ʾ16*8 ASCII �ַ�
void Show_Ascii_Print_8_4(u8 x, u8 y, int16_t);                   //��ʾ8*4 ASCII�ַ�
void Show_Time(void);                                    //��ʾʱ�� 
void Show_Number_Print_16_8(u8, u8, int16_t);
void Display_Clear_Cache(u8,u8);                         //���������
void Show_Ascii_Print(u8, u8, char *);*/
void drawPixel(s8 x, s8 y, u32 Color);
void ClearBuff(u16 num1, u16 num2);
void setFont(fontChoices newFont);
void drawChar(u8 x, u8 y, u32 Color, char text);
void draw_hanzi(u8 x, u8 y, u32 Color, char *);
void drawString(u8 x, u8 y, u32 Color, char *text);
void drawScrollString(u8 ,u8 x, u8 y);
u8 getBitmapFontPixelAtXY(unsigned char letter, unsigned char x, unsigned char y, char *font);
//scroll
void stopScrollText(void);
int8_t getScrollStatus(void);
void scrollText(char inputtext[], int8_t numScrolls);
void setScrollMode(ScrollMode mode);
void setScrollSpeed(u8 pixels_per_second);
void setScrollFont(fontChoices newFont);
void setScrollColor(u32 newColor);
void setScrollbgColor(u32 newColor);
void setScrollXY(u8 row);
void setScrollOffsetFromEdge(u8 row);
void Scrolshowtime(void);
void drawImage(u8 x, u8 y, u32 Color, const u8 *addres);
void Display_Enable(u8 ENABLE);

#endif /* __MATRIX_H */
