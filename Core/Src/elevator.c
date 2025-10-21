#include "elevator.h"

// === 도착(포토) 지연 실행 ===

/*
 * arrivalPending : 도착했는지 여부의 값 / 0 => 도착 x / 1 => 도착 o
 * arrivalMsAcc	  : 도착했을 때 문 열림 딜레이 틱 / 틱은 값이 매우 빠르게 증가해서 32bit로 여유롭게 설정
 */
static volatile uint8_t		arrivalPending = 0;
static volatile uint32_t	arrivalMsAcc   = 0;

/*
 * starterPending : 출발했는지 여부의 값 / 0 => 출발 x / 1 => 출발 0
 * starterMsAcc   : 시작했을 때 스텝모터에 대한 틱
 * motorPending   : 모터가 위로 올라갈지 아래로 내려갈지 상태의 값만 저장 실제 모터의 움직임은 motor라는 변수에 의해 작동함
 */
static volatile uint8_t  	starterPending = 0;
static volatile uint32_t	starterMsAcc   = 0;
static MotorState        	motorPending   = MOTOR_STOP;

/*
 * buzzerOn	: 부저 스위치 변수 0 => 부저 x / 1 => 부저 on
 * buzzerMsAcc	: 부저에 대한 틱
 */
static volatile uint8_t  	buzzerOn = 0;
static volatile uint32_t 	buzzerMsAcc = 0;

/* === 내부 상태 === */
/*
 * timerTick	: ioc에서 설정한 TIM10번의 틱 (1ms)
 * stepCount	: LED 쉬프트동작 틱
 */
static TIM_HandleTypeDef 	*timerTick = NULL;
static volatile uint8_t 	stepCount = 0;

/* === 외부 상태 === */
volatile uint8_t   		currentFloor = 1;
// 실제 스텝모터의 동작 변수
volatile MotorState 		motor = MOTOR_STOP;
static volatile uint8_t  targetFloor = 1;
/*
 * blinkRemain : 점멸 횟수
 * blinkMsAcc : 점멸 되는 딜레이 틱 500ms
 * blinkPhase : 0 1되면서 ledon(), ledoff() 토글처리
 */
static volatile uint8_t  blinkRemain = 0;
static volatile uint32_t blinkMsAcc  = 0;
static uint8_t           blinkPhase  = 0;

// 이걸 잘 모르겠음...
static GPIO_PinState prevF1, prevF2, prevF3;

/* === LED 원-핫 코딩 === */
static uint8_t ledPosition = 0;
static inline void LED_WriteOneHot(uint8_t position)
{
  for(uint8_t i = 0 ; i < 8 ; i++)
    {
      if(i==position) ledOn(i);
      else ledOff(i);
    }
}

/* === 유틸 === */
static inline void openDelayDoor()
{
  TIM11->CCR1 = 150;                  // 서보 후행 동작
}

/*
 * 부저 on 함수 0.5s뒤에 꺼짐
 */
static inline void Buzzer_Beep500ms(void){
  Buzzer_On();
  buzzerOn = 1;
  buzzerMsAcc = 0;
}

/*
 * f = 도착한 층
 */
static inline void seenFloor(uint8_t arrivedFloor)
{
  motor = MOTOR_STOP;
  stopStepper();

  starterPending = 0;          // 출발 예약 취소 ← 중요
  starterMsAcc   = 0;

  currentFloor = arrivedFloor;
  FND_ShowDigit(arrivedFloor);
  Buzzer_Beep500ms();

  blinkRemain = ELEVATOR_BLINK_TOGGLES;
  blinkMsAcc  = ELEVATOR_BLINK_PERIOD_MS;   // 즉시 1토글 유도
  blinkPhase  = 0;

  // 즉시 실행 대신 지연 예약
  arrivalMsAcc   = 0;
  arrivalPending = 1;
}

static inline void startTowardTarget(void)
{
  if (currentFloor == targetFloor)
    {
      motor = MOTOR_STOP;
      stopStepper();
      return;
  }

  // 문 닫힘 먼저
  TIM11->CCR1 = 50;

  // 방향은 "지금" 계산해 저장
  motorPending = (currentFloor < targetFloor) ? MOTOR_CW : MOTOR_CCW;

  // 모터는 아직 정지
  motor = MOTOR_STOP;
  stopStepper();

  // 지연 타이머 무장
  starterPending = 1;
  starterMsAcc   = 0;
}

/*
 * elevatorInit 실제 틱 0.5ms임
 * htim10 TIM10번의 주소 값을 사용하겠다
 * */
void elevatorInit(TIM_HandleTypeDef *htim10)
{
  // timerTick값은 0.5ms 마다 증가
  timerTick = htim10;

  // 초기 표시
  FND_ShowDigit(currentFloor);
  ledPosition = -1;
  LED_WriteOneHot(ledPosition);
  Buzzer_Off();
  buzzerOn = 0;
  buzzerMsAcc = 0;

  // 가짜 엣지 방지: 현재 레벨 저장
  prevF1 = HAL_GPIO_ReadPin(EXIT_0F_GPIO_Port, EXIT_0F_Pin);
  prevF2 = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  prevF3 = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);

  motor = MOTOR_STOP;
  stopStepper();
  HAL_TIM_Base_Start_IT(timerTick);
}

/* 1 ms 틱 */
void Elevator_On1msTick(){
  /* 버튼: 0→1층, 1→2층, 2→3층 */
  if (buttonGetPressed(0))
    {
      targetFloor = 1;
      startTowardTarget();
    }
  if (buttonGetPressed(1))
    {
      targetFloor = 2;
      startTowardTarget();
    }
  if (buttonGetPressed(2))
    {
      targetFloor = 3;
      startTowardTarget();
    }

  /* 스텝 모터 논블로킹 */
  if (motor != MOTOR_STOP)
    {
      // tick이 0.5ms인데 step motor의 최소 틱 값이 1ms라서 해당 조건에서는 1ms로 바꿔서 작동
	  if (motor == MOTOR_CW)
	    {
	      stepMotorOneStep(DIR_CW);
	      if ((++stepCount % ELEVATOR_LED_DIV_STEPS) == 0)
		{
		  ledPosition = (uint8_t)((ledPosition + 7) & 0x7);   // Right 1칸
		  LED_WriteOneHot(ledPosition);
		}
	    }
	  else
	    {
	      stepMotorOneStep(DIR_CCW);
	      if ((++stepCount % ELEVATOR_LED_DIV_STEPS)==0)
		{
		  ledPosition = (uint8_t)((ledPosition + 1)&0x7);   // Left 1칸
		  LED_WriteOneHot(ledPosition);
		}
	    }

    }

  // 문 닫힘 지연 출발
  if (starterPending) {
      if (++starterMsAcc >= ELEVATOR_STARTER_DELAY_MS) {
	  starterPending = 0;
	  starterMsAcc   = 0;

	  // 지연 중에 층이 같아졌으면 취소
	  if (currentFloor == targetFloor) {
	      motor = MOTOR_STOP;
	      stopStepper();
	  } else {
	      motor = motorPending;  // 저장해 둔 방향으로 한 번만 출발
	  }
      }
  }


// 도착 시 문열림 딜레이 조건
  if (arrivalPending) {
      if (++arrivalMsAcc >= ELEVATOR_ARRIVAL_DELAY_MS) {
	  arrivalPending = 0;
	  openDelayDoor();  // 도착시 문 열리는 함수
      }
  }

  /* 도착 점멸 */
  if (blinkRemain)
    {
      if (blinkMsAcc >= ELEVATOR_BLINK_PERIOD_MS)
	{
	  blinkMsAcc = 0; blinkPhase ^= 1;
	  for(uint8_t i=0;i<8;i++)
	    {
	      if (blinkPhase) ledOn(i);
	      else ledOff(i);
	    }
	  if (--blinkRemain == 0) LED_WriteOneHot(-1);
	}
      else blinkMsAcc++;
    }

  if (buzzerOn){
      if (++buzzerMsAcc >= BUZZER_ON_MS){
	  Buzzer_Off();
	  buzzerOn = 0;
      }
  }

  // 1F: 목표가 1층일 때만 인정
  GPIO_PinState n1 = HAL_GPIO_ReadPin(EXIT_0F_GPIO_Port, EXIT_0F_Pin);
  if (n1==PHOTO_ACTIVE_ON && prevF1!=PHOTO_ACTIVE_ON && targetFloor==1 && motor==MOTOR_CCW)
      seenFloor(1);
  prevF1 = n1;

  // 2F: 목표가 2층일 때만 인정
  GPIO_PinState n2 = HAL_GPIO_ReadPin(EXIT_1F_GPIO_Port, EXIT_1F_Pin);
  if (n2==PHOTO_ACTIVE_ON && prevF2!=PHOTO_ACTIVE_ON && targetFloor==2) {
      if ((motor==MOTOR_CW && currentFloor<2) || (motor==MOTOR_CCW && currentFloor>2))
          seenFloor(2);
  }
  prevF2 = n2;

  // 3F: 목표가 3층일 때만 인정
  GPIO_PinState n3 = HAL_GPIO_ReadPin(EXIT_2F_GPIO_Port, EXIT_2F_Pin);
  if (n3==PHOTO_ACTIVE_ON && prevF3!=PHOTO_ACTIVE_ON && targetFloor==3 && motor==MOTOR_CW)
      seenFloor(3);
  prevF3 = n3;


}
