#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "main.h"

#define BUZZER_GPIO_Port GPIOA
#define BUZZER_Pin       GPIO_PIN_15
#define BUZZER_ON_MS     500u             // 0.5 s

typedef enum {
  BUZZER_OFF_STATE = 0,
  BUZZER_ON_STATE = 1
} BuzzerState;

void Buzzer_On(void);
void Buzzer_Off(void);

#endif /* INC_BUZZER_H_ */
