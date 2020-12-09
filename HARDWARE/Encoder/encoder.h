#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx.h"
#include "sys.h"

#define ENCODER_TIM_PERIOD  65535

//ENC1
#define ENC1_TIMER			TIM3
#define ENC1_TIMER_CLK		RCC_APB1Periph_TIM3

#define ENC1_N_PORT			GPIOC
#define ENC1_N_PIN			GPIO_Pin_6
#define ENC1_N_CLK			RCC_AHB1Periph_GPIOC
#define ENC1_N_SOURCE		GPIO_PinSource6
#define ENC1_N_AF			GPIO_AF_TIM3

#define ENC1_P_PORT			GPIOC
#define ENC1_P_PIN			GPIO_Pin_7
#define ENC1_P_CLK			RCC_AHB1Periph_GPIOC
#define ENC1_P_SOURCE		GPIO_PinSource7
#define ENC1_P_AF			GPIO_AF_TIM3

#define ENC1_KEY_PORT			GPIOB
#define ENC1_KEY_PIN			GPIO_Pin_8
#define ENC1_KEY_CLK  		RCC_AHB1Periph_GPIOB
#define ENC1_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOB
#define ENC1_KEY_EXTI_PIN_SRC   EXTI_PinSource8
#define ENC1_KEY_EXTI_LINE      EXTI_Line8
#define ENC1_KEY_EXTI_IRQN      EXTI15_10_IRQn
#define ENC1_KEY_VAL            PBin(8)  


//ENC2
#define ENC2_TIMER			TIM5
#define ENC2_TIMER_CLK		RCC_APB1Periph_TIM5

#define ENC2_N_PORT			GPIOA
#define ENC2_N_PIN			GPIO_Pin_0
#define ENC2_N_CLK			RCC_AHB1Periph_GPIOA
#define ENC2_N_SOURCE		GPIO_PinSource0
#define ENC2_N_AF			GPIO_AF_TIM5

#define ENC2_P_PORT			GPIOA
#define ENC2_P_PIN			GPIO_Pin_1
#define ENC2_P_CLK			RCC_AHB1Periph_GPIOA
#define ENC2_P_SOURCE		GPIO_PinSource1
#define ENC2_P_AF			GPIO_AF_TIM5

#define ENC2_KEY_PORT			GPIOB
#define ENC2_KEY_PIN			GPIO_Pin_9
#define ENC2_KEY_CLK  		RCC_AHB1Periph_GPIOB
#define ENC2_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOB
#define ENC2_KEY_EXTI_PIN_SRC   EXTI_PinSource9
#define ENC2_KEY_EXTI_LINE      EXTI_Line9
#define ENC2_KEY_EXTI_IRQN      EXTI15_10_IRQn
#define ENC2_KEY_VAL            PBin(9)                

typedef struct
{
	uint16_t encoder;
	uint16_t old_encoder;
	int16_t count;
	int32_t distance;
	uint8_t key_pressed;
} EncoderTypeDef;

typedef enum
{
	ENC_KEY1 = 0,
	ENC_KEY2 = 1,
	ENC_KEYn = 2,
}ENC_KEY_Typedef;


void ENC_KEY_Init(void);
void ENC_Reset(ENC_KEY_Typedef ENC_KEY_Id);
s32 ENC_Read(ENC_KEY_Typedef ENC_KEY_Id);
u8 KEY_Read(ENC_KEY_Typedef ENC_KEY_Id);

#endif
