/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*Глобальная переменная для хранения приоритета Задачи 2 */
xTaskHandle xTask2Handle;
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
  unsigned portBASE_TYPE uxPriority;
  /* Получить приоритет Задачи 1. Он равен 2 и не изменяется на протяжении всего времени
  работы учебной программы */
  uxPriority = uxTaskPriorityGet( NULL );

  GPIO_ResetBits(GPIOG,Task_2);
  for(;;)
  {
    GPIO_ToggleBits(GPIOG,Task_1);
	/* Сделать приоритет Задачи 2 на единицу больше приоритета Задачи 1 (равным 3).
	Получить доступ к Задаче 2 из тела Задачи 1 позволяет дескриптор Задачи 2,
	который сохранен в глобальной переменной xTask2Handle*/
	vTaskPrioritySet( xTask2Handle, ( uxPriority + 1 ));
		vTaskDelay(50);

  }
  vTaskDelete( NULL);
}

void vTask2( void *pvParameters)
{
  unsigned portBASE_TYPE uxPriority;
  /* Получить приоритет Задачи 2. Так как после старта планнровшика Задача 1 имеет более высокий приоритет,
  то если Задача 2 получает управление, значит, ее приоритет был повышен до 3 */
  uxPriority = uxTaskPriorityGet( NULL );

  GPIO_ResetBits(GPIOG,Task_1);

  for(;;)
  {
  	GPIO_ToggleBits(GPIOG,Task_2);
	/* Задача 2 понижает свой приоритет на 2 единицы (становится равен 1). Таким образом, он становится ниже
 	приоритета Задачи 1, и Задача 1 получает управление */
	vTaskPrioritySet( NULL, ( uxPriority - 2 ));
	vTaskDelay(50);
  }
  vTaskDelete( NULL);
}

int main()
{
  init_LED();
  xTaskCreate( vTask1, ( const char*  ) "Task 1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
  xTaskCreate( vTask2, ( const char * ) "vTask2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle );
  vTaskStartScheduler();
  while(1);
}
