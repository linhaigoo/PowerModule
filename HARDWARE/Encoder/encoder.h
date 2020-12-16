#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx.h"
#include "sys.h"

#include <os.h>
#define KEY_TIME_GET OS_TS_GET
#define KEY_JITTER_TIME 10               //uint : ms
#define KEY_JITTER_TIME_COUNT            (SystemCoreClock / 1000)
#define ENCODER_TIM_PERIOD  65535



//ENC1
#define ENC1_TIMER			TIM3
#define ENC1_TIMER_CLK	RCC_APB1Periph_TIM3
//PB4
#define ENC1_N_PORT			GPIOB
#define ENC1_N_PIN			GPIO_Pin_4
#define ENC1_N_CLK			RCC_AHB1Periph_GPIOB
#define ENC1_N_SOURCE		GPIO_PinSource4
#define ENC1_N_AF				GPIO_AF_TIM3
//PB5
#define ENC1_P_PORT			GPIOB
#define ENC1_P_PIN			GPIO_Pin_5
#define ENC1_P_CLK			RCC_AHB1Periph_GPIOB
#define ENC1_P_SOURCE		GPIO_PinSource5
#define ENC1_P_AF				GPIO_AF_TIM3
//PD2
#define ENC1_KEY_PORT			GPIOD
#define ENC1_KEY_PIN			GPIO_Pin_2
#define ENC1_KEY_CLK  		RCC_AHB1Periph_GPIOD
#define ENC1_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOD
#define ENC1_KEY_EXTI_PIN_SRC   EXTI_PinSource2
#define ENC1_KEY_EXTI_LINE      EXTI_Line2
#define ENC1_KEY_EXTI_IRQN      EXTI2_IRQn
#define ENC1_KEY_EXTI_IRQ_HANLDER      EXTI2_IRQHandler
#define ENC1_KEY_VAL            PDin(2)


//ENC2
#define ENC2_TIMER			TIM4
#define ENC2_TIMER_CLK		RCC_APB1Periph_TIM4
//PB6
#define ENC2_N_PORT			GPIOB
#define ENC2_N_PIN			GPIO_Pin_6
#define ENC2_N_CLK			RCC_AHB1Periph_GPIOB
#define ENC2_N_SOURCE		GPIO_PinSource6
#define ENC2_N_AF				GPIO_AF_TIM4
//PB7
#define ENC2_P_PORT			GPIOB
#define ENC2_P_PIN			GPIO_Pin_7
#define ENC2_P_CLK			RCC_AHB1Periph_GPIOB
#define ENC2_P_SOURCE		GPIO_PinSource7
#define ENC2_P_AF				GPIO_AF_TIM4
//PB3
#define ENC2_KEY_PORT			GPIOB
#define ENC2_KEY_PIN			GPIO_Pin_3
#define ENC2_KEY_CLK  		RCC_AHB1Periph_GPIOB
#define ENC2_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOB
#define ENC2_KEY_EXTI_PIN_SRC   EXTI_PinSource3
#define ENC2_KEY_EXTI_LINE      EXTI_Line3
#define ENC2_KEY_EXTI_IRQN      EXTI3_IRQn
#define ENC2_KEY_EXTI_IRQ_HANLDER      EXTI3_IRQHandler
#define ENC2_KEY_VAL            PBin(3)


////ENC1
//#define ENC1_TIMER			TIM3
//#define ENC1_TIMER_CLK		RCC_APB1Periph_TIM3

//#define ENC1_N_PORT			GPIOC
//#define ENC1_N_PIN			GPIO_Pin_6
//#define ENC1_N_CLK			RCC_AHB1Periph_GPIOC
//#define ENC1_N_SOURCE		GPIO_PinSource6
//#define ENC1_N_AF			GPIO_AF_TIM3

//#define ENC1_P_PORT			GPIOC
//#define ENC1_P_PIN			GPIO_Pin_7
//#define ENC1_P_CLK			RCC_AHB1Periph_GPIOC
//#define ENC1_P_SOURCE		GPIO_PinSource7
//#define ENC1_P_AF			GPIO_AF_TIM3

//#define ENC1_KEY_PORT			GPIOB
//#define ENC1_KEY_PIN			GPIO_Pin_8
//#define ENC1_KEY_CLK  		RCC_AHB1Periph_GPIOB
//#define ENC1_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOB
//#define ENC1_KEY_EXTI_PIN_SRC   EXTI_PinSource8
//#define ENC1_KEY_EXTI_LINE      EXTI_Line8
//#define ENC1_KEY_EXTI_IRQN      EXTI9_5_IRQn
//#define ENC1_KEY_VAL            PBin(8)  


////ENC2
//#define ENC2_TIMER			TIM5
//#define ENC2_TIMER_CLK		RCC_APB1Periph_TIM5

//#define ENC2_N_PORT			GPIOA
//#define ENC2_N_PIN			GPIO_Pin_0
//#define ENC2_N_CLK			RCC_AHB1Periph_GPIOA
//#define ENC2_N_SOURCE		GPIO_PinSource0
//#define ENC2_N_AF			GPIO_AF_TIM5

//#define ENC2_P_PORT			GPIOA
//#define ENC2_P_PIN			GPIO_Pin_1
//#define ENC2_P_CLK			RCC_AHB1Periph_GPIOA
//#define ENC2_P_SOURCE		GPIO_PinSource1
//#define ENC2_P_AF			GPIO_AF_TIM5

//#define ENC2_KEY_PORT			GPIOB
//#define ENC2_KEY_PIN			GPIO_Pin_9
//#define ENC2_KEY_CLK  		RCC_AHB1Periph_GPIOB
//#define ENC2_KEY_EXTI_PORT_SRC  EXTI_PortSourceGPIOB
//#define ENC2_KEY_EXTI_PIN_SRC   EXTI_PinSource9
//#define ENC2_KEY_EXTI_LINE      EXTI_Line9
//#define ENC2_KEY_EXTI_IRQN      EXTI9_5_IRQn
//#define ENC2_KEY_VAL            PBin(9)                

typedef struct
{
	uint16_t encoder;
	uint16_t old_encoder;
	int16_t count;
	int32_t distance;
	uint32_t key_pressed_timestamp;
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
