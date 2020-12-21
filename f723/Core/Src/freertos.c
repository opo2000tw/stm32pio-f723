/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stddef.h>
#include <stdbool.h>
#include "stm32f723e_discovery.h"
#include "MLX90640_I2C_Driver.h"
#include "MLX90640_API.h"
#include "lwprintf.h"
#include "cpu_utils.h"
#include "spi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define size (sizeof(float) / sizeof(uint8_t) * 768)
#define output_size 3072
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint16_t CPUUsage;

static int c = 0;
extern volatile uint8_t time4_seconds_elapsed;

uint8_t aRxBuffer[output_size] = {0};
uint8_t aTxBuffer[output_size] = {0};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};
/* Definitions for myTimer02 */
osTimerId_t myTimer02Handle;
const osTimerAttr_t myTimer02_attributes = {
  .name = "myTimer02"
};
/* Definitions for ThermalEvent */
osEventFlagsId_t ThermalEventHandle;
const osEventFlagsAttr_t ThermalEvent_attributes = {
  .name = "ThermalEvent"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void Callback01(void *argument);
void Callback02(void *argument);

extern void MX_USB_HOST_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationMallocFailedHook(void);
void vApplicationDaemonTaskStartupHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook( void )
{
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
  to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
  task. It is essential that code added to this hook function never attempts
  to block in any way (for example, call xQueueReceive() with a block time
  specified, or call vTaskDelay()). If the application makes use of the
  vTaskDelete() API function (as this demo application does) then it is also
  important that vApplicationIdleHook() is permitted to return to its calling
  function, because it is the responsibility of the idle task to clean up
  memory allocated by the kernel to any task that has since been deleted. */
  __NOP();
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook( void )
{
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/* USER CODE BEGIN 5 */
__weak void vApplicationMallocFailedHook(void)
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created. It is also called by various parts of the
  demo application. If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */
  while (1)
    printf("[x] vApplicationMallocFailedHook\r\n");
}
/* USER CODE END 5 */

/* USER CODE BEGIN DAEMON_TASK_STARTUP_HOOK */
void vApplicationDaemonTaskStartupHook(void)
{
  printf("[x] vApplicationDaemonTaskStartupHook\r\n");
}
/* USER CODE END DAEMON_TASK_STARTUP_HOOK */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* creation of myTimer02 */
  myTimer02Handle = osTimerNew(Callback02, osTimerPeriodic, NULL, &myTimer02_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  osTimerStart(myTimer01Handle, MLX_FPS_CAL(MLX_RATE));
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of ThermalEvent */
  ThermalEventHandle = osEventFlagsNew(&ThermalEvent_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(MLX_FPS_CAL(MLX_RATE));
    // osDelay(5000);
#if 1
    printf("type[%X]:%d,%d,%d\r\n", aRxBuffer[0], aRxBuffer[1], aRxBuffer[2],
           aRxBuffer[3], aRxBuffer[output_size - 1]);
    aRxBuffer[output_size - 1] = 0;
#endif
#if 1
    printf("[[%d]]\r\n", c);
    c = 0;
    printf("%2.2f,%2.2f\r\n", mlx90640To[128], mlx90640To[256]);
#endif
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for (;;)
  {
    // osDelay(MLX_FPS_CAL(MLX_RATE));
    // int status = MLX90640_GetFrameData(MLX_ADDR, frame);
    // if (status < 0)
    // {
    //   printf("GetFrame Error: %d\r\n", status);
    // }
    // else
    // {
    //   paramsMLX90640 *params = &mlx90640;
    //   MLX90640_CalculateTo(frame, params, emissivity, mlx90640To);
    //   c++;
    // }
    osThreadExit();
  }
  /* USER CODE END StartTask02 */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
  int status = MLX90640_GetFrameData(MLX_ADDR, frame);
  if (status < 0)
  {
    printf("GetFrame Error: %d\r\n", status);
  }
  else
  {
    paramsMLX90640 *params = &mlx90640;
    MLX90640_CalculateTo(frame, params, emissivity, mlx90640To);
    c++;
  }
#if 0
  printf("sub:%d\r\n", MLX90640_GetSubPageNumber(frame));
#endif
  /* USER CODE END Callback01 */
}

/* Callback02 function */
void Callback02(void *argument)
{
  /* USER CODE BEGIN Callback02 */
  osThreadExit();
  /* USER CODE END Callback02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void spi_process(void)
{
  uint8_t type = aRxBuffer[0];
  uint8_t check = aRxBuffer[output_size - 1];
  printf("type=0x%X,end=%X\r\n", type, check);
  if (check != 0xFF)
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    // printf("check\r\n")
  }
  if (aRxBuffer[0] == 0xA0)
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    printf("start\r\n"); // TODO error
  }
  else if (aRxBuffer[0] == 0xA1)
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    printf("reset\r\n");
    osDelay(100);
    NVIC_SystemReset();
  }
  else if (aRxBuffer[0] == 0xBA)
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    printf("thread\r\n");
  }
  else if (aRxBuffer[0] == 0xFC)
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    printf("set\r\n");
  }
  else if (aRxBuffer[0] == 0xFB)
  {
    if (HAL_SPI_TransmitReceive_IT(TEST_SPI_ADDRESS, (uint8_t *)mlx90640To, (uint8_t *)aRxBuffer, output_size) != HAL_OK)
    {
      Error_Handler();
    }
    printf("data\r\n");
  }
  else
  {
    if (HAL_SPI_Receive_IT(TEST_SPI_ADDRESS, (uint8_t *)aTxBuffer, output_size) != HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }
    printf("reserved\r\n");
  }
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    BSP_LED_Toggle(LED_GREEN);
    spi_process();
  }
}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    Error_Handler();
  }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    BSP_LED_Toggle(LED_GREEN);
    spi_process();
  }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
