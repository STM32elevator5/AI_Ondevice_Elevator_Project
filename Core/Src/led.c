#include "led.h"

LED_CONTROL led[8]=
    {
	{GPIOA, GPIO_PIN_5, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOA, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOA, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOB, GPIO_PIN_6, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOC, GPIO_PIN_7, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOA, GPIO_PIN_9, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOA, GPIO_PIN_8, GPIO_PIN_SET, GPIO_PIN_RESET},
	{GPIOB, GPIO_PIN_10, GPIO_PIN_SET, GPIO_PIN_RESET}
    };

void ledOn(uint8_t num)
{
  HAL_GPIO_WritePin(led[num].port, led[num].pinNumber, led[num].onState);
}

void ledOff(uint8_t num)
{
  HAL_GPIO_WritePin(led[num].port, led[num].pinNumber, led[num].offState);
}

void ledAllOff(void)
{
  for (uint8_t i = 0; i < 8; i++) ledOff(i);
}

void ledShiftOn(int16_t position)
{
  for (uint8_t i = 0; i < 8; i++) {
    if (i == (uint8_t)position) ledOn(i);
    else ledOff(i);
  }
}

void ledBlinkByPhase(uint8_t phase)
{
  for (uint8_t i = 0; i < 8; i++) {
    if (phase) ledOn(i);
    else ledOff(i);
  }
}
