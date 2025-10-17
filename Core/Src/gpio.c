/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, STEP_OUTPUT0_Pin|STEP_OUTPUT1_Pin|STEP_OUTPUT2_Pin|LED_OUTPUT0_Pin
                          |LED_OUTPUT1_Pin|LED_OUTPUT2_Pin|LED_OUTPUT6_Pin|LED_OUTPUT5_Pin
                          |FND_OUTPUT1_Pin|FND_OUTPUT0_Pin|BUZZER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, STEP_OUTPUT3_Pin|FND_OUTPUT4_Pin|FND_OUTPUT3_Pin|LED_OUTPUT7_Pin
                          |FND_OUTPUT2_Pin|FND_OUTPUT7_Pin|FND_OUTPUT6_Pin|FND_OUTPUT5_Pin
                          |LED_OUTPUT3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_OUTPUT4_GPIO_Port, LED_OUTPUT4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : STEP_OUTPUT0_Pin STEP_OUTPUT1_Pin STEP_OUTPUT2_Pin LED_OUTPUT0_Pin
                           LED_OUTPUT1_Pin LED_OUTPUT2_Pin LED_OUTPUT6_Pin LED_OUTPUT5_Pin
                           FND_OUTPUT1_Pin FND_OUTPUT0_Pin BUZZER_Pin */
  GPIO_InitStruct.Pin = STEP_OUTPUT0_Pin|STEP_OUTPUT1_Pin|STEP_OUTPUT2_Pin|LED_OUTPUT0_Pin
                          |LED_OUTPUT1_Pin|LED_OUTPUT2_Pin|LED_OUTPUT6_Pin|LED_OUTPUT5_Pin
                          |FND_OUTPUT1_Pin|FND_OUTPUT0_Pin|BUZZER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EXIT_2F_Pin EXIT_1F_Pin EXIT_0F_Pin */
  GPIO_InitStruct.Pin = EXIT_2F_Pin|EXIT_1F_Pin|EXIT_0F_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : STEP_OUTPUT3_Pin FND_OUTPUT4_Pin FND_OUTPUT3_Pin LED_OUTPUT7_Pin
                           FND_OUTPUT2_Pin FND_OUTPUT7_Pin FND_OUTPUT6_Pin FND_OUTPUT5_Pin
                           LED_OUTPUT3_Pin */
  GPIO_InitStruct.Pin = STEP_OUTPUT3_Pin|FND_OUTPUT4_Pin|FND_OUTPUT3_Pin|LED_OUTPUT7_Pin
                          |FND_OUTPUT2_Pin|FND_OUTPUT7_Pin|FND_OUTPUT6_Pin|FND_OUTPUT5_Pin
                          |LED_OUTPUT3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_OUTPUT4_Pin */
  GPIO_InitStruct.Pin = LED_OUTPUT4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_OUTPUT4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON_INPUT2_Pin BUTTON_INPUT0_Pin BUTTON_INPUT1_Pin */
  GPIO_InitStruct.Pin = BUTTON_INPUT2_Pin|BUTTON_INPUT0_Pin|BUTTON_INPUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
