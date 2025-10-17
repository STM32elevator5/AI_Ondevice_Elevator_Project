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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "button.h"
#include "led.h"
#include "fnd.h"
#include "stepper.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum {
  MOTOR_STOP,
  MOTOR_CW,
  MOTOR_CCW
} MotorState;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t currentFloor = 0;
MotorState motorState = MOTOR_STOP; // 모터 멈추도록 지정
uint16_t motorStepCount = 0;
uint32_t lastStepTime = 0;
const uint16_t stepsPerRevolution = 4096; //
const uint32_t stepDelay = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch(GPIO_Pin){
    case GPIO_PIN_5:  // PC5
      // PC5 처리
      break;
    case GPIO_PIN_6:  // PC6
      // PC6 처리
      break;
    case GPIO_PIN_8:  // PC8
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
      break;
    default:
      break;
  }
}

uint32_t millis()
{
  return HAL_GetTick();
}

static inline void onButton(uint8_t floor)
{
  switch (floor) {
    case 0:
      //      if(nextFloor > currentFloor)
      //	{
      //	  // 위로 올라가는 이벤트
      //	  currentFloor = nextFloor;
      //	}
      //      else if(nextFloor < currentFloor){
      //	  // 내려가는 이벤트
      //      }
      //      ledOn(8);
      //      ledLeftShift(8);
      //      FND_ShowDigit(1);
      motorState = MOTOR_CW;
      motorStepCount = 0;
      lastStepTime = HAL_GetTick();
      break;
    case 1:
      //      ledRightShift(0);
      //      FND_ShowDigit(2);
      motorState = MOTOR_CCW;
      motorStepCount = 0;
      lastStepTime = HAL_GetTick();
      break;
    case 2:
      //      FND_ShowDigit(3);
      motorState = MOTOR_STOP;
      stopStepper();
      break;
  }
}

static inline void processMotorTick(uint32_t now)
{
  if(motorState == MOTOR_STOP) return;
  if(now - lastStepTime < stepDelay) return;

  if(motorState == MOTOR_CW) {
      stepMotorOneStep(DIR_CW);
  } else { // MOTOR_CCW
      stepMotorOneStep(DIR_CCW);
  }

  motorStepCount++;                    // ← 스텝 후 증가(0에서 즉시 %0 되는 문제 방지)

  if((motorStepCount % stepsPerRevolution) == 0) {
      if(motorState == MOTOR_CW) {
	  ledRightShift(0);
	  FND_ShowDigit(2);
      } else {
	  ledLeftShift(8);
	  FND_ShowDigit(3);
      }
  }

  lastStepTime = now;

  if(motorStepCount >= stepsPerRevolution) {
      motorState = MOTOR_STOP;
      stopStepper();
  }
}

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
      for(uint8_t i=0; i<3; ++i) {
	  if(buttonGetPressed(i)) onButton(i);
      }

      // 모터 논블로킹 스텝
      uint32_t now = HAL_GetTick();
      processMotorTick(now);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
