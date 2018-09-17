#include "FreeRTOS.h"
#include "task.h"
#include "peridic.h"
#include "eeprom.h"
#include "iic.h"
#include "sysclock.h"
#include "gpioInit.h"
#include "CanApp.h"
#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "ws2812b.h"
#include "rand.h"
#include "at24qtxx.h"
void vApplicationDaemonTaskStartupHook( void )
{
    sysclock_init();
    gpioInit();
    CanAppInit();
    ws2812b_Init();
    IIC_Init();
    at24qt_intit();

    at24qt_setMode(at24qtxx_fastMode);    
}

void vApplicationIdleHook( void )
{
    
}

void vApplicationTickHook( void )
{
    
}