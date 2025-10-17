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

//  extern uint8_t currentFloor = 0;
//  extern uint8_t nextFloor = 0;

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STEP_OUTPUT0_Pin GPIO_PIN_0
#define STEP_OUTPUT0_GPIO_Port GPIOA
#define STEP_OUTPUT1_Pin GPIO_PIN_1
#define STEP_OUTPUT1_GPIO_Port GPIOA
#define STEP_OUTPUT2_Pin GPIO_PIN_4
#define STEP_OUTPUT2_GPIO_Port GPIOA
#define LED_OUTPUT0_Pin GPIO_PIN_5
#define LED_OUTPUT0_GPIO_Port GPIOA
#define LED_OUTPUT1_Pin GPIO_PIN_6
#define LED_OUTPUT1_GPIO_Port GPIOA
#define LED_OUTPUT2_Pin GPIO_PIN_7
#define LED_OUTPUT2_GPIO_Port GPIOA
#define EXIT_2F_Pin GPIO_PIN_5
#define EXIT_2F_GPIO_Port GPIOC
#define EXIT_2F_EXTI_IRQn EXTI9_5_IRQn
#define STEP_OUTPUT3_Pin GPIO_PIN_0
#define STEP_OUTPUT3_GPIO_Port GPIOB
#define FND_OUTPUT4_Pin GPIO_PIN_1
#define FND_OUTPUT4_GPIO_Port GPIOB
#define FND_OUTPUT3_Pin GPIO_PIN_2
#define FND_OUTPUT3_GPIO_Port GPIOB
#define LED_OUTPUT7_Pin GPIO_PIN_10
#define LED_OUTPUT7_GPIO_Port GPIOB
#define FND_OUTPUT2_Pin GPIO_PIN_12
#define FND_OUTPUT2_GPIO_Port GPIOB
#define FND_OUTPUT7_Pin GPIO_PIN_13
#define FND_OUTPUT7_GPIO_Port GPIOB
#define FND_OUTPUT6_Pin GPIO_PIN_14
#define FND_OUTPUT6_GPIO_Port GPIOB
#define FND_OUTPUT5_Pin GPIO_PIN_15
#define FND_OUTPUT5_GPIO_Port GPIOB
#define EXIT_1F_Pin GPIO_PIN_6
#define EXIT_1F_GPIO_Port GPIOC
#define EXIT_1F_EXTI_IRQn EXTI9_5_IRQn
#define LED_OUTPUT4_Pin GPIO_PIN_7
#define LED_OUTPUT4_GPIO_Port GPIOC
#define EXIT_0F_Pin GPIO_PIN_8
#define EXIT_0F_GPIO_Port GPIOC
#define EXIT_0F_EXTI_IRQn EXTI9_5_IRQn
#define LED_OUTPUT6_Pin GPIO_PIN_8
#define LED_OUTPUT6_GPIO_Port GPIOA
#define LED_OUTPUT5_Pin GPIO_PIN_9
#define LED_OUTPUT5_GPIO_Port GPIOA
#define FND_OUTPUT1_Pin GPIO_PIN_11
#define FND_OUTPUT1_GPIO_Port GPIOA
#define FND_OUTPUT0_Pin GPIO_PIN_12
#define FND_OUTPUT0_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_15
#define BUZZER_GPIO_Port GPIOA
#define BUTTON_INPUT2_Pin GPIO_PIN_3
#define BUTTON_INPUT2_GPIO_Port GPIOB
#define BUTTON_INPUT0_Pin GPIO_PIN_4
#define BUTTON_INPUT0_GPIO_Port GPIOB
#define BUTTON_INPUT1_Pin GPIO_PIN_5
#define BUTTON_INPUT1_GPIO_Port GPIOB
#define LED_OUTPUT3_Pin GPIO_PIN_6
#define LED_OUTPUT3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
