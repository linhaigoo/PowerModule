#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx.h"
#include "sys.h"

#define CTRL_OUT_NUM 5

//PA2 PM_CTRL
#define PM_CTRL_PORT		GPIOA
#define PM_CTRL_PIN			GPIO_Pin_2
#define PM_CTRL_CLK  		RCC_AHB1Periph_GPIOA
#define PM_CTRL_OTYPE   GPIO_OType_OD
#define PM_CTRL         PAout(2)

//PB9 D_ON_TEST
#define D_ON_TEST_PORT		GPIOB
#define D_ON_TEST_PIN			GPIO_Pin_9
#define D_ON_TEST_CLK  		RCC_AHB1Periph_GPIOB
#define D_ON_TEST_OTYPE   GPIO_OType_PP
#define D_ON_TEST         PBout(9)

//PB8  D_ON
#define D_ON_PORT					GPIOB
#define D_ON_PIN					GPIO_Pin_8
#define D_ON_CLK  				RCC_AHB1Periph_GPIOB
#define D_ON_OTYPE   			GPIO_OType_PP
#define D_ON         			PBout(8)

//PA3   MODE_SWITCHER
#define MODE_SWITCHER_PORT					GPIOA
#define MODE_SWITCHER_PIN						GPIO_Pin_3
#define MODE_SWITCHER_CLK  					RCC_AHB1Periph_GPIOA
#define MODE_SWITCHER_OTYPE   			GPIO_OType_PP
#define MODE_SWITCHER         			PAout(3)

//PA11 RS_CTRL
#define RS_CTRL_PORT		GPIOA
#define RS_CTRL_PIN			GPIO_Pin_11
#define RS_CTRL_CLK  		RCC_AHB1Periph_GPIOA
#define RS_CTRL_OTYPE   GPIO_OType_PP
#define RS_CTRL         PAout(11)


void CTRL_OUT_Init(void);
#endif
