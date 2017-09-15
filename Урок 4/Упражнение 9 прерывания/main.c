/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"
#include "semphr.h"
//#include "portasm.s"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

xQueueHandle xQueue;

void init_LED()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
}

void vSenderTask( void *pvParameters)
{
  long IValueToSend;
  portBASE_TYPE xStatus;
  GPIO_ResetBits(GPIOD,GPIO_Pin_All);
  IValueToSend = (long) pvParameters;

  for(;;)
  {
    xStatus = xQueueSendToBack( xQueue,&IValueToSend,0);//  &amp;
	if (xStatus != pdPASS)
	{ /* Если попытка записи не была успешной —  индицировать ошибку. */
 		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	}
	taskYIELD();
  }
  vTaskDelete( NULL );
}
void vReceiveTask( void *pvParameters)
{
  long IReceivedValue;
  portBASE_TYPE xStatus;
  GPIO_ResetBits(GPIOD,GPIO_Pin_All);
  for(;;)
  {
  	if (uxQueueMessagesWaiting(xQueue) != 0)
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14);

	xStatus = xQueueReceive(xQueue, &IReceivedValue, 100 );
		if (xStatus == pdPASS)
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		else
		  	GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
  }
  vTaskDelete( NULL);
}

xSemaphoreHandle xBinarvSemaphore;

int main()
{
  init_LED();

  vSemaphoreCreateBinary(xBinarvSemaphore);
  //_dos_setvect(Ox82, vExamplelnterruptHandler);
  xQueue=xQueueCreate( 5, sizeof(long) );

  xTaskCreate( vSenderTask, ( const char*  ) "Task 1", configMINIMAL_STACK_SIZE, (void *const)1, 1, NULL );
  xTaskCreate( vSenderTask, ( const char*  ) "Task 2", configMINIMAL_STACK_SIZE, (void *const)2, 1, NULL );
  xTaskCreate( vReceiveTask, ( const char*  ) "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL );

  vTaskStartScheduler();
  while(1);
}
