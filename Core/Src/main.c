/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "communication.h"
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
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart6_tx;

/* Definitions for TaskUART1 */
osThreadId_t TaskUART1Handle;
/**
 * @brief UART1 Communication Task - Handles incoming data from CMM Controller
 * @details Stack increased to 2048 bytes (512*4) to accommodate nested function calls
 *          and local variable storage without stack overflow risk.
 *          Memory usage: 5 tasks × 2KB = 10KB (16% of 64KB available RAM)
 */
const osThreadAttr_t TaskUART1_attributes = {
    .name = "TaskUART1",
    .stack_size = 512 * 4, /* FIXED: Increased from 128*4 (512B) to 512*4 (2048B) to prevent stack overflow */
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for TaskUART6 */
osThreadId_t TaskUART6Handle;
const osThreadAttr_t TaskUART6_attributes = {
    .name = "TaskUART6",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for TaskReadDs */
osThreadId_t TaskReadDsHandle;
const osThreadAttr_t TaskReadDs_attributes = {
    .name = "TaskReadDs",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityAboveNormal,
};
/* Definitions for TaskSYNC */
osThreadId_t TaskSYNCHandle;
const osThreadAttr_t TaskSYNC_attributes = {
    .name = "TaskSYNC",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityRealtime7,
};
/* Definitions for TaskHALT */
osThreadId_t TaskHALTHandle;
const osThreadAttr_t TaskHALT_attributes = {
    .name = "TaskHALT",
    .stack_size = 512 * 4,
    .priority = (osPriority_t)osPriorityRealtime7,
};
/**
 * @brief Signal Mutex - Protects shared resources between HALT and SYNC tasks
 * @details Prevents race conditions when accessing u8IsFirstHaltArrived and u8IsInTp20Measurement
 */
extern osMutexId_t SignalMutexHandle;
const osMutexAttr_t SignalMutex_attributes = {
    .name = "SignalMutex"};
/* Definitions for Uart6TxSem */
osSemaphoreId_t Uart6TxSemHandle;
const osSemaphoreAttr_t Uart6TxSem_attributes = {
    .name = "Uart6TxSem"};
/* Definitions for Uart1TxSem */
osSemaphoreId_t Uart1TxSemHandle;
const osSemaphoreAttr_t Uart1TxSem_attributes = {
    .name = "Uart1TxSem"};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
void StartTaskUART1(void *argument);
void StartTaskUART6(void *argument);
void StartTaskReadDs(void *argument);
void StartTaskSYNC(void *argument);
void StartTaskHALT(void *argument);

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
  HAL_Delay(20000); // Wait for 20 seconds to allow Communication stabilization
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM9_Init();
  MX_TIM5_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  Initialize();
  EmptyUartBuffers();
  HAL_UART_Receive_IT(&huart1, (uint8_t *)au8RxBufCmm.uaiDataArray, 1);
  HAL_UART_Receive_IT(&huart6, (uint8_t *)au8RxBufAcs.uaiDataArray, 1);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of SignalMutex */
  SignalMutexHandle = osMutexNew(&SignalMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Uart6TxSem */
  Uart6TxSemHandle = osSemaphoreNew(1, 1, &Uart6TxSem_attributes);

  /* creation of Uart1TxSem */
  Uart1TxSemHandle = osSemaphoreNew(1, 1, &Uart1TxSem_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskUART1 */
  TaskUART1Handle = osThreadNew(StartTaskUART1, NULL, &TaskUART1_attributes);
  /* FIX: Verify thread creation - if NULL allocation fails, RTOS cannot operate */
  if (TaskUART1Handle == NULL)
    Error_Handler();

  /* creation of TaskUART6 */
  TaskUART6Handle = osThreadNew(StartTaskUART6, NULL, &TaskUART6_attributes);
  /* FIX: Verify thread creation - this task handles critical ACS communication */
  if (TaskUART6Handle == NULL)
    Error_Handler();

  /* creation of TaskReadDs */
  TaskReadDsHandle = osThreadNew(StartTaskReadDs, NULL, &TaskReadDs_attributes);
  /* FIX: Verify thread creation - prevents system crash if memory allocation fails */
  if (TaskReadDsHandle == NULL)
    Error_Handler();

  /* creation of TaskSYNC */
  TaskSYNCHandle = osThreadNew(StartTaskSYNC, NULL, &TaskSYNC_attributes);
  /* FIX: Verify thread creation - synchronization task is real-time critical */
  if (TaskSYNCHandle == NULL)
    Error_Handler();

  /* creation of TaskHALT */
  TaskHALTHandle = osThreadNew(StartTaskHALT, NULL, &TaskHALT_attributes);
  /* FIX: Verify thread creation - handles critical hardware shutdown signals */
  if (TaskHALTHandle == NULL)
    Error_Handler();

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

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

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */
}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 84 - 1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 100 - 1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
}

/**
 * @brief TIM9 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 84 - 1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 1000 - 1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */
}

/**
 * @brief TIM10 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 840;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 10000;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */
}

/**
 * @brief TIM11 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 8400;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 50000;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief USART6 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, ENCB_CV1_Pin | MOTO_EN_24V_Pin | HUB_CV3_Pin | HUB_CV2_Pin | HUB_CV1_Pin | ENCB_CV3_Pin | ENCB_CV2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ENCA_CV1_Pin | ENCA_CV3_Pin | ENCA_CV2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CS_EE2_Pin | Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(INT_ON_CPU_GPIO_Port, INT_ON_CPU_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(AIR_SENSE_CNT_GPIO_Port, AIR_SENSE_CNT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ST_HUB_RESET_Pin | ST_HUB_BOOT_Pin | FRONT_POWER_LED_Pin | FRONT_STOP_LED_Pin | FRONT_READY_LED_Pin | FRONT_ACTIVE_LED_Pin | FRONT_ERROR1_LED_Pin | FRONT_ERROR2_LED_Pin | FRONT_ERROR3_LED_Pin | MARK0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENCB_CV1_Pin MOTO_EN_24V_Pin AIR_SENSE_CNT_Pin HUB_CV3_Pin
                           HUB_CV2_Pin HUB_CV1_Pin ENCB_CV3_Pin ENCB_CV2_Pin */
  GPIO_InitStruct.Pin = ENCB_CV1_Pin | MOTO_EN_24V_Pin | AIR_SENSE_CNT_Pin | HUB_CV3_Pin | HUB_CV2_Pin | HUB_CV1_Pin | ENCB_CV3_Pin | ENCB_CV2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ENCA_CV1_Pin ENCA_CV3_Pin ENCA_CV2_Pin */
  GPIO_InitStruct.Pin = ENCA_CV1_Pin | ENCA_CV3_Pin | ENCA_CV2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SW3_Pin DIP_SW2_Pin DIP_SW1_Pin DIP_SW6_Pin
                           DIP_SW4_Pin */
  GPIO_InitStruct.Pin = DIP_SW3_Pin | DIP_SW2_Pin | DIP_SW1_Pin | DIP_SW6_Pin | DIP_SW4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_EE2_Pin Buzzer_Pin */
  GPIO_InitStruct.Pin = CS_EE2_Pin | Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : EMERG_CPU_Pin */
  GPIO_InitStruct.Pin = EMERG_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(EMERG_CPU_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : INT_ON_CPU_Pin */
  GPIO_InitStruct.Pin = INT_ON_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(INT_ON_CPU_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : COLLISION_CPU_Pin LEDOFF_CPU_Pin PICS_ERROR_CPU_Pin */
  GPIO_InitStruct.Pin = COLLISION_CPU_Pin | LEDOFF_CPU_Pin | PICS_ERROR_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SW7_Pin DIP_SW8_Pin DIP_SW9_Pin STOP_CPU_Pin
                           DIP_SW5_Pin */
  GPIO_InitStruct.Pin = DIP_SW7_Pin | DIP_SW8_Pin | DIP_SW9_Pin | STOP_CPU_Pin | DIP_SW5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ST_HUB_RESET_Pin ST_HUB_BOOT_Pin FRONT_POWER_LED_Pin FRONT_STOP_LED_Pin
                           FRONT_READY_LED_Pin FRONT_ACTIVE_LED_Pin FRONT_ERROR1_LED_Pin FRONT_ERROR2_LED_Pin
                           FRONT_ERROR3_LED_Pin MARK0_Pin */
  GPIO_InitStruct.Pin = ST_HUB_RESET_Pin | ST_HUB_BOOT_Pin | FRONT_POWER_LED_Pin | FRONT_STOP_LED_Pin | FRONT_READY_LED_Pin | FRONT_ACTIVE_LED_Pin | FRONT_ERROR1_LED_Pin | FRONT_ERROR2_LED_Pin | FRONT_ERROR3_LED_Pin | MARK0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DIP_SW10_Pin DIP_SW11_Pin DIP_SW12_Pin */
  GPIO_InitStruct.Pin = DIP_SW10_Pin | DIP_SW11_Pin | DIP_SW12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : MARK1_SYNC_Pin MARK2_HALT_Pin */
  GPIO_InitStruct.Pin = MARK1_SYNC_Pin | MARK2_HALT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PPOFF_CPU_Pin PDUMP_CPU_Pin */
  GPIO_InitStruct.Pin = PPOFF_CPU_Pin | PDUMP_CPU_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTaskUART1 */
/**
 * @brief  Function implementing the TaskUART1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskUART1 */
void StartTaskUART1(void *argument)
{
  /* USER CODE BEGIN 5 */
  (void)argument;
  uint32_t flag;
  /* Infinite loop */
  for (;;)
  {
    flag = osThreadFlagsWait(EVENT_DATA_READY | EVENT_ERROR, osFlagsWaitAny, osWaitForever);
    if (flag == EVENT_DATA_READY)
    {
      vPortEnterCritical();
      uint8_t sizeToCopy = au8RxBufAcs.uSize;
      memcpy(au8TxBufCmm.uaiDataArray, (uint8_t *)au8RxBufAcs.uaiDataArray, sizeToCopy);
      vPortExitCritical();

      if (osSemaphoreAcquire(Uart1TxSemHandle, osWaitForever) == osOK)
      {
        if (SendData(&huart1, au8TxBufCmm.uaiDataArray, sizeToCopy) == HAL_OK)
        {
          HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
          HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
        }
        else
        {
          ResetCommunication();
          EmptyUartBuffers();
        }
      }
      else
      {
        ResetCommunication();
        EmptyUartBuffers();
      }
    }
    else if (flag == EVENT_ERROR)
    {
      ResetCommunication();
      EmptyUartBuffers();
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTaskUART6 */
/**
 * @brief Function implementing the TaskUART6 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskUART6 */
void StartTaskUART6(void *argument)
{
  /* USER CODE BEGIN StartTaskUART6 */
  (void)argument;
  uint32_t flag;
  /* Infinite loop */
  for (;;)
  {
    flag = osThreadFlagsWait(EVENT_DATA_READY | EVENT_ERROR, osFlagsWaitAny, osWaitForever);

    if (flag == EVENT_ERROR)
    {
      ResetCommunication();
      EmptyUartBuffers();
    }

    else if (flag == EVENT_DATA_READY)
    {
      HAL_TIM_Base_Stop_IT(&htim10);

      vPortEnterCritical();
      uint8_t sizeToCopy = au8RxBufCmm.uSize;
      memcpy(au8TxBufAcs.uaiDataArray, (uint8_t *)au8RxBufCmm.uaiDataArray, sizeToCopy);
      vPortExitCritical();

      if (osSemaphoreAcquire(Uart6TxSemHandle, osWaitForever) == osOK)
      {
        if (SendData(&huart6, au8TxBufAcs.uaiDataArray, sizeToCopy) == HAL_OK)
        {
          HAL_UART_Receive_IT(&huart6, (uint8_t *)&au8RxBufAcs.uaiDataArray[0], 1);
          HAL_UART_Receive_IT(&huart1, (uint8_t *)&au8RxBufCmm.uaiDataArray[0], 1);
        }
        else
        {
          ResetCommunication();
          EmptyUartBuffers();
        }
      }
    }
  }
  /* USER CODE END StartTaskUART6 */
}

/* USER CODE BEGIN Header_StartTaskReadDs */
/**
 * @brief Function implementing the TaskReadDs thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskReadDs */
void StartTaskReadDs(void *argument)
{
  /* USER CODE BEGIN StartTaskReadDs */
  (void)argument;
  /* Infinite loop */
  for (;;)
  {
    DipSwitchStatus();
    Tp20CheckForError();
    osDelay(1);
  }
  /* USER CODE END StartTaskReadDs */
}

/* USER CODE BEGIN Header_StartTaskSYNC */
/**
 * @brief Function implementing the TaskSYNC thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskSYNC */
void StartTaskSYNC(void *argument)
{
  /* USER CODE BEGIN StartTaskSYNC */
  (void)argument;
  /* Infinite loop */
  for (;;)
  {
    uint32_t event_flags = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    /** @brief Process SYNC_CPU interrupt - initiates measurement synchronization */
    if (osMutexAcquire(SignalMutexHandle, osWaitForever) == osOK)
    {
      /* FIX: Handle EVENT_ERROR signal from ISR timeout (line 215 in stm32f4xx_it.c)
         This is the safe place to call ResetSynkAndHalt() with mutex protection */
      if (event_flags & EVENT_ERROR)
      {
        ResetSynkAndHalt();
      }

      // Interrupt SYNK_CPU
      if (HAL_GPIO_ReadPin(MARK1_SYNC_GPIO_Port, MARK1_SYNC_Pin) == IN_SNYC_ON) // falling
      {
        if ((!u8IsFirstSnycArrived) && (gpPdumpStatus == IN_PDAMP_OFF))
        {
          u8IsFirstSnycArrived = TRUE;
          u8IsInTp20Measurement = 1;
          TriggerToAcsControl(GPIO_PIN_SET);
          u161MilliSecondCounter = 0;
        }
      }
      else if (u8IsFirstHaltArrived) // rising
      {
        ResetSynkAndHalt();
      }
      osMutexRelease(SignalMutexHandle);
    }
  }
  /* USER CODE END StartTaskSYNC */
}

/* USER CODE BEGIN Header_StartTaskHALT */
/**
 * @brief Function implementing the TaskHALT thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskHALT */
void StartTaskHALT(void *argument)
{
  /* USER CODE BEGIN StartTaskHALT */
  (void)argument;
  /* Infinite loop */
  for (;;)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    /** @brief Process HALT_CPU interrupt - triggers measurement shutdown sequence */
    /* FIX: Mutex MUST be released ONLY if successfully acquired
       Moving release inside if-block prevents dangling release if acquire fails */
    if (osMutexAcquire(SignalMutexHandle, osWaitForever) == osOK)
    {
      if ((!u8IsFirstHaltArrived) && (u8IsInTp20Measurement) && (u161MilliSecondCounter >= SNYK_WAIT_TIME) && (u161MilliSecondCounter <= HALT_TIME_OFF))
      {
        u8IsFirstHaltArrived = TRUE;
        TriggerToAcsControl(GPIO_PIN_SET);
      }
      osMutexRelease(SignalMutexHandle);
    }
  }
  /* USER CODE END StartTaskHALT */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM2 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2)
  {
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
