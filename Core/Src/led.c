#include "led.h"

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

void ledOn(uint8_t num)
{
  HAL_GPIO_WritePin(led[num].port, led[num].pinNumber, led[num].onState);
}
void ledOff(uint8_t num)
{
  HAL_GPIO_WritePin(led[num].port, led[num].pinNumber, led[num].offState);
}

void ledLeftShift(uint8_t num)
{
  for(uint8_t i = 0; i < num; i++)
    {
      ledOn(i);
      HAL_Delay(100);
    }
  HAL_Delay(500);
  for(uint8_t i = 0; i < num; i++)
    {
      ledOff(i);
      HAL_Delay(100);
    }
}
void ledRightShift(uint8_t num)
{
  uint8_t end = (num == 0) ? 0 : num;     // 0이면 0까지, 아니면 num까지
  for (int8_t i = 7; i >= (int8_t)end; i--) {
      ledOn((uint8_t)i);
      HAL_Delay(100);
  }
  HAL_Delay(500);
  for (int8_t i = 7; i >= (int8_t)end; i--) {
      ledOff((uint8_t)i);
      HAL_Delay(100);
  }
}

//void ledOn(uint8_t num)
//{
//  for(uint8_t i = 0; i < num; i++)
//    {
//      HAL_GPIO_WritePin(led[i].port, led[i].pinNumber, led[i].onState);
//    }
//}
//
//void ledOff(uint8_t num)
//{
//  for(uint8_t i = 0; i < num; i++)
//    {
//      HAL_GPIO_WritePin(led[i].port, led[i].pinNumber, led[i].offState);
//    }
//}
//
//void ledLeftShift()
//{
//  for(uint8_t i = 0; i < 8; i++)
//    {
//      ledOn(i);
//      HAL_Delay(100);
//    }
//  HAL_Delay(500);
//  for(uint8_t i = 0; i < 8; i++)
//    {
//      ledOff(i);
//      HAL_Delay(100);
//    }
//}
//
//void ledRightShift()
//{
//  for (int i = 8; i > 0; i--)
//    {
//      ledOn(8 - i);
//      HAL_Delay(100);
//    }
//
//  HAL_Delay(500);
//
//  for (int i = 8; i > 0; i--)
//    {
//      ledOff(8 - i);
//      HAL_Delay(100);
//    }
//}
