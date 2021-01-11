#include "PowerModule.h"
#include "stdbool.h"
#include "gpio.h"

PM_Typedef PM_module;

#define STM32_UUID ((uint32_t *)0x1FFF7A10)

void PM_Init(PM_Typedef * pPM)
{
	pPM->ID = (STM32_UUID[0] & 0xff);
	switch(pPM->ID)
	{
		case 32:pPM->ID=1;break;
		case 84:pPM->ID=2;break;
		case 78:pPM->ID=3;break;
		case 33:pPM->ID=4;break;
		default:break;
	}
	
	
	pPM->I_LIM = ILIM_INIT;
	pPM->V_LIM = VLIM_INIT;
	pPM->ON_T_DUTY_LIM = 1.0f;
	pPM->vSet = 6.8f;
	pPM->iSet = 2.0f;
	pPM->iCur = 3.0f;
	pPM->vCur = 5.0f;
	pPM->on_t = DEFAULT_ON_T;
	pPM->all_t = DEFAULT_ALL_T;
	pPM->time = 0.0f;
	pPM->curMODE = VOL_MODE;
	
	pPM->isWorking = false;	
	pPM->is_calibrate_fail = true;	
	
	
	PM_module.ina_ini = 0.0f;
	PM_module.acs_ini = 0.0f;
	
	MY_DAC_Init();
	DacSetVol(3.29);
	
	MY_ADC_Init();
	
	CTRL_OUT_Init();
}
