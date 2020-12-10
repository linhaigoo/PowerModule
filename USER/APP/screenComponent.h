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
}VAR_TypeEdef;


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
	
}SCREEN_VAR_ID_Typedef;

typedef struct
{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t spaceLen;
	VAR_TypeEdef type;
	SCREEN_VAR_ID_Typedef promp_id;
	uint8_t rvar_id; //relative var id
	uint8_t ifOverLap;
	void * ptr;
}SCREEN_VAR_Typedef;


char SCREEN_BUFFER[6][19] = 
{
	//page 0 
	"  0A    0s    0s",
	"  0V    ___OFF__",
	//page 1
	"MODE  AL_T  ON_T",
	"VOL     0s    0s",
  //page 2
	"ID  I_LIM  V_LIM",
	" 0   6.0A  40.0V"
};


typedef struct
{
	const char *boxString;
	uint8_t item_num;
	uint8_t item_selected;
	uint8_t stringLen;
	uint8_t * varPtr;

}COMP_BOX_Typedef;

#define MODE_STRING_LEN 5
const char mode_string_COMBOX[MODE_NUM][MODE_STRING_LEN]=
{
	{"VOL"},
	{"AMP"},
};

COMP_BOX_Typedef PM_ModeBox = {(const char *)mode_string_COMBOX, MODE_NUM, 0, MODE_STRING_LEN, &PM_module.curMODE};

SCREEN_VAR_Typedef screen_vars[]=
{
	{0,0,0,0,0,0,NULL},
	//id 1-9
	{0,0,3,FLOAT_T,CURRENT_VAR_PROMP,0,0, &PM_module.iSet},    //current dispaly
	{0,0,3,FLOAT_T,CURRENT_VAR_PROMP,0,1, &PM_module.iCur},    //cur current dispaly
	{3,0,1,STRING,0,0,0, "A"},    //current promp
	{5,0,3,FLOAT_T,ALL_T_PROMP,0,0, &PM_module.all_t},   //ALL_T dispaly
	{11,0,3,FLOAT_T,ON_T_PROMP,0,0, &PM_module.on_t}, //ON_T dispaly
	{0,1,3,FLOAT_T,VOL_VAR_PROMP,0,0, &PM_module.vSet}, //voltage dispaly
	{0,1,3,FLOAT_T,VOL_VAR_PROMP,0,1, &PM_module.vCur}, //cur voltage dispaly
	{3,1,1,STRING,0,0,0, "V"},              //voltage promp
	{8,1,8,WAVE,0,0,0, &PM_module.time},   //wave dispaly	
	
	//id 10-15
	{0,2,4,STRING,0,0,0,"MODE"},   	//MODE string
	{6,2,4,STRING,0,0,0,"AL_T"},     //AL_T string
	{12,2,4,STRING,0,0,0,"ON_T"}, 	 //ON_T string	
	{0,3,4,STRING_VAR, MODE_PROMP,0,0,&PM_ModeBox},   	//VOL/AMP string
	{6,3,3,FLOAT_T, ALL_T_PROMP,0,0,&PM_module.all_t},     //AL_T var
	{12,3,3,FLOAT_T,ON_T_PROMP,0,0,&PM_module.on_t}, 	 //ON_T var	
	
	//id 16-21
	{0,4,3,STRING,0,0,0,"ID"},   	//ID string
	{4,4,5,STRING,0,0,0,"I_LIM"},     //I_LIM string
	{11,4,5,STRING,0,0,0,"V_LIM"}, 	  //V_LIM string	
	{0,5,3,UINT8_T,ID_PROMP,0,0,&PM_module.ID},   	//ID var
	{4,5,4,FLOAT_T,I_LIM_PROMP,0,0,&PM_module.I_LIM},    //I_LIM var
	{11,5,4,FLOAT_T,V_LIM_PROMP,0,0,&PM_module.V_LIM}, 	//V_LIM var	
};


const SCREEN_VAR_ID_Typedef screen_menu[]=
{
	CURRENT_VAR, VOL_VAR, MODE_VAR, ALL_T_VAR, ON_T_VAR, ID_VAR, I_LIM_VAR, V_LIM_VAR,
};
const uint8_t MENU_SIZE = sizeof(screen_menu) / sizeof(SCREEN_VAR_ID_Typedef);

#define MODE_STRING_INDEX_START 2
#define MODE_STRING_INDEX_STOP 3


#endif