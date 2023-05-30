
/*
example code for priority preemtive scheduling of FreeRTOS using mutex resource managing.
*/


#include <stdio.h>
#include <conio.h>

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "portable.h"
#include "semphr.h"


#pragma warning(disable: 4996)



// Define task stack sizes
#define TASK_STACK_SIZE configMINIMAL_STACK_SIZE


// Define task periods in milliseconds
#define TASK1_PERIOD_MS 200
#define TASK2_PERIOD_MS 6000
#define TASK3_PERIOD_MS 1000


void vTask1(void* pvParameters);
void vTask2(void* pvparameters);
void vTask3(void* pvparameters);


TaskHandle_t T1 = NULL;
TaskHandle_t T2 = NULL;
TaskHandle_t T3 = NULL;

SemaphoreHandle_t xMutex;

int main()
{
	xMutex = xSemaphoreCreateMutex();

	if (xMutex != NULL) //The semaphore was created successfully and can be used.
	{
	    xTaskCreate(vTask1, "T1", TASK_STACK_SIZE, NULL, 3, NULL);
		xTaskCreate(vTask2, "T2", TASK_STACK_SIZE, NULL, 2, NULL);
		xTaskCreate(vTask3, "T3", TASK_STACK_SIZE, NULL, 1, NULL);


		// Start scheduler
		vTaskStartScheduler();

		for (;;);

		return 0;
	}
	return 0;
}

void vTask1(void* pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(TASK1_PERIOD_MS);
	xLastWakeTime = xTaskGetTickCount();
	if (xMutex != NULL)
	{
		for (;;)
		{
			if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
			{
				printf("%s \n", pcTaskGetName(T1));
				xSemaphoreGive(xMutex);
				printf("Released the mutex (%s)\n", pcTaskGetName(T1));
				vTaskDelayUntil(&xLastWakeTime, xPeriod); // Delay for period
			}
		}
	}
}
void vTask2(void* pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(TASK2_PERIOD_MS);
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		printf("%s \n", pcTaskGetName(T2)); 
		vTaskDelayUntil(&xLastWakeTime, xPeriod); // Delay for period
	}
}

void vTask3(void* pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_PERIOD_MS);
	xLastWakeTime = xTaskGetTickCount();

	
		if (xMutex != NULL)
		{
			
			if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
			{
				for (;;)
				{
				printf("%s \n", pcTaskGetName(T3));
				vTaskDelayUntil(&xLastWakeTime, xPeriod); // Delay for period
				//xSemaphoreGive(xMutex);
				//printf("Released the mutex (%s)\n", pcTaskGetName(T3));
				}
			}
					
		}
	
	
}


