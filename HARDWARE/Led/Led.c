#include "Led.h"

void LED_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	#ifdef LED1
	 RCC_AHB1PeriphClockCmd(LED1_PORT_CLK, ENABLE); 
	 GPIO_InitStructure.GPIO_Pin = LED1_PIN; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//����
	 GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	 GPIO_WriteBit(LED1_PORT,LED1_PIN,Bit_RESET);
	#endif
};





