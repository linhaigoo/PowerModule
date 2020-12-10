#ifndef __POWER_MODULE_H__
#define __POWER_MODULE_H__

#include "stdint.h"

#define PM_ID 2

#define ILIM_MAX 6.5f
#define VLIM_MAX 42.0f
#define ILIM_INIT 6.0f
#define VLIM_INIT 40.0f
#define ID_MAX   8
#define MODE_NUM 2
#define VOL_MODE 0
#define AMP_MODE 1

typedef struct
{
	float I_LIM;
	float V_LIM;
	float ON_T_DUTY_LIM;
	float vSet;
	float iSet;
	float vCur;
	float iCur;
	float on_t;
	float all_t;
	float time;
	uint8_t curMODE;//  0 :vol 1:amp
	uint8_t ID;
	uint8_t isWorking;
}PM_Typedef;


extern PM_Typedef PM_module;

void PM_Init(PM_Typedef * pPM);

#endif