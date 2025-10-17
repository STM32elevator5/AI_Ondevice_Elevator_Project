#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"

#define STEPS_PER_REVOLATION    4096
#define DIR_CW                    0   //시계방향
#define DIR_CCW                 1   //반시계 방향

#define IN1_PIN        GPIO_PIN_0
#define IN1_GPIO_PORT GPIOA
#define IN2_PIN        GPIO_PIN_1
#define IN2_GPIO_PORT GPIOA
#define IN3_PIN        GPIO_PIN_4
#define IN3_GPIO_PORT GPIOA
#define IN4_PIN        GPIO_PIN_0
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

void stepMotorOneStep(uint8_t dir);
void stopStepper(void);

#endif /* INC_STEPPER_H_ */
