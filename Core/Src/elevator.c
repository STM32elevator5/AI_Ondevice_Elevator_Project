/* elevator.c
 * 1 ms 틱 기반 엘리베이터 제어
 */
#include "elevator.h"

/** 틱 타이머 핸들(1 ms 주기) */
static TIM_HandleTypeDef *tickTimer = NULL;

/* === 지연 상태 === */
/** 도착 지연 활성 플래그(1: 대기 중) */
static volatile uint8_t  arrivalPending = 0;
/** 도착 지연 누적(ms), 1 ms 틱마다 +1 */
static volatile uint32_t arrivalMsAcc   = 0;
/** 출발 지연 활성 플래그(1: 대기 중) */
static volatile uint8_t  starterPending = 0;
/** 출발 지연 누적(ms), 1 ms 틱마다 +1 */
static volatile uint32_t starterMsAcc   = 0;
/** 출발 지연 종료 시 적용할 방향 */
static MotorState        motorPending   = MOTOR_STOP;

/* === 부저 상태 === */
/** 부저 ON 스위치(1: 동작 중) */
static volatile uint8_t  buzzerOn    = 0;
/** 부저 ON 누적(ms), 1 ms 틱마다 +1 */
static volatile uint32_t buzzerMsAcc = 0;

/* === 이동 및 표시 상태 === */
/** LED 쉬프트 디바이더용 스텝 카운터 */
static volatile uint8_t  stepCount   = 0;
/** LED 원-핫 위치(0~7, 0xFF 비활성 표현으로 사용 가능) */
static uint8_t           ledPosition = 0xFF;
/** 잔여 점멸 토글 수 */
static volatile uint8_t  blinkRemain = 0;
/** 점멸 누적(ms), 1 ms 틱마다 +1 */
static volatile uint32_t blinkMsAcc  = 0;
/** 점멸 위상(0/1). 1이면 전체 ON, 0이면 전체 OFF */
static uint8_t           blinkPhase  = 0;

/* === 포토센서 직전 상태 === */
static GPIO_PinState prevF1, prevF2, prevF3;

/* === 외부 가시 상태 === */
volatile uint8_t   currentFloor = 1;
volatile MotorState motor       = MOTOR_STOP;
/** 목표 층(1~3) */
static volatile uint8_t targetFloor = 1;

/**
 * 1초 부저 트리거
 * - Buzzer_On() 즉시 실행
 * - BUZZER_ON_MS 경과 시 자동 OFF
 */
static inline void buzzerOneSecOn(void)
{
  Buzzer_On();
  buzzerOn    = 1;
  buzzerMsAcc = 0;
}

/**
 * 층 도착 처리
 * @param arrivedFloor 감지된 층(1~3)
 * - 모터 정지, 현재층 표시, 점멸 시퀀스, 문 열림 지연 예약
 */
static inline void seenFloor(uint8_t arrivedFloor)
{
  motor = MOTOR_STOP;
  stopStepper();

  starterPending = 0;
  starterMsAcc   = 0;

  currentFloor = arrivedFloor;
  fndShowDigit(arrivedFloor);
  buzzerOneSecOn();

  ledPosition = 0xFF;
  ledAllOff();

  blinkRemain = ELEVATOR_BLINK_TOGGLES;
  blinkMsAcc  = ELEVATOR_BLINK_PERIOD_MS; /* 다음 틱에 즉시 토글 유도 */
  blinkPhase  = 0;

  arrivalMsAcc   = 0;
  arrivalPending = 1;
}

/**
 * 목표층으로 이동 예약
 * - 문 닫기 수행 후 출발 지연 시작
 */
static inline void startTowardTarget(void)
{
  if (currentFloor == targetFloor)
    {
      motor = MOTOR_STOP;
      stopStepper();
      return;
    }

  closeDelayDoor();
  motorPending = (currentFloor < targetFloor) ? MOTOR_CW : MOTOR_CCW;

  motor = MOTOR_STOP;
  stopStepper();

  starterPending = 1;
  starterMsAcc   = 0;
}

/**
 * 초기화
 * @param htim 1 ms 주기의 타이머 핸들
 */
void elevatorInit(TIM_HandleTypeDef *htim)
{
  tickTimer = htim;

  fndShowDigit(currentFloor);
  ledPosition = 0xFF;          /* 전체 OFF */
  ledAllOff();
  Buzzer_Off();
  buzzerOn    = 0;
  buzzerMsAcc = 0;

  prevF1 = HAL_GPIO_ReadPin(EXIT_0F_GPIO_Port, EXIT_0F_Pin);
  prevF2 = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  prevF3 = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);

  motor = MOTOR_STOP;
  stopStepper();

  HAL_TIM_Base_Start_IT(tickTimer);  /* 1 ms 주기 IT */
}

/**
 * 주기 처리 루프(1 ms마다 호출)
 * - 버튼→목표층 설정 후 출발 예약
 * - 출발·도착 지연 카운트
 * - 점멸·부저 타이밍
 * - 포토 엣지→도착 처리
 * - 모터 논블로킹 스텝 및 LED 쉬프트
 */
void startingElevatorSystem(void)
{
  /* 버튼 입력 */
  if (buttonGetPressed(0)) { targetFloor = 1; startTowardTarget(); }
  if (buttonGetPressed(1)) { targetFloor = 2; startTowardTarget(); }
  if (buttonGetPressed(2)) { targetFloor = 3; startTowardTarget(); }

  /* 출발 지연 처리(1 ms 틱) */
  if (starterPending) {
      if (++starterMsAcc >= ELEVATOR_STARTER_DELAY_MS) {
	  starterPending = 0;
	  starterMsAcc   = 0;
	  if (currentFloor != targetFloor) motor = motorPending;
      }
  }

  /* 도착 지연 처리(1 ms 틱) */
  if (arrivalPending) {
      if (++arrivalMsAcc >= ELEVATOR_ARRIVAL_DELAY_MS) {
	  arrivalPending = 0;
	  openDelayDoor();
      }
  }

  /* 도착 점멸(1 ms 틱) */
  if (blinkRemain) {
      if (++blinkMsAcc >= ELEVATOR_BLINK_PERIOD_MS) {
	  blinkMsAcc = 0;
	  blinkPhase ^= 1;
	  ledBlinkByPhase(blinkPhase);
	  if (--blinkRemain == 0) {
	      ledPosition = 0xFF;
	      ledAllOff();
	  }
      }
  }

  /* 부저 자동 OFF(1 ms 틱) */
  if (buzzerOn && ++buzzerMsAcc >= BUZZER_ON_MS) {
      Buzzer_Off();
      buzzerOn = 0;
  }

  /* 포토 엣지 처리 */
  GPIO_PinState n1 = HAL_GPIO_ReadPin(EXIT_0F_GPIO_Port, EXIT_0F_Pin);
  if (n1 == PHOTO_ACTIVE_ON && prevF1 != PHOTO_ACTIVE_ON && targetFloor == 1 && motor == MOTOR_CCW)
    seenFloor(1);
  prevF1 = n1;

  GPIO_PinState n2 = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  if (n2 == PHOTO_ACTIVE_ON && prevF2 != PHOTO_ACTIVE_ON && targetFloor == 2) {
      if ((motor == MOTOR_CW  && currentFloor < 2) ||
	  (motor == MOTOR_CCW && currentFloor > 2)) seenFloor(2);
  }
  prevF2 = n2;

  GPIO_PinState n3 = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);
  if (n3 == PHOTO_ACTIVE_ON && prevF3 != PHOTO_ACTIVE_ON && targetFloor == 3 && motor == MOTOR_CW)
    seenFloor(3);
  prevF3 = n3;

  /* 모터 논블로킹 스텝 + LED 쉬프트 */
  if (motor != MOTOR_STOP) {
      if (motor == MOTOR_CW) {
	  stepMotorOneStep(DIR_CW);
	  if ((++stepCount % ELEVATOR_LED_DIV_STEPS) == 0) {
	      ledPosition = (uint8_t)((ledPosition + 7) & 0x7);  /* 우로 1칸 */
	      ledShiftOn(ledPosition);
	  }
      } else {
	  stepMotorOneStep(DIR_CCW);
	  if ((++stepCount % ELEVATOR_LED_DIV_STEPS) == 0) {
	      ledPosition = (uint8_t)((ledPosition + 1) & 0x7);  /* 좌로 1칸 */
	      ledShiftOn(ledPosition);
	  }
      }
  }
}
