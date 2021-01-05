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
static void spi_process(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define size (sizeof(float) / sizeof(uint8_t) * 768)
#define spi_evt_id 0x00000001
#define i2c_evt_id 0x00000002
#define qqq_evt_id 0x00000004
float vdd, Ta, tr;
int subPage;
osStatus_t spi_state;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint16_t CPUUsage;
uint8_t *test_ptr = (uint8_t *) &mlx90640To[0];
extern struct mlx_event mlx_event;
extern volatile uint8_t time4_seconds_elapsed;

uint8_t aRxBuffer[DMA_U8_OUTPUT_SIZE]__attribute__((aligned (4))) = {0};
uint8_t aTxBuffer[DMA_U8_OUTPUT_SIZE]__attribute__((aligned (4))) = {0};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes =
{
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes =
{
  .name = "myTask02",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes =
{
  .name = "myTimer01"
};
/* Definitions for myTimer02 */
osTimerId_t myTimer02Handle;
const osTimerAttr_t myTimer02_attributes =
{
  .name = "myTimer02"
};
/* Definitions for empty_id */
osSemaphoreId_t empty_idHandle;
const osSemaphoreAttr_t empty_id_attributes =
{
  .name = "empty_id"
};
/* Definitions for filled_id */
osSemaphoreId_t filled_idHandle;
const osSemaphoreAttr_t filled_id_attributes =
{
  .name = "filled_id"
};
/* Definitions for ThermalEvent */
osEventFlagsId_t ThermalEventHandle;
const osEventFlagsAttr_t ThermalEvent_attributes =
{
  .name = "ThermalEvent"
};
/* Definitions for Thermal1sEvent */
osEventFlagsId_t Thermal1sEventHandle;
const osEventFlagsAttr_t Thermal1sEvent_attributes =
{
  .name = "Thermal1sEvent"
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
void vApplicationMallocFailedHook(void);
void vApplicationDaemonTaskStartupHook(void);

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
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */
  HAL_SPIEx_FlushRxFifo(TEST_SPI_ADDRESS);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of empty_id */
  empty_idHandle = osSemaphoreNew(10, 10, &empty_id_attributes);

  /* creation of filled_id */
  filled_idHandle = osSemaphoreNew(10, 10, &filled_id_attributes);

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

  /* creation of Thermal1sEvent */
  Thermal1sEventHandle = osEventFlagsNew(&Thermal1sEvent_attributes);

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
  int status = MLX90640_SynchFrame(MLX_ADDR);
  if (status != 0)
  {
    printf("\r\nMLX90640_SynchFrame failed with error code:%d\r\n", status);
    Error_Handler();
  }
  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) != GPIO_PIN_SET) // NSS signal
  {
    osDelay(1);
    printf("Init, NSS fail\r\n");
  }
  if (HAL_SPI_TransmitReceive_DMA(TEST_SPI_ADDRESS, test_ptr, (uint8_t *)aRxBuffer, DMA_U8_OUTPUT_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
  else
  {
    printf("\r\nHAL_SPI_TransmitReceive_DMA DMA_U8_OUTPUT_SIZE=%d\r\n", DMA_U8_OUTPUT_SIZE);
  }
  /* Infinite loop */
  for (;;)
  {
    osEventFlagsWait(ThermalEventHandle, spi_evt_id | i2c_evt_id, osFlagsWaitAll, osWaitForever);
    printf("------------\r\n");
    // printf("subPage:%d,vdd:%2.3f,tr:%2.3f\r\n", subPage, vdd, tr);
    // printf("slot:spi[%02d],i2c[%02d],err[%d]\r\n", mlx_event.spi, mlx_event.i2c, mlx_event.err);
    if (buffer_tag == BUFFER_A)
    {
      printf("sub=[%d],BUFF_A=[%2.3f],[%2.3f],[%2.3f],[%2.3f],[%2.3f]\r\n",
             subPage, mlx90640To[0], mlx90640To[127], mlx90640To[128], mlx90640To[256], mlx90640To[768 - 1]);
    }
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
    osDelay(1000);
    // osEventFlagsWait(Thermal1sEventHandle, spi_evt_id | i2c_evt_id, osFlagsWaitAll, osWaitForever);
    printf("1s:spi[%02d],i2c[%02d]\r\n", mlx_event.spi, mlx_event.i2c);
    printf("err:fc[%d],fd[%d],init[%d],vd[%d],tr[%d]\r\n",
           mlx_event.frame_check_err, mlx_event.frame_data_err, mlx_event.init_err,
           mlx_event.vdd_err, mlx_event.tr_err);
    mlx_event.spi = 0;
    mlx_event.i2c = 0;
  }
  /* USER CODE END StartTask02 */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */
  int frame_ready_status;
cb_01_error:
  frame_ready_status = MLX90640_GetFrameData(MLX_ADDR, frame);
  if (frame_ready_status < 0)
  {
    printf("GetFrame Error: [%d]\r\n", frame_ready_status);
    mlx_event.init_err++;
    goto cb_01_error;
  }
  else
  {
    subPage = MLX90640_GetSubPageNumber(frame);
    if (subPage >= 2 || subPage < 0)
    {
      printf("subPage Error: [%d]\r\n", subPage);
      goto cb_01_error;
    }
    // spi_state = osSemaphoreAcquire(empty_idHandle, osWaitForever);
    // if (spi_state != osOK)
    // {
    //   printf("osSemaphoreAcquire empty_idHandle 0:%d\r\n", spi_state);
    // }
    vdd = MLX90640_GetVdd(frame, &mlx90640);
    if (vdd <= 0)
    {
      mlx_event.vdd_err++;
      goto cb_01_error;
    }
    tr = MLX90640_GetTa(frame, &mlx90640) - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    if (tr > TEMPERATURE_MAXIMUM || tr <= TEMPERATURE_MINIMUM)
    {
      mlx_event.tr_err++;
      goto cb_01_error;
    }
    if (buffer_tag == BUFFER_A)
    {
      MLX90640_CalculateTo(frame, &mlx90640, emissivity, tr, mlx90640To, buffer_tag);
      printf("cb_sub=[%d],BUFF_A=[%2.3f],[%2.3f],[%2.3f],[%2.3f],[%2.3f]\r\n",
             subPage, mlx90640To[0], mlx90640To[127], mlx90640To[128], mlx90640To[256], mlx90640To[768 - 1]);
    }
    // spi_state = osSemaphoreRelease(filled_idHandle);
    // if (spi_state != osOK)
    // {
    //   printf("osSemaphoreRelease filled_idHandle 0:%d\r\n", spi_state);
    // }
    mlx_event.i2c++;
    osEventFlagsSet(ThermalEventHandle, i2c_evt_id);
  }
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
  uint8_t check = aRxBuffer[DMA_U8_OUTPUT_SIZE - 1];
  uint8_t type = aRxBuffer[0];
  printf("type=0x%X,end=%X\r\n", type, check);
  // spi_state = osSemaphoreAcquire(filled_idHandle, 0);
  // if (spi_state != osOK)
  // {
  //   printf("osSemaphoreAcquire filled_idHandle 1:%d\r\n", spi_state);
  // }
  if (check == 0xFF)
  {
    mlx_event.spi++;
#if 0
    // while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) != GPIO_PIN_SET) // NSS signal
    // {
    // }
    // if (HAL_SPI_TransmitReceive_DMA(TEST_SPI_ADDRESS, (uint8_t *)mlx90640To, (uint8_t *)aRxBuffer, DMA_U8_OUTPUT_SIZE) != HAL_OK)
    // {
    //   Error_Handler();
    // }
#endif
//     if (buffer_tag == BUFFER_A)
//     {
//       // printf("cb_sub=[%d],BUFF_A=[%2.3f],[%2.3f],[%2.3f],[%2.3f],[%2.3f]\r\n",
//       //        subPage, mlx90640To[0], mlx90640To[127], mlx90640To[128], mlx90640To[256], mlx90640To[768 - 1]);
//       // __NOP();
//     }
  }
  else
  {
    mlx_event.frame_check_err++;
#if DEBUG == 1
    printf("reserved\r\n");
#else
    Error_Handler();
#endif
  }
  // spi_state = osSemaphoreRelease(empty_idHandle);
  // if (spi_state != osOK)
  // {
  //   printf("osSemaphoreRelease empty_idHandle 1:%d\r\n", spi_state);
  // }
}
void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
  HAL_SPI_DMAStop(TEST_SPI_ADDRESS);
  HAL_SPIEx_FlushRxFifo(TEST_SPI_ADDRESS);
  __HAL_RCC_DMA2_CLK_DISABLE();
  HAL_SPI_DeInit(TEST_SPI_ADDRESS);
  __HAL_RCC_DMA2_CLK_ENABLE();
  HAL_SPI_Init(TEST_SPI_ADDRESS);
  while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) != GPIO_PIN_SET) // NSS signal
  {
  }
  if (HAL_SPI_TransmitReceive_DMA(TEST_SPI_ADDRESS, test_ptr, (uint8_t *)aRxBuffer, DMA_U8_OUTPUT_SIZE) != HAL_OK)
  {
    Error_Handler();
  }
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    printf("HAL_SPI_ErrorCallback");
    spi_process();
    BSP_LED_Toggle(LED_GREEN);
    osEventFlagsSet(ThermalEventHandle, spi_evt_id);
  }
}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    printf("HAL_SPI_ErrorCallback");
    Error_Handler();
  }
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi == TEST_SPI_ADDRESS)
  {
    spi_process();
    BSP_LED_Toggle(LED_GREEN);
    osEventFlagsSet(ThermalEventHandle, spi_evt_id);
  }
}
void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
  // printf("rx:%d,tx:%d\r\n", __HAL_DMA_GET_COUNTER(&hdma_spi1_rx), __HAL_DMA_GET_COUNTER(&hdma_spi1_tx));
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
