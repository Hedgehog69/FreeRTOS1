/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#define Task_2	GPIO_Pin_14
#define Task_1	GPIO_Pin_13

void init_LED()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&gpio);
}


void vTask2( void *pvParameters)
{
  GPIO_ToggleBits(GPIOG,Task_2);
  vTaskDelete( NULL);// Схема освобождения памяти heap_1.c не работает!!!
}


void vTask1( void *pvParameters)
{
  vTaskDelay(100);
  for(;;)
  {
    GPIO_ToggleBits(GPIOG,Task_1);
  	xTaskCreate( vTask2, ( const char * ) "vTask2", 2000, NULL, 2, NULL );
	vTaskDelay(100);
  }
  vTaskDelete( NULL);
}

int main()
{
  init_LED();
  xTaskCreate( vTask1, ( const char*  ) "Task 1", 2000, NULL, 1, NULL );
  vTaskStartScheduler();
  while(1);
}
