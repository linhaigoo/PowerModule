#ifndef __SCREEN_COMONENT_H__
#define __SCREEN_COMONENT_H__

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "PowerModule.h"



typedef enum
{
	UINT8_T,
	FLOAT_T,
	STRING,
	STRING_VAR,
	WAVE,
}VAR_Typedef;


typedef enum
{
	VAR_EMPTY = 0,
	CURRENT_VAR = 1,
	CURRENT_CUR_VAR = 2,
	CURRENT_VAR_PROMP = 3,
	ALL_T_DISPALY = 4,
	ON_T_DISPALY = 5,
	VOL_VAR = 6,
	VOL_CUR_VAR = 7,
	VOL_VAR_PROMP = 8,
	WAVE_DIAPLAY = 9,
	
	MODE_PROMP = 10,
	ALL_T_PROMP = 11,
	ON_T_PROMP = 12,
	MODE_VAR = 13,
	ALL_T_VAR = 14,
	ON_T_VAR = 15,
	
	ID_PROMP = 16,
	I_LIM_PROMP = 17,
	V_LIM_PROMP = 18,
	ID_VAR = 19,
	I_LIM_VAR = 20,
	V_LIM_VAR = 21,
	COMPONENT_NUM = 22,
	
}SCREEN_VAR_ID_Typedef;

typedef struct
{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t spaceLen;
	VAR_Typedef type;
	uint8_t promp_id;
	uint8_t rvar_id; //relative var id
	uint8_t ifOverLap;
	void * ptr;
}SCREEN_VAR_Typedef;


extern char SCREEN_BUFFER[6][19];


typedef struct
{
	const char *boxString;
	uint8_t item_num;
	uint8_t item_selected;
	uint8_t stringLen;
	uint8_t * varPtr;

}COMP_BOX_Typedef;

#define MODE_STRING_LEN 5
extern const char mode_string_COMBOX[MODE_NUM][MODE_STRING_LEN];

extern COMP_BOX_Typedef PM_ModeBox;

extern SCREEN_VAR_Typedef screen_vars[];


extern const SCREEN_VAR_ID_Typedef screen_menu[];
extern const uint8_t MENU_SIZE;

extern bool extScreenUpdate[COMPONENT_NUM];

#define MODE_STRING_INDEX_START 2
#define MODE_STRING_INDEX_STOP 3


#endif

