#include "led.h"
#include "fnd.h"
#include "stepper.h"
#include "button.h"
#include "tim.h"
#include "buzzer.h"
#include "stm32f4xx_hal.h"

/*
 * Prescaler => 500 - 1
 * ARR	     => 100 - 1
 *
 * 2KHz, 0.5ms
 */

// 엘레베이터의 출발, 도착 지연 시간 0.5s
#define ELEVATOR_STARTER_DELAY_MS   	1000
#define ELEVATOR_ARRIVAL_DELAY_MS 	1000

/*
 * ELEVATOR_BLINK_PERIOD_MS : 엘레베이터 도착시 깜빡이는 딜레이 시간 500ms
 * ELEVATOR_BLINK_TOGGLES   : 엘레베이터 도착시 깜빡이는 횟수 on/off 3회
 *
 * ELEVATOR_LED_DIV_STEPS   : 엘레베이터 층 이동간 LED shift 스텝 틱 100ms
 */
#define ELEVATOR_BLINK_PERIOD_MS 500
#define ELEVATOR_BLINK_TOGGLES   6
#define ELEVATOR_LED_DIV_STEPS   100

/* 활성 레벨은 필요 시 변경 */
#define PHOTO_ACTIVE_ON   GPIO_PIN_RESET   // 광차단 시 LOW라면 RESET, 반대면 SET

typedef enum
{
  MOTOR_STOP = 0,
  MOTOR_CW,
  MOTOR_CCW
} MotorState;

void elevatorInit(TIM_HandleTypeDef *htim10);
void Elevator_On1msTick(void);          // TIM10에서 호출
