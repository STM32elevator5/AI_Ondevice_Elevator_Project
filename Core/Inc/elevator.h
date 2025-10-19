#include "main.h"
#include "led.h"
#include "fnd.h"
#include "stepper.h"
#include "button.h"
#include "tim.h"

/* 기존 정의 유지 */
#define ELEVATOR_BLINK_PERIOD_MS 150
#define ELEVATOR_BLINK_TOGGLES   6
#define ELEVATOR_LED_DIV_STEPS   128
#define STEP_INTERVAL_MS         2

typedef enum
{
  MOTOR_STOP=0,
  MOTOR_CW,
  MOTOR_CCW
} MotorState;

/* 포토센서: 1F=PC6, 2F=PC5 가정. 활성 레벨은 필요 시 변경 */
#define PHOTO_Port        GPIOC
#define PHOTO_1F_Pin      GPIO_PIN_6
#define PHOTO_2F_Pin      GPIO_PIN_5
#define PHOTO_ACTIVE_ON   GPIO_PIN_RESET   // 광차단 시 LOW라면 RESET, 반대면 SET

void Elevator_Init_1ms(TIM_HandleTypeDef *htim_1ms);
void Elevator_StartUp(void);
void Elevator_StartDown(void);
void Elevator_Stop(void);
void Elevator_On1msTick(void);          // TIM10에서 호출
