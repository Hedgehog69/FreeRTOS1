/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"

/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
#include <misc.h>

xSemaphoreHandle xBinarvSemaphore;

void init_LED()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_15;
	gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
}

void vPeriodic_Task()
{
  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_TIM1,  ENABLE  );


  TIM_TimeBaseInitTypeDef tim;

  tim.TIM_Prescaler          =  48000;//430
  tim.TIM_CounterMode        =  TIM_CounterMode_Up;
  tim.TIM_Period             =  1000;
  tim.TIM_ClockDivision      =  0;
  tim.TIM_RepetitionCounter  =  0;

  TIM_TimeBaseInit(  TIM1, &tim  );

  

  TIM_UpdateRequestConfig(  TIM1,  TIM_UpdateSource_Global  );
  TIM_UpdateDisableConfig(  TIM1,  DISABLE  );
  TIM_ITConfig(  TIM1, TIM_IT_Update,  ENABLE );
  


  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel                    =  TIM1_UP_TIM10_IRQn ;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  =  configLIBRARY_LOWEST_INTERRUPT_PRIORITY;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority         =  15;  
  NVIC_InitStructure.NVIC_IRQChannelCmd                 =  ENABLE;  

  NVIC_Init(  &NVIC_InitStructure  ); 
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn,15);//Priority must be lower than syscall!!!!!!!!!!!!!!!!!!!!!!!!
   TIM_Cmd(  TIM1,  ENABLE  );
  
}

void TIM1_UP_TIM10_IRQHandler(  void  )
{ 
	configASSERT( xBinarvSemaphore ); 
	GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
	TIM_ClearITPendingBit(  TIM1,  TIM_IT_Update  ); 

	static portBASE_TYPE xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;  
	xSemaphoreGiveFromISR(xBinarvSemaphore, &xHigherPriorityTaskWoken);
	if( xHigherPriorityTaskWoken == pdTRUE )
	{	
		portYIELD_FROM_ISR(&xHigherPriorityTaskWoken);
	}	
	 
}
static void fucking_interrupt(void *pvParameters)
{
	GPIO_ToggleBits(GPIOD,GPIO_Pin_13);

	//TIM_Cmd(  TIM1,  ENABLE  );
 	
	vTaskDelete(NULL);
}
static void vHandlerTask(void *pvParameters)
{
  
  GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
  configASSERT( xBinarvSemaphore );
  
  for (;;)
  {	
  	xSemaphoreTake( xBinarvSemaphore, portMAX_DELAY);	
	GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
	xTaskCreate( fucking_interrupt, "AAAAA",   3000, NULL, 3, NULL	);
	
  }
}







int main()
{
  
  init_LED();
  //__disable_irq();
  vSemaphoreCreateBinary(xBinarvSemaphore);
  if(xBinarvSemaphore!=NULL)
  	GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
  vPeriodic_Task(); 
  if(xBinarvSemaphore!=0)
  {
	xTaskCreate( vHandlerTask, 		"Handler", 3000, NULL, 1, NULL	);
	xTaskCreate( fucking_interrupt, "AAAAA",   3000, NULL, 3, NULL	);
  	vTaskStartScheduler();
  }
  while(1);
}
