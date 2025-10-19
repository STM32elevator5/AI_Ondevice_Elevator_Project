#include "elevator.h"
#include "led.h"
#include "fnd.h"
#include "button.h"
#include "stm32f4xx_hal.h"   // MCU에 맞게 조정

/* ===== 기본 파라미터(헤더에 없으면 기본값 사용) ===== */
#ifndef STEP_INTERVAL_MS
#define STEP_INTERVAL_MS           4u     // 스텝 간격 4ms 예시
#endif
#ifndef ELEVATOR_LED_DIV_STEPS
#define ELEVATOR_LED_DIV_STEPS     8u     // 스텝 8번마다 LED 1칸 이동
#endif
#ifndef ELEVATOR_BLINK_PERIOD_MS
#define ELEVATOR_BLINK_PERIOD_MS   150u   // 도착 점멸 주기
#endif
#ifndef PHOTO_ACTIVE_ON
#define PHOTO_ACTIVE_ON            GPIO_PIN_RESET   // 포토센서 기본 활성 레벨(일반적으로 Low)
#endif

/* ===== 포토센서 핀 매핑: ioc 매크로 사용 ===== */
#ifndef EXIT_1F_GPIO_Port
#define EXIT_1F_GPIO_Port          GPIOC
#endif
#ifndef EXIT_2F_GPIO_Port
#define EXIT_2F_GPIO_Port          GPIOC
#endif

/* ===== 외부에 노출되는 상태 ===== */
volatile uint8_t   currentFloor = 1;
volatile MotorState motor       = MOTOR_STOP;

/* ===== 내부 상태 ===== */
static TIM_HandleTypeDef *s_1ms = NULL;
static volatile uint32_t step_ms_acc = 0;
static volatile uint32_t stepCount   = 0;

/* 원-핫 LED 시프트 상태 */
static uint8_t s_ledPos = 0;
static inline void LED_WriteOneHot(uint8_t pos){
  for (uint8_t i=0;i<8;i++) { if (i==pos) ledOn(i); else ledOff(i); }
}

/* 도착 점멸: 2회(토글 4번) */
static volatile uint8_t  blinkRemain = 0;
static volatile uint32_t blinkMsAcc  = 0;
static uint8_t           blinkPhase  = 0;   // 0=OFF, 1=ON
#ifndef ELEVATOR_BLINK_TOGGLES
#define ELEVATOR_BLINK_TOGGLES      4u     // 2번 깜빡 = 4토글
#endif

/* 포토센서 엣지 검출용 이전 상태 */
static GPIO_PinState prev1F;
static GPIO_PinState prev2F;

/* ===== 내부 유틸 ===== */
static inline void handleArrival(uint8_t floor1based){
  /* 정지 */
  motor = MOTOR_STOP;
  stopStepper();

  /* 층 표시 */
  currentFloor = floor1based;
  FND_ShowDigit(floor1based);

  /* 점멸 무장 */
  blinkRemain = ELEVATOR_BLINK_TOGGLES;
  blinkMsAcc  = ELEVATOR_BLINK_PERIOD_MS;  // 즉시 토글 유도
  blinkPhase  = 0;
}

/* ===== 외부 API ===== */
void Elevator_Init_1ms(TIM_HandleTypeDef *htim_1ms){
  s_1ms = htim_1ms;

  /* 초기 표시 */
  FND_ShowDigit(currentFloor);
  s_ledPos = 0;
  LED_WriteOneHot(s_ledPos);

  /* 현재 센서 레벨을 기준으로 저장(가짜 엣지 방지) */
  prev1F = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  prev2F = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);

  /* 정지 보장 + 타이머 시작 */
  motor = MOTOR_STOP;
  stopStepper();
  HAL_TIM_Base_Start_IT(s_1ms);
}

void Elevator_StartUp(void){
  motor = MOTOR_CW;
  step_ms_acc = 0;
  stepCount   = 0;
}

void Elevator_StartDown(void){
  motor = MOTOR_CCW;
  step_ms_acc = 0;
  stepCount   = 0;
}

void Elevator_Stop(void){
  motor = MOTOR_STOP;
  stopStepper();
  blinkRemain = 0;  // 모든 효과 중단
}

/* EXTI 사용 시 main.c의 HAL_GPIO_EXTI_Callback()에서 호출 가능 */
void Elevator_OnPhotoEXTI(uint16_t GPIO_Pin){
  if (motor == MOTOR_CW)
    {
      if (GPIO_Pin == EXIT_1F_Pin) handleArrival(1);
    }
  else if (motor == MOTOR_CCW)
    {
      if (GPIO_Pin == EXIT_2F_Pin) handleArrival(2);
    }
}


/* 1 ms 주기 호출 필수: TIMx IRQ → HAL_TIM_PeriodElapsedCallback()에서 호출 */
void Elevator_On1msTick(void){
  /* 버튼: 0=정지, 1=상행, 2=하행 */
  if (buttonGetPressed(0)) Elevator_Stop();
  if (buttonGetPressed(1)) Elevator_StartUp();
  if (buttonGetPressed(2)) Elevator_StartDown();

  /* 스텝 모터 논블로킹 분주 */
  if (motor != MOTOR_STOP){
      if (++step_ms_acc >= STEP_INTERVAL_MS){
	  step_ms_acc = 0;

	  if (motor == MOTOR_CW){
	      stepMotorOneStep(DIR_CW);
	      if ((++stepCount % ELEVATOR_LED_DIV_STEPS) == 0){
		  s_ledPos = (uint8_t)((s_ledPos + 7) & 0x7);  // Right 1칸
		  LED_WriteOneHot(s_ledPos);
	      }
	  } else {
	      stepMotorOneStep(DIR_CCW);
	      if ((++stepCount % ELEVATOR_LED_DIV_STEPS) == 0){
		  s_ledPos = (uint8_t)((s_ledPos + 1) & 0x7);  // Left 1칸
		  LED_WriteOneHot(s_ledPos);
	      }
	  }
      }
  }

  /* 도착 점멸: 실제 토글 수행 후 종료 시 원-핫 복구 */
  if (blinkRemain){
      if (blinkMsAcc >= ELEVATOR_BLINK_PERIOD_MS){
	  blinkMsAcc = 0;
	  blinkPhase ^= 1;
	  for (uint8_t i=0;i<8;i++){ if (blinkPhase) ledOn(i); else ledOff(i); }
	  if (--blinkRemain == 0) LED_WriteOneHot(s_ledPos);
      } else {
	  blinkMsAcc++;
      }
  }

  /* 포토센서 엣지 폴링(모터 구동 중에만 유효) */
  GPIO_PinState now1 = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  if (motor != MOTOR_STOP && now1 == PHOTO_ACTIVE_ON && prev1F != PHOTO_ACTIVE_ON){
      handleArrival(1);
  }
  prev1F = now1;

  GPIO_PinState now2 = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);
  if (motor != MOTOR_STOP && now2 == PHOTO_ACTIVE_ON && prev2F != PHOTO_ACTIVE_ON){
      handleArrival(2);
  }
  prev2F = now2;
}
