#include "System_Init.h"
void System_Init()
{
	LED_Init();
#ifdef USE_DialSwitch
	DialSW_Init();
#endif
	
#ifdef USE_TFT_lcd
	NT5510_Lcd_Init();
#endif
	
#ifdef USE_jlx_lcd
	jlx_lcd_Init();
#endif
	
#ifdef USE_OV7670
	OV7670_Init();
#endif
	
#ifdef USE_OV2640
	OV2640_Init();
#endif

#ifdef USE_MOTOR
	Motor_T4_Init();
#endif

#ifdef USE_ENCODER
	Encoder1_Init();
	Encoder2_Init();
#endif

#ifdef USE_GPIO
	GPIO_Config();
#endif

#ifdef USE_RemoteControl
	Remote_Control_Init();
#endif

#ifdef USE_HC_SR
		HC_SR_Init();
#endif

}