#include "lib_bsp.h"
#include "stlogo.h"

uint8_t DemoIndex = 0;
uint8_t NbLoop = 1;
uint32_t i, PsramTest = 0;
/* Global variables ----------------------------------------------------------*/
uint8_t SDDetectIT = 0;
/* Global extern variables ---------------------------------------------------*/
#ifndef USE_FULL_ASSERT
uint16_t ErrorCounter = 0;
#endif
/* Private function prototypes -----------------------------------------------*/

BSP_DemoTypedef BSP_examples[] =
{
  {LCD_demo, "LCD", 0},
  {Touchscreen_demo1, "TS Polling demo", 0}, //TODO only can start twice?
  // {Touchscreen_demo2, "TS Dual Touch demo", 0},
  // {Touchscreen_demo3, "TS Interrupt demo", 0},
  // {Log_demo, "LCD LOG", 0},
  {PSRAM_demo, "PSRAM", 0},
  // {PSRAM_DMA_demo, "PSRAM DMA", 0},
  {QSPI_demo, "QSPI", 0},
  // {AudioPlay_demo, "AUDIO PLAY", 0},
  // {AudioLoopback_demo, "AUDIO REC PLAYBACK", 0},
};

uint32_t COUNT_OF_EXAMPLE_SIZE = sizeof(BSP_examples) / sizeof(BSP_DemoTypedef);

/**
  * @brief  Display main demo messages.
  * @param  None
  * @retval None
  */
void Display_DemoDescription(void)
{
  uint8_t desc[50];

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 2, (uint8_t *)"STM32F723E BSP", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 14, (uint8_t *)"Drivers examples", CENTER_MODE);

  /* Draw Bitmap */
  BSP_LCD_DrawBitmap((BSP_LCD_GetXSize() - 80) / 2, 30, (uint8_t *)stlogo);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 12, (uint8_t *)"STMicroelectronics 2016", CENTER_MODE);

  BSP_LCD_SetFont(&Font12);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, BSP_LCD_GetYSize() / 2 + 1, BSP_LCD_GetXSize(), 60);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 20, (uint8_t *)"Press button to start :", CENTER_MODE);
  sprintf((char *)desc, "%s example", BSP_examples[DemoIndex].DemoName);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() / 2 + 35, (uint8_t *)desc, CENTER_MODE);
}

/**
  * @brief  Check for user input.
  * @param  None
  * @retval Input state (1 : active / 0 : Inactive)
  */
uint8_t CheckForUserInput(void)
{
  if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  {
    HAL_Delay(10);
    while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);
    return 1 ;
  }
  return 0;
}

/**
 * @brief  Configure the MPU attributes as Write Back for PSRAM mapped on FMC
 *         BANK1/BANK2. This function configures two regions.
 * @note   The Base Address is 0x64000000 (for Display purposes) and
 *         0x60000000 (for Audio Record module).
 *         The Region Size is 512KB, it is related to PSRAM memory size.
 * @param  None
 * @retval None
 */
void MPU_ConfigPSRAM(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes for PSRAM with recomended configurations:
     Normal memory, Shareable, write-back (Display purposes) */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x64000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes for PSRAM with recomended configurations:
     Normal memory, Shareable, write-back */
  MPU_InitStruct.BaseAddress = 0x60000000;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

void testQQ(void)
{
  if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  {
    HAL_Delay(10);
    while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);   /* Wait for button released */

    BSP_examples[DemoIndex++].DemoFunc();

    if (DemoIndex >= COUNT_OF_EXAMPLE(BSP_examples))
    {
      /* Increment number of loops which be used by EEPROM example */
      NbLoop++;
      DemoIndex = 0;
    }
    Display_DemoDescription();
  }
  BSP_LED_Toggle(LED6);
}
