#include "fnd.h"

FND_CONTROL fndSeg[7]=
    {
	{GPIOA, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET}, // a
	{GPIOA, GPIO_PIN_11, GPIO_PIN_SET, GPIO_PIN_RESET}, // b
	{GPIOB, GPIO_PIN_12, GPIO_PIN_SET, GPIO_PIN_RESET}, // c
	{GPIOB, GPIO_PIN_2, GPIO_PIN_SET, GPIO_PIN_RESET}, // d
	{GPIOB, GPIO_PIN_1, GPIO_PIN_SET, GPIO_PIN_RESET}, // e
	{GPIOB, GPIO_PIN_15, GPIO_PIN_SET, GPIO_PIN_RESET}, // f
	{GPIOB, GPIO_PIN_14, GPIO_PIN_SET, GPIO_PIN_RESET}, // g
    };

uint8_t fndData[3] = {
    0x06, // 1
    0x5b, // 2
    0x4f, // 3
};

static void seg_write(int idx, uint8_t on){
  HAL_GPIO_WritePin(fndSeg[idx].port, fndSeg[idx].pinNumber,
		    on ? fndSeg[idx].onState : fndSeg[idx].offState);
}

void FND_AllOff(void){
  for(int i = 0 ; i < 7 ; i++) seg_write(i, 0);
}

void FND_WriteMask(uint8_t mask){
  for(int i = 0 ; i < 7 ; i++) seg_write(i, (mask >> i) & 0x1);
}

void FND_ShowDigit(uint8_t digit){
  switch(digit){
    case 1: FND_WriteMask(fndData[0]); break;
    case 2: FND_WriteMask(fndData[1]); break;
    case 3: FND_WriteMask(fndData[2]); break;
    default: FND_AllOff(); break;
  }
}
