#include "screenComponent.h"

char SCREEN_BUFFER[6][19] = 
{
	//page 0 
	"0.00A 0.00s 0.00",
	"0.00V   ___OFF__",
	//page 1
	"MODE ALL_T ON_T ",
	"VOL  0.00s 0.00s",
  //page 2
	"ID  I_LIM  V_LIM",
	" 0   6.0A  40.0V"
};




const char mode_string_COMBOX[MODE_NUM][MODE_STRING_LEN]=
{
	{"VOL"},
	{"AMP"},
};

COMP_BOX_Typedef PM_ModeBox = {(const char *)mode_string_COMBOX, MODE_NUM, 0, MODE_STRING_LEN, &PM_module.curMODE};

SCREEN_VAR_Typedef screen_vars[]=
{
	{0,0,0,FLOAT_T,0,0,NULL},
	//id 1-9
	{0,0,4,FLOAT_T,CURRENT_VAR_PROMP,0,0, &PM_module.iSet},    //current dispaly
	{0,0,4,FLOAT_T,CURRENT_VAR_PROMP,0,1, &PM_module.iCur},    //cur current dispaly
	{4,0,1,STRING,0,0,0, "A"},    //current promp
	{6,0,4,FLOAT_T,ALL_T_PROMP,0,0, &PM_module.all_t},   //ALL_T dispaly
	{12,0,4,FLOAT_T,ON_T_PROMP,0,0, &PM_module.on_t}, //ON_T dispaly
	{0,1,4,FLOAT_T,VOL_VAR_PROMP,0,0, &PM_module.vSet}, //voltage dispaly
	{0,1,4,FLOAT_T,VOL_VAR_PROMP,0,1, &PM_module.vCur}, //cur voltage dispaly
	{4,1,1,STRING,0,0,0, "V"},              //voltage promp
	{8,1,8,WAVE,0,0,0, &PM_module.time},   //wave dispaly	
	
	//id 10-15
	{0,2,4,STRING,0,0,0,"MODE"},   	//MODE string
	{5,2,5,STRING,0,0,0,"ALL_T"},     //AL_T string
	{11,2,4,STRING,0,0,0,"ON_T"}, 	 //ON_T string	
	{0,3,4,STRING_VAR, MODE_PROMP,0,0,&PM_ModeBox},   	//VOL/AMP string
	{5,3,4,FLOAT_T, ALL_T_PROMP,4,0,&PM_module.all_t},     //AL_T var
	{11,3,4,FLOAT_T,ON_T_PROMP,5,0,&PM_module.on_t}, 	 //ON_T var	
	
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

bool extScreenUpdate[COMPONENT_NUM];
