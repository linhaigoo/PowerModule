#include "encoder.h"
#include <stdio.h>
#include "stm32f4xx.h"

GPIO_TypeDef* ENC_KEY_CH_PORT[ENC_KEYn<<1] = {ENC1_N_PORT, ENC1_P_PORT, ENC2_N_PORT, ENC2_P_PORT};
const uint16_t ENC_KEY_CH_PIN[ENC_KEYn<<1] = {ENC1_N_PIN, ENC1_P_PIN, ENC2_N_PIN, ENC2_P_PIN};
const uint8_t ENC_KEY_CH_PIN_SOURCE[ENC_KEYn<<1] = {ENC1_N_SOURCE, ENC1_P_SOURCE, ENC2_N_SOURCE, ENC2_P_SOURCE};
const uint8_t ENC_KEY_CH_PIN_AF[ENC_KEYn<<1] = {ENC1_N_AF, ENC1_P_AF, ENC2_N_AF, ENC2_P_AF};
const uint32_t ENC_KEY_CH_PORT_CLK[ENC_KEYn<<1] = {ENC1_N_CLK, ENC1_P_CLK, ENC2_N_CLK,ENC2_P_CLK};

TIM_TypeDef * ENC_KEY_CH_TIM[ENC_KEYn] = {ENC1_TIMER, ENC2_TIMER};
const uint32_t ENC_KEY_CH_TIM_CLK[ENC_KEYn] = {ENC1_TIMER_CLK, ENC2_TIMER_CLK};

GPIO_TypeDef* ENC_KEY_KEY_PORT[ENC_KEYn] = {ENC1_KEY_PORT, ENC2_KEY_PORT};
uint16_t ENC_KEY_KEY_PIN[ENC_KEYn] = {ENC1_KEY_PIN, ENC2_KEY_PIN};
const uint32_t ENC_KEY_KEY_PORT_CLK[ENC_KEYn] = {ENC1_KEY_CLK, ENC2_KEY_CLK};


const uint8_t ENC_KEY_EXTI_PORT_SRC[ENC_KEYn] = {ENC1_KEY_EXTI_PORT_SRC, ENC2_KEY_EXTI_PORT_SRC};
const uint8_t ENC_KEY_EXTI_PIN_SRC[ENC_KEYn] = {ENC1_KEY_EXTI_PIN_SRC, ENC2_KEY_EXTI_PIN_SRC};
const uint32_t ENC_KEY_EXTI_LINE[ENC_KEYn] = {ENC1_KEY_EXTI_LINE, ENC2_KEY_EXTI_LINE};
IRQn_Type ENC_KEY_EXTI_IRQN[ENC_KEYn] = {ENC1_KEY_EXTI_IRQN, ENC2_KEY_EXTI_IRQN};

EncoderTypeDef ENC_KEY[ENC_KEYn];

void ENC_Reset(ENC_KEY_Typedef ENC_KEY_Id)
{
	TIM_SetCounter(ENC_KEY_CH_TIM[ENC_KEY_Id], 0);

	ENC_KEY[ENC_KEY_Id].encoder = 0;
	ENC_KEY[ENC_KEY_Id].old_encoder = 0;
	ENC_KEY[ENC_KEY_Id].count = 0;
	ENC_KEY[ENC_KEY_Id].distance = 0;
	ENC_KEY[ENC_KEY_Id].key_pressed = 0;
}

s32 ENC_Read(ENC_KEY_Typedef ENC_KEY_Id)
{
	ENC_KEY[ENC_KEY_Id].old_encoder = ENC_KEY[ENC_KEY_Id].encoder;
	ENC_KEY[ENC_KEY_Id].encoder = TIM_GetCounter(ENC_KEY_CH_TIM[ENC_KEY_Id]);
	ENC_KEY[ENC_KEY_Id].count = ENC_KEY[ENC_KEY_Id].encoder - ENC_KEY[ENC_KEY_Id].old_encoder;
	ENC_KEY[ENC_KEY_Id].distance += ENC_KEY[ENC_KEY_Id].count;
	return ENC_KEY[ENC_KEY_Id].count;
}

u8 KEY_Read(ENC_KEY_Typedef ENC_KEY_Id)
{
	u8 val = 0;
	if(ENC_KEY[ENC_KEY_Id].key_pressed && ((uint32_t)KEY_TIME_GET() - ENC_KEY[ENC_KEY_Id].key_pressed_timestamp > KEY_JITTER_TIME_COUNT))
	{
		val = GPIO_ReadInputDataBit(ENC_KEY_KEY_PORT[ENC_KEY_Id], ENC_KEY_KEY_PIN[ENC_KEY_Id]) == RESET;
		ENC_KEY[ENC_KEY_Id].key_pressed = 0;
	}
	return val;
}

void ENC_KEY_Init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	int i;
	for(i = 0; i < ENC_KEYn; i++)
	{
		RCC_AHB1PeriphClockCmd(ENC_KEY_KEY_PORT_CLK[i] , ENABLE);
		GPIO_InitStructure.GPIO_Pin = ENC_KEY_KEY_PIN[i];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(ENC_KEY_KEY_PORT[i], &GPIO_InitStructure);
	}
	
	for(i = 0; i < ENC_KEYn<<1; i++)
	{
		RCC_AHB1PeriphClockCmd(ENC_KEY_CH_PORT_CLK[i] , ENABLE);
		GPIO_InitStructure.GPIO_Pin = ENC_KEY_CH_PIN[i];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(ENC_KEY_CH_PORT[i], &GPIO_InitStructure);
		GPIO_PinAFConfig(ENC_KEY_CH_PORT[i], ENC_KEY_CH_PIN_SOURCE[i], ENC_KEY_CH_PIN_AF[i]);
	}

	for(i = 0; i < ENC_KEYn; i++)
	{
		RCC_APB1PeriphClockCmd(ENC_KEY_CH_TIM_CLK[i], ENABLE);
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
		TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;  
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		
		TIM_TimeBaseInit(ENC_KEY_CH_TIM[i], &TIM_TimeBaseStructure);
		
		// set them up as encoder inputs
		// set both inputs to rising polarity to let it use both edges
		TIM_EncoderInterfaceConfig (ENC_KEY_CH_TIM[i], TIM_EncoderMode_TI12,
									TIM_ICPolarity_Rising,
									TIM_ICPolarity_Rising);
		TIM_SetAutoreload (ENC_KEY_CH_TIM[i], 0xffff);
		
		// turn on the timer/counters
		TIM_Cmd(ENC_KEY_CH_TIM[i], ENABLE);
		
		ENC_Reset(i);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	

	for(i = 0; i < ENC_KEYn; i++)
	{
		SYSCFG_EXTILineConfig(ENC_KEY_EXTI_PORT_SRC[i], ENC_KEY_EXTI_PIN_SRC[i]);
		EXTI_InitStructure.EXTI_Line = ENC_KEY_EXTI_LINE[i];
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = ENC_KEY_EXTI_IRQN[i];
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

//void EXTI9_5_IRQHandler(void) 
//{
//	if(EXTI_GetITStatus(ENC1_KEY_EXTI_LINE) != RESET) 
//	{
//		 ENC_KEY[ENC_KEY1].key_pressed = !ENC1_KEY_VAL;
//			
//			

//		EXTI_ClearITPendingBit(ENC1_KEY_EXTI_LINE);
//	}
//	if(EXTI_GetITStatus(ENC2_KEY_EXTI_LINE) != RESET) 
//	{
//		 ENC_KEY[ENC_KEY2].key_pressed = !ENC2_KEY_VAL;
//			
//			

//		EXTI_ClearITPendingBit(ENC1_KEY_EXTI_LINE);
//	}
//}

void ENC1_KEY_EXTI_IRQ_HANLDER(void)
{
	if(EXTI_GetITStatus(ENC1_KEY_EXTI_LINE) != RESET) 
	{
		 ENC_KEY[ENC_KEY1].key_pressed = !ENC1_KEY_VAL;

		EXTI_ClearITPendingBit(ENC1_KEY_EXTI_LINE);
	}
}

void ENC2_KEY_EXTI_IRQ_HANLDER(void)
{
	if(EXTI_GetITStatus(ENC2_KEY_EXTI_LINE) != RESET) 
	{
		 ENC_KEY[ENC_KEY2].key_pressed = !ENC2_KEY_VAL;
		 ENC_KEY[ENC_KEY2].key_pressed_timestamp = KEY_TIME_GET();

		EXTI_ClearITPendingBit(ENC2_KEY_EXTI_LINE);
	}
}
