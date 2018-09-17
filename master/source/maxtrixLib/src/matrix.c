/**
 *
 * Bit-Banging of 32*32 RGB Matrix Panels using a STM32F401
 *
 * Author: Paul Rogalinski, paul@paul.vc
 * License: Public Domain / http://unlicense.org/
 *
 * This is for educational purposes only, driving such panels with
 * bitbanging is inefficient and naive. Nevertheless, one might this 
 * code useful for prototyping purposes moving onto a better suited 
 * hardware such as FPGA or at least some DMA Kung-Fu.
 *
 *
 * Pin Assignment for an STM Nucle-F401RE
 * ==============================================
 *
 * PORT A: Color Data
 * ----------------------------------------------
 *
 * r1   PORT A: GPIO_PIN_5
 * g1   PORT A: GPIO_PIN_6
 * b1   PORT A: GPIO_PIN_7
 *
 * r2   PORT A: GPIO_PIN_8
 * g2   PORT A: GPIO_PIN_9
 * b2   PORT A: GPIO_PIN_10
 *
 *
 * Address Select, Output Enable
 * ----------------------------------------------
 *
 * A    PORT B: GPIO_PIN_5
 * B    PORT B: GPIO_PIN_6
 * C    PORT B: GPIO_PIN_8
 * D    PORT B: GPIO_PIN_9
 * OE   PORT B: GPIO_PIN_10
 *
 * Clock & Latch
 * ----------------------------------------------
 * CLK  PORT C: GPIO_PIN_7
 * STB  PORT C: GPIO_PIN_0
 */


#include "stm32f10x.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>


#define DISP_ON         GPIOB->BSRRH = GPIO_PIN_10;
#define DISP_OFF        GPIOB->BSRRL = GPIO_PIN_10;
#define CLK_TOGGLE      GPIOC->ODR ^= (1 << 7); GPIOC->ODR ^= (1 << 7);
#define STROBE          GPIOC->BSRRL = GPIO_PIN_0; GPIOC->BSRRH = GPIO_PIN_0;

#define MATRIX_WIDTH    32
#define MATRIX_HEIGHT   32
#define MATRIX_SIZE     MATRIX_WIDTH*MATRIX_HEIGHT

void setRow(int row);
void showLine(int amount);
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane );
void randomizeFramebuffer(uint32_t *buffer);
int randomMod = 0;



int  main() {

  int waits[] = {10,20,40,80,160,320,640,1280};
  uint32_t framebuffer[MATRIX_SIZE];
  int frame = 0;

  // setup hardware / gpio ports

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN ;       // enable ports A, B, C
  GPIOA->MODER |= (1 << 10) | (1 << 12) | (1 << 14) | (1 << 18) | (1 << 16) | (1 << 20);  // enable PORT A pins 5,6,7,9,8,10
  GPIOB->MODER |= (1 << 12) | (1 << 20) | (1 << 18) | (1 << 10) | (1 << 16);              // enable PORT B pins 6,10,4,5,3
  GPIOC->MODER |= (1 << 14) | (1 << 0);                                                   // enable PORT C pin 0,7
  GPIOA->OSPEEDR = GPIO_SPEED_HIGH;
  GPIOB->OSPEEDR = GPIO_SPEED_HIGH;
  GPIOC->OSPEEDR = GPIO_SPEED_HIGH;

  // set initial control lines values.
  GPIOC->ODR &= ~(1 << 0);  // latch low
  GPIOC->ODR &= ~(1 << 7);  // clock low
  DISP_OFF;

  randomizeFramebuffer(framebuffer);

  int scan = MATRIX_HEIGHT/2;

  while (1) {
    uint8_t rowOffset1;
    uint8_t rowOffset2;
    for (int y=0; y<scan; y++){
      setRow(y);
      rowOffset1 = MATRIX_WIDTH*y;
      rowOffset2 = (MATRIX_WIDTH*y)+scan;

      for (int plane=0; plane < 8; plane ++){
        for (int x=0; x<MATRIX_WIDTH; x++){
          setRGB(framebuffer[rowOffset1+x], framebuffer[rowOffset2+x], plane);
          CLK_TOGGLE;
        }
        showLine(waits[plane]);
      }
    }
    if (++frame % 25 == 0) randomizeFramebuffer(framebuffer);
    if (frame % 200 == 0) randomMod++;
  }
}

/**
 * generates some random junk for testing on the framebuffer.
 */
void randomizeFramebuffer(uint32_t *buffer) {
  if (randomMod > 7) randomMod = 1;
  int max = (1 << randomMod);

  for (int i=0; i < MATRIX_SIZE; i++){
    buffer[i] = 0x00 |
      ((rand() % max)  <<  0) |
      ((rand() % max)  <<  8) |
      ((rand() % max)  << 16);
  }
}

/**
 * sets the row on the row gpio ports
 */
void setRow(int row) {

  // todo: perhaps a lookup table could give us a tiny boost here.

  if (row & 0b0001) GPIOB->BSRRL = GPIO_PIN_5;
  else GPIOB->BSRRH = GPIO_PIN_5;

  if (row & 0b0010) GPIOB->BSRRL = GPIO_PIN_6;
  else GPIOB->BSRRH = GPIO_PIN_6;

  if (row & 0b0100) GPIOB->BSRRL = GPIO_PIN_8;
  else GPIOB->BSRRH = GPIO_PIN_8;

  if (row & 0b1000) GPIOB->BSRRL = GPIO_PIN_9;
  else GPIOB->BSRRH = GPIO_PIN_9;
}


/**
 * loads rgb1 and rgb2 gpio ports with the given bitplane
 */
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane ){

  // TODO: this is quite a good candidate for performance
  // TODO: optimizations. if you want to drive more chained
  // TODO: panels, start tweaking this code.
  GPIOA->ODR = 0x00000000;

  if (rgb1 & (1 << plane))        GPIOA->BSRRL = GPIO_PIN_5;
  if (rgb1 & (1 << (plane + 8)))  GPIOA->BSRRL = GPIO_PIN_6;
  if (rgb1 & (1 << (plane + 16))) GPIOA->BSRRL = GPIO_PIN_7;

  if (rgb2 & (1 << plane))        GPIOA->BSRRL = GPIO_PIN_8;
  if (rgb2 & (1 << (plane + 8)))  GPIOA->BSRRL = GPIO_PIN_9;
  if (rgb2 & (1 << (plane + 16))) GPIOA->BSRRL = GPIO_PIN_10;
}


/**
 * latches / shows a line and waits for n amount of time.
 */
void showLine(int amount) {
  STROBE;
  DISP_ON;
  for (int c=0; c<amount; c++) asm("nop");
  DISP_OFF;
}