#ifndef INC_ELEVATOR_H_
#define INC_ELEVATOR_H_

#include "stm32f4xx_hal.h"
#include "led.h"
#include "fnd.h"
#include "stepper.h"
#include "servo.h"
#include "button.h"
#include "buzzer.h"

/** 출발 지연 시간(ms): 버튼 입력 후 모터 구동까지 대기 */
#define ELEVATOR_STARTER_DELAY_MS 1000
/** 도착 지연 시간(ms): 포토 감지 후 문 열림까지 대기 */
#define ELEVATOR_ARRIVAL_DELAY_MS 1000
/** 도착 점멸 주기(ms): blinkPhase 토글 간격 */
#define ELEVATOR_BLINK_PERIOD_MS 500
/** 도착 점멸 횟수(토글 수) */
#define ELEVATOR_BLINK_TOGGLES 6
/** 이동 중 LED 쉬프트 분주(스텝 대비 LED 갱신 간격) */
#define ELEVATOR_LED_DIV_STEPS 100
/** 포토센서 활성 레벨 */
#define PHOTO_ACTIVE_ON GPIO_PIN_RESET

/** 모터 상태 */
typedef enum {
  MOTOR_STOP = 0,  /** 정지 */
  MOTOR_CW,        /** 정회전: 상층 이동 */
  MOTOR_CCW        /** 역회전: 하층 이동 */
} MotorState;

/**
 * 엘리베이터 초기화
 * @param htim 주기 인터럽트 타이머 핸들(1 ms 주기)
 */
void elevatorInit(TIM_HandleTypeDef *htim);

/**
 * 주기 처리 루프(1 ms마다 호출)
 * - 버튼 처리, 출발/도착 지연, 점멸, 부저, 포토 엣지, 모터 스텝
 */
void startingElevatorSystem(void);

/** 외부 가시 상태: 현재층(1~3) */
extern volatile uint8_t currentFloor;
/** 외부 가시 상태: 모터 상태 */
extern volatile MotorState motor;

#endif /* INC_ELEVATOR_H_ */
