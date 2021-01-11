#ifndef __DAC_H__
#define __DAC_H__ 
#include "sys.h"
#include "stdbool.h"

//PA4 V_ADJ
#define DAC_PORT		  GPIOA
#define DAC_PIN			GPIO_Pin_2
#define DAC_CLK  		RCC_AHB1Periph_GPIOA
#define DAC_CHANNEL    DAC_Channel_1
#define DAC_SET_CHANNEL DAC_SetChannel1Data

void MY_DAC_Init(void);
bool DacSetVol(float vol);

#endif

