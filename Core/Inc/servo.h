#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include "stm32f4xx_hal.h"

#define OPEN_SERVO_DOOR		150
#define CLOSE_SERVO_DOOR	50

void openDelayDoor(void);
void closeDelayDoor(void);

#endif /* INC_SERVO_H_ */
