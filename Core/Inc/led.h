#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f4xx_hal.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pinNumber;
  GPIO_PinState onState;
  GPIO_PinState offState;
}LED_CONTROL;

void ledOn(uint8_t num);
void ledOff(uint8_t num);
void ledAllOff(void);
void ledShiftOn(int16_t position);
void ledBlinkByPhase(uint8_t phase);

#endif /* INC_LED_H_ */
