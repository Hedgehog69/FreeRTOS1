/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

void init_LED()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
}

void vTask1( void *pvParameters)
{

  volatile unsigned long ul;

 for(;;)
 {
/* Инвертировать бит 0 порта PORTF */
  GPIO_ToggleBits(GPIOD,GPIO_Pin_13);

  for( ul = 0; ul<4000L; ul++ )
  {

  }

 }
 vTaskDelete( NULL);
}

void vTask2( void *pvParameters)
{

  volatile unsigned long ul;

 for(;;)
 {
/* Инвертировать бит 0 порта PORTF */
  GPIO_ToggleBits(GPIOD,GPIO_Pin_14);

  for( ul = 0; ul<8000L; ul++ )
  {

  }

 }
 vTaskDelete( NULL);
}

int main()
{
  init_LED();
  xTaskCreate( vTask1, ( const char*  ) "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
  xTaskCreate( vTask2, ( const char * ) "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  vTaskStartScheduler();
  while(1);
}
