#ifndef INC_FND_H_
#define INC_FND_H_

#include "main.h"

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t pinNumber;
  GPIO_PinState onState;
  GPIO_PinState offState;
}FND_CONTROL;

void FND_AllOff(void);
void FND_WriteMask(uint8_t mask);     // bit0=a ... bit6=g
void FND_ShowDigit(uint8_t digit);    // 1,2,3만 표시


#endif /* INC_FND_H_ */
