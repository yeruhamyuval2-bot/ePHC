/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MSGlobals.h"
#include "Utilies.h"
#include "Communication.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENCB_CV1_Pin GPIO_PIN_3
#define ENCB_CV1_GPIO_Port GPIOE
#define MOTO_EN_24V_Pin GPIO_PIN_4
#define MOTO_EN_24V_GPIO_Port GPIOE
#define ENCA_CV1_Pin GPIO_PIN_13
#define ENCA_CV1_GPIO_Port GPIOC
#define DIP_SW3_Pin GPIO_PIN_0
#define DIP_SW3_GPIO_Port GPIOC
#define DIP_SW2_Pin GPIO_PIN_1
#define DIP_SW2_GPIO_Port GPIOC
#define DIP_SW1_Pin GPIO_PIN_2
#define DIP_SW1_GPIO_Port GPIOC
#define DIP_SW6_Pin GPIO_PIN_3
#define DIP_SW6_GPIO_Port GPIOC
#define CS_EE2_Pin GPIO_PIN_0
#define CS_EE2_GPIO_Port GPIOA
#define EMERG_CPU_Pin GPIO_PIN_4
#define EMERG_CPU_GPIO_Port GPIOA
#define DIP_SW4_Pin GPIO_PIN_5
#define DIP_SW4_GPIO_Port GPIOC
#define INT_ON_CPU_Pin GPIO_PIN_0
#define INT_ON_CPU_GPIO_Port GPIOB
#define COLLISION_CPU_Pin GPIO_PIN_1
#define COLLISION_CPU_GPIO_Port GPIOB
#define DIP_SW7_Pin GPIO_PIN_7
#define DIP_SW7_GPIO_Port GPIOE
#define DIP_SW8_Pin GPIO_PIN_8
#define DIP_SW8_GPIO_Port GPIOE
#define DIP_SW9_Pin GPIO_PIN_9
#define DIP_SW9_GPIO_Port GPIOE
#define AIR_SENSE_CNT_Pin GPIO_PIN_10
#define AIR_SENSE_CNT_GPIO_Port GPIOE
#define HUB_CV3_Pin GPIO_PIN_11
#define HUB_CV3_GPIO_Port GPIOE
#define HUB_CV2_Pin GPIO_PIN_12
#define HUB_CV2_GPIO_Port GPIOE
#define HUB_CV1_Pin GPIO_PIN_13
#define HUB_CV1_GPIO_Port GPIOE
#define STOP_CPU_Pin GPIO_PIN_14
#define STOP_CPU_GPIO_Port GPIOE
#define DIP_SW5_Pin GPIO_PIN_15
#define DIP_SW5_GPIO_Port GPIOE
#define LEDOFF_CPU_Pin GPIO_PIN_10
#define LEDOFF_CPU_GPIO_Port GPIOB
#define ST_HUB_RESET_Pin GPIO_PIN_8
#define ST_HUB_RESET_GPIO_Port GPIOD
#define ST_HUB_BOOT_Pin GPIO_PIN_10
#define ST_HUB_BOOT_GPIO_Port GPIOD
#define DIP_SW10_Pin GPIO_PIN_11
#define DIP_SW10_GPIO_Port GPIOD
#define DIP_SW11_Pin GPIO_PIN_12
#define DIP_SW11_GPIO_Port GPIOD
#define DIP_SW12_Pin GPIO_PIN_13
#define DIP_SW12_GPIO_Port GPIOD
#define MARK1_SYNC_Pin GPIO_PIN_14
#define MARK1_SYNC_GPIO_Port GPIOD
#define MARK1_SYNC_EXTI_IRQn EXTI15_10_IRQn
#define MARK2_HALT_Pin GPIO_PIN_15
#define MARK2_HALT_GPIO_Port GPIOD
#define MARK2_HALT_EXTI_IRQn EXTI15_10_IRQn
#define ENCA_CV3_Pin GPIO_PIN_8
#define ENCA_CV3_GPIO_Port GPIOC
#define ENCA_CV2_Pin GPIO_PIN_9
#define ENCA_CV2_GPIO_Port GPIOC
#define Buzzer_Pin GPIO_PIN_8
#define Buzzer_GPIO_Port GPIOA
#define FRONT_POWER_LED_Pin GPIO_PIN_0
#define FRONT_POWER_LED_GPIO_Port GPIOD
#define FRONT_STOP_LED_Pin GPIO_PIN_1
#define FRONT_STOP_LED_GPIO_Port GPIOD
#define FRONT_READY_LED_Pin GPIO_PIN_2
#define FRONT_READY_LED_GPIO_Port GPIOD
#define FRONT_ACTIVE_LED_Pin GPIO_PIN_3
#define FRONT_ACTIVE_LED_GPIO_Port GPIOD
#define FRONT_ERROR1_LED_Pin GPIO_PIN_4
#define FRONT_ERROR1_LED_GPIO_Port GPIOD
#define FRONT_ERROR2_LED_Pin GPIO_PIN_5
#define FRONT_ERROR2_LED_GPIO_Port GPIOD
#define FRONT_ERROR3_LED_Pin GPIO_PIN_6
#define FRONT_ERROR3_LED_GPIO_Port GPIOD
#define MARK0_Pin GPIO_PIN_7
#define MARK0_GPIO_Port GPIOD
#define PPOFF_CPU_Pin GPIO_PIN_5
#define PPOFF_CPU_GPIO_Port GPIOB
#define PPOFF_CPU_EXTI_IRQn EXTI9_5_IRQn
#define PDUMP_CPU_Pin GPIO_PIN_8
#define PDUMP_CPU_GPIO_Port GPIOB
#define PDUMP_CPU_EXTI_IRQn EXTI9_5_IRQn
#define PICS_ERROR_CPU_Pin GPIO_PIN_9
#define PICS_ERROR_CPU_GPIO_Port GPIOB
#define ENCB_CV3_Pin GPIO_PIN_0
#define ENCB_CV3_GPIO_Port GPIOE
#define ENCB_CV2_Pin GPIO_PIN_1
#define ENCB_CV2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
