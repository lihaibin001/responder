#ifndef __IWDOG_H__
#define __IWDOG_H__

#include "stm32f10x_conf.h"

/**
  * @brief  ��ʼ�����Ź�
  * @param  ��
  * @retval ��
  */
void IWdog_Init(void);	

/**
  * @brief  ι��
  * @param  ��
  * @retval ��
  */
void IWdog_Feed(void);
#endif //__IWDOG_H__
