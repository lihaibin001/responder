/**
  ******************************************************************************
  * @file    stm32f10x_rgb_uart.c
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   ���ڹ����ļ�
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "stm32f10x_rgb_uart.h"
#include <stdarg.h>

char g_DatRev[Max_UART_String] = {0};  //USART����
u8 g_DatRev_num=0,UART_FLAG=0;  //���� ��־λ  UART_FLAG = 0x01 Ϊ���ܵ��ַ��� 0x02 Ϊ�������
//u16 g_DatRev_num_uart2;   //usart2 ��������
//u8 aaa;
/*
 * ��������UART1_Config
 * ����  ��UART1_Config ���ã�����ģʽ����  115200-8-N-1
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void USART1_Config(u8 EN,u32 Baud_Rate) // 115200 65535
{
	if(EN==ENABLE){
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;

		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure); 
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART1, ENABLE);
		USART_ClearFlag(USART1,USART_FLAG_TC);                     //������ɱ�־λ
  }
}

void USART2_Config(u8 EN,u32 bound){
	if(EN==ENABLE){
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;
		
			/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 
	USART_DeInit(USART2);  
  //USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  //USART2_RX     PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????????
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //??????PA10
 
   
  //USART ???????�??
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
 
		USART_Init(USART2, &USART_InitStructure); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART2, ENABLE);  
	}		
}

char strcmp1(char *lk,char *ls)
{
		char *a = lk, *b = ls;
		u8 bSame = TRUE;
		while (*b!='\0')
		{
				if (*a++ != *b++) 
				{
						bSame = FALSE; break;
				}
		}
    return bSame;
}

/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
    /* ��printf���ݷ������� */
	  USART_SendData(USART1, (unsigned char) ch);
	  while(!(USART1->SR & USART_FLAG_TXE));
	
	  return (ch);
}

/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */
/*
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{
		if ( *Data == 0x5c )  //'\'
		{
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}

void Get_Flag()
{
		if(UART_FLAG==0x01)   //�����־λ��һλ��һ˵���������յ�'\n'
		{
				UART_FLAG &= 0xFE;  //�����־λ
				printf("String is:%s\n",g_DatRev);
				//�����ܵ����ݺ󣬽��������������������
				
				if(strcmp1(g_DatRev,"AT+RST\r\n"))
				{
					printf("OK\r");
					SystemReset();
				}
				else if(strcmp1(g_DatRev,"AT+SPI_FLASH_BulkErase\n"))
				{
						//SPI_FLASH_BulkErase();
						printf("[OK] SPI_FLASH_BulkErase OK");
				}
				else if(strcmp1(g_DatRev,"AT+W\r\n"))
				{
					  //aaa = 'w';
				}
				else if(strcmp1(g_DatRev,"AT+S\r\n"))
				{
					  //aaa = 's';
				}
				else if(strcmp1(g_DatRev,"AT+A\r\n"))
				{
					  //aaa = 'a';
				}
				else if(strcmp1(g_DatRev,"AT+D\r\n"))
				{
					  //aaa = 'd';
				}
				else if(strcmp1(g_DatRev,"AT+CLOSE\r\n"))
				{
						printf("[OK] CLOSE SYS\n");
				}

				//if(strcmp1(g_DatRev,"AT+HELP\r\n"))
				else{
						printf("***   System Help   ***\n");
						printf("Please any AT+ ..  \n");
						printf("AT+RST to reboot System\n");
						printf("AT+SPI_FLASH_BulkErase to Erase\n");
						printf("AT+CLOSE close led\n");
				}
				/*    ##################    */
				while(g_DatRev_num)  //��ս��ܻ�����
				{
						 g_DatRev[g_DatRev_num]=0;
						 g_DatRev_num--;
				}
		}
		else if(UART_FLAG==0x02)  //������������ 
		{
				UART_FLAG &= 0xFD;  //�����־λ
				printf("\r\n Data overflow \r\n");
				while(g_DatRev_num)    //��ս��ܻ���
				{
						 g_DatRev[g_DatRev_num]=0;
						 g_DatRev_num--;
				}
		}
}

/*******************************************************************************
* Function Name  : SystemReset
* Description    : Configures the port pin connected to the push button. GPIO_D_4
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__asm void SystemReset(void)
{
 MOV R0, #1           //;
 MSR FAULTMASK, R0    //;
 LDR R0, =0xE000ED0C  //;
 LDR R1, =0x05FA0004  //;
 STR R1, [R0]         //;
 
deadloop
    B deadloop        //;
}

/************************* (C) COPYRIGHT 2016  END OF FILE **********************/ 
