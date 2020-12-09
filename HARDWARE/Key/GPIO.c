#include "GPIO.h"
void GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//IO0 :PF14 IO1 :PF15
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); 
	
	#ifdef USE_IO0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO1
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
	//IO2-9 :PG0-7
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	
	#ifdef USE_IO2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO3	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO4	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	

	
	//IO15-18 :PF4-7
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	#ifdef USE_IO15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
		#ifdef USE_IO16
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
		#ifdef USE_IO17
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
		#ifdef USE_IO18
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	

	
	//IO19 PF10
	#ifdef USE_IO19
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	#endif
	
	//IO11-14 PG10-13
	#ifdef USE_IO11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
		#ifdef USE_IO12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
		#ifdef USE_IO13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	#ifdef USE_IO14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	#endif
	
	
}