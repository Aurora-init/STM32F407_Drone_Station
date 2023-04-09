/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define STATE1_Pin GPIO_PIN_2
#define STATE1_GPIO_Port GPIOE
#define DIR5_Pin GPIO_PIN_3
#define DIR5_GPIO_Port GPIOE
#define STEP6_Pin GPIO_PIN_5
#define STEP6_GPIO_Port GPIOE
#define DIR6_Pin GPIO_PIN_6
#define DIR6_GPIO_Port GPIOE
#define LED0_Pin GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define DIR7_Pin GPIO_PIN_0
#define DIR7_GPIO_Port GPIOC
#define STEP7_Pin GPIO_PIN_1
#define STEP7_GPIO_Port GPIOC
#define STEP4_Pin GPIO_PIN_6
#define STEP4_GPIO_Port GPIOA
#define STEP1_Pin GPIO_PIN_12
#define STEP1_GPIO_Port GPIOB
#define KEY_NEXT_Pin GPIO_PIN_13
#define KEY_NEXT_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define DIR2_Pin GPIO_PIN_11
#define DIR2_GPIO_Port GPIOD
#define STEP2_Pin GPIO_PIN_12
#define STEP2_GPIO_Port GPIOD
#define DIR1_Pin GPIO_PIN_13
#define DIR1_GPIO_Port GPIOD
#define DIR4_Pin GPIO_PIN_12
#define DIR4_GPIO_Port GPIOA
#define STEP5_Pin GPIO_PIN_3
#define STEP5_GPIO_Port GPIOD
#define DHT11_Pin GPIO_PIN_6
#define DHT11_GPIO_Port GPIOD
#define BEEP_Pin GPIO_PIN_7
#define BEEP_GPIO_Port GPIOD
#define DIR3_Pin GPIO_PIN_10
#define DIR3_GPIO_Port GPIOG
#define STATE2_Pin GPIO_PIN_11
#define STATE2_GPIO_Port GPIOG
#define LED_TEST_Pin GPIO_PIN_15
#define LED_TEST_GPIO_Port GPIOG
#define STEP3_Pin GPIO_PIN_7
#define STEP3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
