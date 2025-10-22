#include "servo.h"

void openDelayDoor()
{
  TIM11->CCR1 = OPEN_SERVO_DOOR;                  // 서보 후행 동작
}

void closeDelayDoor()
{
  TIM11->CCR1 = CLOSE_SERVO_DOOR;                 // 서보 후행 동작
}
