# :kissing_cat:AI_Ondevice_Elevator_Project
AI 온디바이스 5조 STM32와 peripheral을 활용한 엘레베이터 시스템 만들기

## :star2::star2:프로젝트 결과:star2::star2:
:point_right:(유튜브 영상)[https://www.youtube.com/watch?v=ElRo6nXT7hM]

## 프로젝트 디렉토리 구조

```
Core/
├─ Inc/
│  ├─ button.h
│  ├─ buzzer.h
│  ├─ elevator.h
│  ├─ fnd.h
│  ├─ led.h
│  ├─ main.h
│  ├─ servo.h
│  └─ stepper.h
└─ Src/
   ├─ button.c
   ├─ buzzer.c
   ├─ elevator.c
   ├─ fnd.c
   ├─ led.c
   ├─ main.c
   ├─ servo.c
   └─ stepper.c
```

## LED 구조
```
typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pinNumber;
  GPIO_PinState onState;
  GPIO_PinState offState;
}LED_CONTROL;

test
  
LED_CONTROL led[8]=
    {
        {GPIOA, GPIO_PIN_5, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOB, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOC, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_9, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOA, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET},
        {GPIOB, GPIO_PIN_10, GPIO_PIN_SET, GPIO_PIN_RESET}
    };
```

## BUTTON 구조
```
typedef struct
{
  GPIO_TypeDef *port;
  uint16_t     pinNumber;
  GPIO_PinState onState;
}BUTTON_CONTROL;

BUTTON_CONTROL button[3]=
    {
        {GPIOB, GPIO_PIN_4, 0},
        {GPIOB, GPIO_PIN_5, 0},
        {GPIOB, GPIO_PIN_3, 0},
    };
```

## STEPPER
```
#define STEPS_PER_REVOLATION    4096
#define DIR_CW                    0   //시계방향
#define DIR_CCW                 1   //반시계 방향

#define IN1_PIN        GPIO_PIN_1
#define IN1_GPIO_PORT GPIOB
#define IN2_PIN        GPIO_PIN_15
#define IN2_GPIO_PORT GPIOB
#define IN3_PIN        GPIO_PIN_14
#define IN3_GPIO_PORT GPIOB
#define IN4_PIN        GPIO_PIN_13
#define IN4_GPIO_PORT GPIOB

static const uint8_t HALF_STEP_SEQ[8][4]=
{
        {1,0,0,0},
        {1,1,0,0},
        {0,1,0,0},
        {0,1,1,0},
        {0,0,1,0},
        {0,0,1,1},
        {0,0,0,1},
        {1,0,0,1},
};
```

## FND
```
typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pinNumber;
  GPIO_PinState onState;   // Common-Cathode: GPIO_PIN_SET, Common-Anode: GPIO_PIN_RESET
  GPIO_PinState offState;  // Common-Cathode: GPIO_PIN_RESET, Common-Anode: GPIO_PIN_SET
} FND_CONTROL;

// 세그먼트 인덱스(A~G, DP)asdf
enum { SEG_A=0, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP, SEG_COUNT=8 };

extern FND_CONTROL fnd[SEG_COUNT];

FND_CONTROL fnd[SEG_COUNT] =
{
  {GPIOA, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET}, // A
  {GPIOA, GPIO_PIN_11, GPIO_PIN_SET, GPIO_PIN_RESET}, // B
  {GPIOB, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET}, // C
  {GPIOB, GPIO_PIN_2,  GPIO_PIN_SET, GPIO_PIN_RESET}, // D
  {GPIOB, GPIO_PIN_1,  GPIO_PIN_SET, GPIO_PIN_RESET}, // E
  {GPIOB, GPIO_PIN_15, GPIO_PIN_SET, GPIO_PIN_RESET}, // F
  {GPIOB, GPIO_PIN_14, GPIO_PIN_SET, GPIO_PIN_RESET}, // G
  {GPIOB, GPIO_PIN_13, GPIO_PIN_SET, GPIO_PIN_RESET}, // DP
};
```
