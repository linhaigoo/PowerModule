#include "gpio.h"
#include <stdio.h>
#include "stm32f4xx.h"

#define PIN_NUM CTRL_OUT_NUM

GPIO_TypeDef* OUT_PORT[PIN_NUM] = {PM_CTRL_PORT, D_ON_TEST_PORT,	D_ON_PORT,	MODE_SWITCHER_PORT};
uint16_t OUT_PIN[PIN_NUM] = {PM_CTRL_PIN, D_ON_TEST_PIN,	D_ON_PIN,	MODE_SWITCHER_PIN};
const uint32_t OUT_PORT_CLK[PIN_NUM] = {PM_CTRL_CLK, D_ON_TEST_CLK, D_ON_CLK, MODE_SWITCHER_CLK};
const GPIOOType_TypeDef OUT_PORT_OTYPE[PIN_NUM] =  {PM_CTRL_OTYPE, D_ON_TEST_OTYPE, D_ON_OTYPE, MODE_SWITCHER_OTYPE};

void CTRL_OUT_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;

	for(int i = 0; i < PIN_NUM; i++)
	{
		RCC_AHB1PeriphClockCmd(OUT_PORT_CLK[i] , ENABLE);
		GPIO_InitStructure.GPIO_Pin = OUT_PIN[i];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = OUT_PORT_OTYPE[i];
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(OUT_PORT[i], &GPIO_InitStructure);
	}
}
