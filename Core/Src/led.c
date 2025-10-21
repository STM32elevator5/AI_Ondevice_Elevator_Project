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
