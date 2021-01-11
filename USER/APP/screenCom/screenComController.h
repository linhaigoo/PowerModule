#ifndef __SCREEN_COM_CONTROLL_H__
#define __SCREEN_COM_CONTROLL_H__

#include "stdint.h"
#include "stdbool.h"
#include "powerModule.h"

typedef struct
{
	uint8_t manualDir;
	float manualDistance;
	float manualSpeed;
	float manualAcc;
	float manualDec;
	uint8_t manualHome;
	uint8_t manual_start_stop;
	
	uint32_t stretch_count;
	float distance;

	uint8_t single_stretch;
	uint8_t loop_stretch;
	uint8_t stretch_count_clc;

	float stretch_speed;
	float stretch_acc;
	float stretch_dec;
	
	float release_speed;
	float release_acc;
	float release_dec;
	
	float home_speed;

}SCREEN_SET_Typedef;


typedef enum
{
	MANUAL_DIR = 0,
	MANUAL_DISTANCE = 1,
	MANUAL_SPEED = 2,
	MANUAL_ACC = 3,
	MANUAL_DEC = 4,
	MANUAL_HOME = 5,
	MANUAL_START_STOP = 6,
	
	STRETCH_COUNT = 7,
	DISTANCE = 8,
	SINGLE_STRETCH = 9,
	LOOP_STRETCH = 10,
	STRETCH_COUNT_CLC = 11,
	
	STRETCH_SPEED = 12,
	STRETCH_ACC = 13,
	STRETCH_DEC = 14,
	
	RELEASE_SPEED = 15,
	RELEASE_ACC = 16,
	RELEASE_DEC = 17,
	
	HOME_SPPED = 18,
	
	SCREEN_SET_ORDER_LENGTH =19
}SCREEN_SET_ORDER_Typedef;



typedef enum
{
	TXT_FLOAT,
	TXT_UINT32_T,
	UINT8_T
}ControlValueTypedef;

typedef struct
{
	uint32_t idx;
	void * const addr;
	SCREEN_SET_ORDER_Typedef Order;
	ControlValueTypedef ValueType;
}VALUE_ID_ADDRESS_Typedef;

extern SCREEN_SET_Typedef screenSet;
extern const VALUE_ID_ADDRESS_Typedef ValueIdAddressArray[];
extern const int ValueIdAddressArrayLength;
extern bool RecValueUpdated[SCREEN_SET_ORDER_LENGTH];

#endif

