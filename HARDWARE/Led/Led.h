#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED1 
//PORT_CLK
#define LED1_PORT_CLK RCC_AHB1Periph_GPIOE
//IO  PG15
#define LED1_PORT GPIOE
#define LED1_PIN  GPIO_Pin_6

//END

void LED_Init(void);
#define LED1_SET PEout(6) 
#endif

