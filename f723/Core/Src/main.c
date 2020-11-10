/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "i2c.h"
#include "quadspi.h"
#include "sai.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define  FPS2HZ   0x02
#define  FPS4HZ   0x03
#define  FPS8HZ   0x04
#define  FPS16HZ  0x05
#define  FPS32HZ  0x06
#define  FPS64HZ  0x07

#define  MLX90640_ADDR 0x33
#define	 RefreshRate FPS32HZ
#define  TA_SHIFT 8 //Default shift for MLX90640 in open air

static uint16_t eeMLX90640[832];
static float mlx90640To[768];
uint16_t frame[834];
float emissivity=0.95;
int status;

paramsMLX90640 mlx90640;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_QUADSPI_Init();
  MX_SAI2_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_UART7_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  status = MLX90640_SetRefreshRate(MLX90640_ADDR, RefreshRate);
  if (status != 0)
  {
    printf("\r\nSetRefreshRate error with code:%d\r\n",status);
    while (1);
  }
	status = MLX90640_SetChessMode(MLX90640_ADDR);
  if (status != 0)
  {
    printf("\r\nSetChessMode error with code:%d\r\n",status);
    while (1);
  }
  status = MLX90640_DumpEE(MLX90640_ADDR, eeMLX90640);
  if (status != 0)
  {
    printf("\r\nload system parameters error with code:%d\r\n",status);
    while (1);
  }
  status = MLX90640_ExtractParameters(eeMLX90640, &mlx90640);
  if (status != 0)
  {
    printf("\r\nParameter extraction failed with error code:%d\r\n",status);
    while (1);
  }
  int a = 0, b = 0;
  while (1)
  {
    // HAL_Delay(20);
    int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
    if (status < 0)
    {
      printf("GetFrame Error: %d\r\n",status);
    }
    else
    {
      ;
      // a++;
      // printf("a=%d\r\n",a);
    }
    float vdd = MLX90640_GetVdd(frame, &mlx90640);
    float Ta = MLX90640_GetTa(frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    //printf("vdd:  %f Tr: %f\r\n",vdd,tr);
    MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);
    b++;
    printf("b=%d\r\n",b);
    // printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
    // for(int i = 0; i < 768; i++){
    //   if(i%32 == 0 && i != 0){
    //     printf("\r\n");
    //   }
    // printf("%2.2f ",mlx90640To[0]);
    // }
    // printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
    if (status < 0)
    {
      //printf("GetFrame Error: %d\r\n",status);
    }
    float vdd = MLX90640_GetVdd(frame, &mlx90640);
    float Ta = MLX90640_GetTa(frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    //printf("vdd:  %f Tr: %f\r\n",vdd,tr);
    MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);

    // printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
    // for(int i = 0; i < 768; i++){
    //   if(i%32 == 0 && i != 0){
    //     printf("\r\n");
    //   }
    printf("%2.2f ",mlx90640To[0]);
    // }
    // printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
    if (status < 0)
    {
      //printf("GetFrame Error: %d\r\n",status);
    }
    float vdd = MLX90640_GetVdd(frame, &mlx90640);
    float Ta = MLX90640_GetTa(frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    //printf("vdd:  %f Tr: %f\r\n",vdd,tr);
    MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);

    // printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
    // for(int i = 0; i < 768; i++){
    //   if(i%32 == 0 && i != 0){
    //     printf("\r\n");
    //   }
    printf("%2.2f ",mlx90640To[0]);
    // }
    // printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
    if (status < 0)
    {
      //printf("GetFrame Error: %d\r\n",status);
    }
    float vdd = MLX90640_GetVdd(frame, &mlx90640);
    float Ta = MLX90640_GetTa(frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    //printf("vdd:  %f Tr: %f\r\n",vdd,tr);
    MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);

    // printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
    // for(int i = 0; i < 768; i++){
    //   if(i%32 == 0 && i != 0){
    //     printf("\r\n");
    //   }
    printf("%2.2f\r\n ",mlx90640To[0]);
    // }

    // printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    int status = MLX90640_GetFrameData(MLX90640_ADDR, frame);
    if (status < 0)
    {
      //printf("GetFrame Error: %d\r\n",status);
    }
    float vdd = MLX90640_GetVdd(frame, &mlx90640);
    float Ta = MLX90640_GetTa(frame, &mlx90640);

    float tr = Ta - TA_SHIFT; //Reflected temperature based on the sensor ambient temperature
    //printf("vdd:  %f Tr: %f\r\n",vdd,tr);
    MLX90640_CalculateTo(frame, &mlx90640, emissivity , tr, mlx90640To);

    printf("\r\n==========================IAMLIUBO MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
    for(int i = 0; i < 768; i++){
      if(i%32 == 0 && i != 0){
        printf("\r\n");
      }
      printf("%2.2f ",mlx90640To[i]);
    }
    printf("\r\n==========================IAMLIUB0 MLX90640 WITH STM32 SWI2C EXAMPLE Github:github.com/imliubo==========================\r\n");
  }
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART6
                              |RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_UART7|RCC_PERIPHCLK_SAI2
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV2;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.Sai2ClockSelection = RCC_SAI2CLKSOURCE_PLLSAI;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
