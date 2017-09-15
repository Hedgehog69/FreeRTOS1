/*FreeRTOS includes*/
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
/*ST microelectronics includes*/
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

/*���������� ���������� ��� �������� ���������� ������ 2 */
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
  /* �������� ��������� ������ 1. �� ����� 2 � �� ���������� �� ���������� ����� �������
  ������ ������� ��������� */
  uxPriority = uxTaskPriorityGet( NULL );

  GPIO_ResetBits(GPIOG,Task_2);
  for(;;)
  {
    GPIO_ToggleBits(GPIOG,Task_1);
	/* ������� ��������� ������ 2 �� ������� ������ ���������� ������ 1 (������ 3).
	�������� ������ � ������ 2 �� ���� ������ 1 ��������� ���������� ������ 2,
	������� �������� � ���������� ���������� xTask2Handle*/
	vTaskPrioritySet( xTask2Handle, ( uxPriority + 1 ));
		vTaskDelay(50);

  }
  vTaskDelete( NULL);
}

void vTask2( void *pvParameters)
{
  unsigned portBASE_TYPE uxPriority;
  /* �������� ��������� ������ 2. ��� ��� ����� ������ ������������ ������ 1 ����� ����� ������� ���������,
  �� ���� ������ 2 �������� ����������, ������, �� ��������� ��� ������� �� 3 */
  uxPriority = uxTaskPriorityGet( NULL );

  GPIO_ResetBits(GPIOG,Task_1);

  for(;;)
  {
  	GPIO_ToggleBits(GPIOG,Task_2);
	/* ������ 2 �������� ���� ��������� �� 2 ������� (���������� ����� 1). ����� �������, �� ���������� ����
 	���������� ������ 1, � ������ 1 �������� ���������� */
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
