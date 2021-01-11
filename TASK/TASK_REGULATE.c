#include "task_init.h"
#include "PowerModule.h"
#include "gpio.h"

float ZERO_VOL_OUT = 40.0f;
float R1_R3_RATIO = 12.0f;


void measurePM(void)
{

		PM_module.vinCur = GetAdc(VIN_SENSE) * (3.3f * (212.0f / 12.0f) / 4096.f);
//		PM_module.vinCur = GetAdc(VIN_SENSE) * (3.3f / 4096.f);
		PM_module.vCur = GetAdc(VOUT_SENSE) * (3.3f * (50.0f / 3.0f) / 4096.f);
//		PM_module.vCur = GetAdc(VOUT_SENSE) * (3.3f  / 4096.f);
		PM_module.ina_i_cur = (GetAdc(INA) * (3.3f  / 4096.f ) -3.75) * 5.0f - PM_module.ina_ini; //ina
		PM_module.acs_i_cur = ((GetAdc(ACS)) * (3.3f / 4096.f) - 2.5)*10.0f * 1.08f - PM_module.acs_ini;//acs
	
	if(PM_module.acs_i_cur > 6.5f)
	{
		PM_module.isWorking = AMP_EXCESS;
		PM_module.on = false;
	}
}

bool calibratePM()
{
	float v[2];
	float v_adj[2]={2.5, 1.5};
	PM_module.ina_ini = 0;
	PM_module.acs_ini = 0;
	D_ON = 0;
	DacSetVol(v_adj[0]);
	delay_ms(1000);
	measurePM();
	v[0] = PM_module.vCur;
	PM_module.ina_ini += PM_module.ina_i_cur;
	PM_module.acs_ini += PM_module.acs_i_cur;
	DacSetVol(v_adj[1]);
	delay_ms(1000);
	measurePM();
	v[1] = PM_module.vCur;
	PM_module.ina_ini += PM_module.ina_i_cur;
	PM_module.acs_ini += PM_module.acs_i_cur;
	
	float R1_R3_RATIO_temp = (v[0] - v[1]) / (v_adj[1] - v_adj[0]);
	if(fabs((R1_R3_RATIO_temp - R1_R3_RATIO)/R1_R3_RATIO)< 0.3f) return false;
	R1_R3_RATIO = R1_R3_RATIO_temp;
	float ZERO_VOL_OUT_temp = v[1] + R1_R3_RATIO * v_adj[1];
	if(fabs((ZERO_VOL_OUT_temp - ZERO_VOL_OUT)/ZERO_VOL_OUT)< 0.3f) return false;
	
	
	ZERO_VOL_OUT = ZERO_VOL_OUT_temp;
	PM_module.ina_ini /= 2.0f;
	PM_module.acs_ini /= 2.0f;
	PM_module.is_calibrate_fail = false;
	return true;
}

extern bool trig_request;

void TASK_REGULATE(void *pdata)
{
  CPU_SR cpu_sr=0;

	pdata = pdata; 
	

	if(calibratePM())
	{
		PM_module.isWorking = CALIBRATE_FAIL;
	}
//	PM_CTRL = 1;
//	D_ON_TEST = 0;
//	D_ON = 1;
//	MODE_SWITCHER = 1;
	
	uint8_t pre_workstate = PM_module.isWorking;
	
	while(1)
	{
		if(pre_workstate != PM_module.isWorking)
		{
			PM_module.time = 0.0f;
			pre_workstate = PM_module.isWorking;
			trig_request = true;
		}
		PM_module.on = (PM_module.time < PM_module.on_t) && PM_module.isWorking == NORM;
		D_ON = PM_module.on;
		
		measurePM();

		PM_module.iCur = PM_module.acs_i_cur;
		
		if(PM_module.on)
		{
			if(PM_module.curMODE == VOL_MODE)
			{
				PM_module.iSet = PM_module.iCur;
			}
			else if(PM_module.curMODE == AMP_MODE)
			{
				if(PM_module.iCur > 0.1f)
				{
					float r_estimation = PM_module.vCur / PM_module.iCur;
					PM_module.vSet = PM_module.iSet * r_estimation;
					if(PM_module.vSet > 40.0f)
					{
						PM_module.vSet = 40.0f;
					}
					if(PM_module.vSet < 0.1f)
					{
						PM_module.vSet = 0.1f;
					}
				}
				else if(PM_module.vSet < 1.0f)
				{
					PM_module.vSet = 1.0f;
				}
//				else
//				{
//					PM_module.vSet = PM_module.vSet£»
//				}
			}
			
			PM_module.dac_out = (ZERO_VOL_OUT - PM_module.vSet) / R1_R3_RATIO;
			DacSetVol(PM_module.dac_out);
			
		}
		PM_module.time += 0.01;
		if(PM_module.time > PM_module.all_t)
		{
			PM_module.time = 0;
		}

		delay_ms(10);
	}
}

