#include "PowerModule.h"
#include "stdbool.h"


PM_Typedef PM_module;

void PM_Init(PM_Typedef * pPM)
{
	pPM->I_LIM = ILIM_INIT;
	pPM->V_LIM = VLIM_INIT;
	pPM->ON_T_DUTY_LIM = 1.0f;
	pPM->vSet = 6.8f;
	pPM->iSet = 2.0f;
	pPM->iCur = 3.0f;
	pPM->vCur = 5.0f;
	pPM->on_t = 2.0f;
	pPM->all_t = 2.5f;
	pPM->time = 0.0f;
	pPM->curMODE = VOL_MODE;
	pPM->ID = PM_ID;
	pPM->isWorking = false;	
	
	MY_DAC_Init();
	DacSetVol(3.29);
	
	MY_ADC_Init();
}
