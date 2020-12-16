#include "task_init.h"
#include "PowerModule.h"
#include "gpio.h"

#define ZERO_VOL_OUT (49*0.8)
#define R1_R3_RATIO 12

void TASK_REGULATE(void *pdata)
{
  CPU_SR cpu_sr=0;

	int count = 0;

	pdata = pdata; 
	
	CTRL_OUT_Init();
	
	PM_CTRL = 1;
	D_ON_TEST = 1;
	D_ON = 0;
	MODE_SWITCHER = 1;
	
	bool dir = true;
	while(1)
	{
		PM_CTRL = 0;
//		PM_module.vCur = GetAdc(VOUT_SENSE) * (3.3f * (50.0f / 3.0f) / 4096.f);
		PM_module.vCur = GetAdc(VOUT_SENSE) * (3.3f  / 4096.f);
//		PM_module.vinCur = GetAdc(VIN_SENSE) * (3.3f * (212.0f / 12.0f) / 4096.f);
		PM_module.vinCur = GetAdc(VIN_SENSE) * (3.3f / 4096.f);
//		PM_module.iCur = (GetAdc(INA) * (3.3f  / 4096.f ) -3.75) * 5.0f; //ina
//		PM_module.acs_i_cur = ((GetAdc(ACS)) * (3.3f / 4096.f) - 2.5)*10.0f;
		
		PM_module.iCur = (GetAdc(INA) * (3.3f  / 4096.f )); //ina
		PM_module.acs_i_cur = ((GetAdc(ACS)) * (3.3f / 4096.f));
		
		PM_module.dac_out = (ZERO_VOL_OUT - PM_module.vSet) / R1_R3_RATIO;
		DacSetVol(PM_module.dac_out);
		delay_ms(1000);
	}
}

