#include "stepper.h"

static uint8_t stepIndex = 0;

void stepMotorOneStep(uint8_t dir)
{
    if(dir == DIR_CW)
    {
        stepIndex++;
        if(stepIndex > 7) stepIndex = 0;
    }
    else
    {
        if(stepIndex == 0) stepIndex = 7;
        else stepIndex--;
    }

    HAL_GPIO_WritePin(IN1_GPIO_PORT, IN1_PIN, HALF_STEP_SEQ[stepIndex][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_GPIO_PORT, IN2_PIN, HALF_STEP_SEQ[stepIndex][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_GPIO_PORT, IN3_PIN, HALF_STEP_SEQ[stepIndex][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_GPIO_PORT, IN4_PIN, HALF_STEP_SEQ[stepIndex][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void stopStepper(void)
{
    HAL_GPIO_WritePin(IN1_GPIO_PORT, IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2_GPIO_PORT, IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN3_GPIO_PORT, IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN4_GPIO_PORT, IN4_PIN, GPIO_PIN_RESET);
}
