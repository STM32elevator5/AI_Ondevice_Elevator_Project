#include "buzzer.h"

void Buzzer_On(void)
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_ON_STATE);
}

void Buzzer_Off(void)
{
  HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, BUZZER_OFF_STATE);
}
