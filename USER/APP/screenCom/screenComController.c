#include "screenComController.h"


const VALUE_ID_ADDRESS_Typedef ValueIdAddressArray[]=
{
	//canvas 0:
	{0x0047,	&screenSet.distance, 					DISTANCE, 				TXT_FLOAT},
	{0x0051,	&screenSet.stretch_speed,			STRETCH_SPEED ,		TXT_FLOAT},
	{0x0052,	&screenSet.stretch_acc,				STRETCH_ACC,  		TXT_FLOAT},
	{0x0053,	&screenSet.stretch_dec,	  		STRETCH_DEC,  		TXT_FLOAT},
	
	{13,	&screenSet.release_speed,			RELEASE_SPEED ,		TXT_FLOAT},
	{14,	&screenSet.release_acc,				RELEASE_ACC,  		TXT_FLOAT},
	{20,	&screenSet.release_dec,	  		RELEASE_DEC,  		TXT_FLOAT},
	
	//canvas 1:
	{1<<16 | 0x0032,	&screenSet.manualDir,				MANUAL_DIR,				UINT8_T},
	{1<<16 | 0x0033,	&screenSet.manualDistance,	MANUAL_DISTANCE,		TXT_FLOAT},
	{1<<16 | 0x0034,	&screenSet.manualSpeed,			MANUAL_SPEED,			TXT_FLOAT},
	{1<<16 | 0x0035,	&screenSet.manualAcc,				MANUAL_ACC,		TXT_FLOAT},
	{1<<16 | 0x0036,	&screenSet.manualDec,				MANUAL_DEC,		TXT_FLOAT},
	{1<<16 | 0x003C,	&screenSet.manual_start_stop,  MANUAL_START_STOP,	UINT8_T},
	{1<<16 | 0x003E,	&screenSet.manualHome,			MANUAL_HOME,				UINT8_T},
	
	//canvas 2:	
	{2<<16 | 0x0022,	&screenSet.stretch_count,		STRETCH_COUNT,		TXT_UINT32_T},
	{2<<16 | 0x0020,	&screenSet.distance,				DISTANCE,					TXT_FLOAT},
	{2<<16 | 0x0006,	&screenSet.stretch_speed,		STRETCH_SPEED,		TXT_FLOAT},
	{2<<16 | 0x0015,	&screenSet.single_stretch,	SINGLE_STRETCH,		UINT8_T},
	{2<<16 | 0x0016,	&screenSet.loop_stretch,		LOOP_STRETCH,			UINT8_T},
	{2<<16 | 0x0003,	&screenSet.stretch_count_clc,			STRETCH_COUNT_CLC,		UINT8_T}
};

const int ValueIdAddressArrayLength = sizeof(ValueIdAddressArray) / sizeof(VALUE_ID_ADDRESS_Typedef);