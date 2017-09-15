/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "queue.h"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

xQueueHandle xQueue;
#define mainSENDER_l 1
#define mainSENDER_2 2


typedef struct
{
  unsigned char ucValue;
  unsigned char ucSource;
} xData;

static xData xStructsToSend[2] =
{
  {255, 255},
  {255, 255}
};


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
  portBASE_TYPE xStatus;
  GPIO_ResetBits(GPIOD,GPIO_Pin_All);

  for(;;)
  {

    xStatus = xQueueSendToBack( xQueue,&pvParameters,100);//  &amp;
	if (xStatus != pdPASS)
	{ /* Если попытка записи не была успешной —  индицировать ошибку. */
 		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	}
	taskYIELD();
  }
}
void vReceiveTask( void *pvParameters)
{
  xData IReceivedStructure;//={0,0};
  portBASE_TYPE xStatus;
  GPIO_ResetBits(GPIOD,GPIO_Pin_All);

  for(;;)
  {
  	if (uxQueueMessagesWaiting(xQueue) != 3)
	{
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
	}

	xStatus = xQueueReceive(xQueue, &IReceivedStructure, 0 );
	if (xStatus == pdPASS)
	{

	  if (IReceivedStructure.ucSource == mainSENDER_l)
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	  else
			GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
	}
  }
}

int main()
{
  init_LED();


  xQueue=xQueueCreate( 3, sizeof(xData) );

  xTaskCreate( vSenderTask, ( const char*  ) "Task 1", 1000, &xStructsToSend[0], 2, NULL );
  xTaskCreate( vSenderTask, ( const char*  ) "Task 2", 1000, &xStructsToSend[1], 2, NULL );
  xTaskCreate( vReceiveTask, ( const char*  ) "Task 2", 1000, NULL, 1, NULL );

  vTaskStartScheduler();
  while(1);
}
