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

void vTask1( void *pvParameters)
{
  for(;;)
  {
    GPIO_ToggleBits(GPIOG,Task_1);
	vTaskDelay(100);
	taskYIELD();
  }
  vTaskDelete( NULL);
}
void vTask2( void *pvParameters)
{

  for(;;)
  {
    GPIO_ToggleBits(GPIOG,Task_2);
	vTaskDelay(100);
	taskYIELD();
  }
  vTaskDelete( NULL);
}

int main()
{
  init_LED();
    GPIO_ToggleBits(GPIOG,Task_1);
  xTaskCreate( vTask1, ( const char*  ) "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  xTaskCreate( vTask2, ( const char*  ) "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

  vTaskStartScheduler();
  while(1);
}
