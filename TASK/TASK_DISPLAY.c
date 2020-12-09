#include "task_init.h"
#include "oled.h"
#include "bmp.h"

#include "encoder.h"
#include "snprintfModify.h"
/***********************************
外部输入请加在该任务中
************************************/


/******全局状态变量声明********/

/***************************/

#define ILIM_MAX 6.5f
#define VLIM_MAX 42.0f
#define ID_MAX   8

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
	" 0    6A    42V "
};

typedef enum
{
	UINT8_T,
	FLOAT_T,
	STRING,
	WAVE,
}VAR_TypeEdef;

typedef struct
{
	uint8_t pos_x;
	uint8_t pos_y;
	uint8_t spaceLen;
	VAR_TypeEdef type;
	uint8_t name_id;
	uint8_t rvar_id; //relative var id
	uint8_t ifOverLap;
	void * ptr;
}SCREEN_VAR_Typedef;

typedef enum
{
	VAR_EMPTY = 0,
	CURRENT_VAR = 1,
	ALL_T_DISPALY = 2,
	ON_T_DISPALY = 3,
	VOL_VAR = 4,
	WAVE_DIAPLAY = 5,
	
	MODE_PROMP = 6,
	ALL_T_PROMP = 7,
	ON_T_PROMP = 8,
	MODE_VAR = 9,
	ALL_T_VAR = 10,
	ON_T_VAR = 11,
	
	ID_PROMP = 12,
	I_LIM_PROMP = 13,
	V_LIM_PROMP = 14,
	ID_VAR = 15,
	I_LIM_VAR = 16,
	V_LIM_VAR = 17,
}SCREEN_VAR_ID_Typedef;


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
	uint8_t curMODE;
	uint8_t ID;
	uint8_t isWorking;
}PM_Typedef;

PM_Typedef PM_module;

SCREEN_VAR_Typedef screen_vars[]=
{
	{0,0,0,0,0,0,NULL},
	//id 1-5
	{0,0,3,FLOAT_T,0,0,1, &PM_module.iCur},    //current dispaly 
	{5,0,3,FLOAT_T,0,0,0, &PM_module.all_t},   //ALL_T dispaly
	{11,0,3,FLOAT_T,0,0,0, &PM_module.on_t}, //ON_T dispaly
	{0,1,3,FLOAT_T,0,0,1, &PM_module.vCur}, //voltage dispaly
	{8,1,8,WAVE,0,0,1, &PM_module.time},   //wave dispaly	
	
	//id 6-11
	{0,2,4,STRING,0,0,0,NULL},   	//MODE string
	{6,2,4,STRING,0,0,0,NULL},     //AL_T string
	{12,2,4,STRING,0,0,0,NULL}, 	 //ON_T string	
	{0,3,4,STRING,0,0,0,&PM_module.curMODE},   	//VOL/AMP string
	{6,3,4,FLOAT_T,0,ALL_T_DISPALY,0,&PM_module.all_t},     //AL_T var
	{12,3,4,FLOAT_T,0,ON_T_DISPALY,0,&PM_module.on_t}, 	 //ON_T var	
	
	//id 12-17
	{0,4,4,STRING,0,0,0,NULL},   	//ID string
	{6,4,4,STRING,0,0,0,NULL},     //I_LIM string
	{12,4,4,STRING,0,0,0,NULL}, 	  //V_LIM string	
	{0,5,3,UINT8_T,0,0,0,&PM_module.ID},   	//ID var
	{6,5,4,FLOAT_T,0,0,0,&PM_module.I_LIM},    //I_LIM var
	{12,5,4,FLOAT_T,0,0,0,&PM_module.V_LIM}, 	//V_LIM var	
};

typedef struct
{
	uint8_t selectedVarID;
	uint8_t isModifying;
	uint16_t unOperationDuration;
	uint8_t curPage;
}INTERACT_MANGE_Typedef;



char snprintfBuff[16];
#define BLANK_SPACE_PRESERVE  4


SCREEN_VAR_ID_Typedef varValueParser(SCREEN_VAR_ID_Typedef varID, PM_Typedef * pPM, void * pVarTmp, VAR_TypeEdef type)
{
	float float_tmp;
	uint8_t uint8_t_tmp;
	SCREEN_VAR_ID_Typedef varUpdateID = VAR_EMPTY;
	switch(type)
	{
		case UINT8_T:
			uint8_t_tmp = *(uint8_t*) pVarTmp;
			switch(varID)
			{
				case ID_VAR:
					uint8_t_tmp = uint8_t_tmp % (ID_MAX+1);
				default:
					
					break;
			}
			*(uint8_t*)pVarTmp = uint8_t_tmp;
			break;
			
		case FLOAT_T:
			float_tmp = *(float*) pVarTmp;
			switch(varID)
			{
				case CURRENT_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					float_tmp = (float_tmp < pPM->I_LIM) ? float_tmp : 0;
					break;
				case VOL_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					float_tmp = (float_tmp < pPM->V_LIM) ? float_tmp : 0;
					break;
				case ALL_T_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					if(float_tmp < pPM->on_t)
					{
						pPM->on_t = float_tmp;
						varUpdateID = ON_T_VAR;
					}
					break;
				case ON_T_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					if(float_tmp > pPM->all_t)
					{
						pPM->all_t = float_tmp;
						varUpdateID = ALL_T_VAR;
					}
					break;
					
				case I_LIM_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					float_tmp = (float_tmp < ILIM_MAX) ? float_tmp : 0;
					break;
					
				case V_LIM_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					float_tmp = (float_tmp < VLIM_MAX) ? float_tmp : 0;
					break;
				default:
					break;
			}
			*(float*)pVarTmp = float_tmp;
			break;
					
				default:
					break;
			}
	
	return varUpdateID;
}


const char * strPrepare(SCREEN_VAR_ID_Typedef id)
{
	int strLen;
	switch(screen_vars[id].type)
	{
		case UINT8_T:
			strLen = MySnprintfInt(*(uint8_t *)screen_vars[id].ptr, &snprintfBuff[BLANK_SPACE_PRESERVE], screen_vars[id].spaceLen+1);
			break;
		case FLOAT_T:
			strLen = MySnprintfFloat(*(float *)screen_vars[id].ptr, &snprintfBuff[BLANK_SPACE_PRESERVE], screen_vars[id].spaceLen+1, 1);
			break;
		default:
			break;
	}
	if(strLen > 0)
	{
		int spaceFill = screen_vars[id].spaceLen - strLen;
		if(spaceFill > 0 && spaceFill < BLANK_SPACE_PRESERVE)
		{
			return &snprintfBuff[BLANK_SPACE_PRESERVE-spaceFill];
		}
	}
	return NULL;
}


bool strFill(INTERACT_MANGE_Typedef * pIntrMange, SCREEN_VAR_ID_Typedef id, bool sync, bool inv_mode)
{
	if(id != VAR_EMPTY)
	{
		const char * str = strPrepare(id);
		if(str != NULL)
		{
			if()
			OLED_ShowString(screen_vars[id].pos_x, (screen_vars[id].pos_y % 2) << 1, str, inv_mode);
			if(sync)
			{
				char * pSCREEN_BUFFER_start = &SCREEN_BUFFER[screen_vars[rvar_id].pos_y][screen_vars[rvar_id].pos_x];
				while(*(str++)!='\0')
				{
					*(pSCREEN_BUFFER_start++) = *(str++);
				}
				u8 rvar_id = screen_vars[id].rvar_id;
				if(rvar_id != VAR_EMPTY)
				{
					char * pSCREEN_BUFFER_start = &SCREEN_BUFFER[screen_vars[rvar_id].pos_y][screen_vars[rvar_id].pos_x];
					while(*(str++)!='\0')
					{
						*(pSCREEN_BUFFER_start++) = *(str++);
					}
				}
			}
			return true;
		}
	}
	return false;
}


void interact(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM, u8 key_pressed, s16 count)
{
	if(key_pressed)
	{
		if(pIntrMange->isModifying == VAR_EMPTY)
		{
			pPM->isWorking = !pPM->isWorking;
		}
		else
		{
			pIntrMange->isModifying = !pIntrMange->isModifying;
			if(pIntrMange->isModifying)
			{		
				strFill(pIntrMange->selectedVarID, false, true);
			}
			else
			{
				strFill(pIntrMange->selectedVarID, true, false);
			}
		}
	}
	else if(count != 0)
	{
		if(pIntrMange->isModifying)
		{
			int strLen;
			float float_tmp;
			s16   int_tmp;
			switch(screen_vars[pIntrMange->selectedVarID].type)
			{
				case UINT8_T:
					int_tmp = *(uint8_t*)screen_vars[pIntrMange->selectedVarID].ptr + (count>>2);
					SCREEN_VAR_ID_Typedef updateID = varValueParser(pIntrMange->selectedVarID, pPM, &float_tmp, FLOAT_T);
					*(uint8_t*)screen_vars[pIntrMange->selectedVarID].ptr = float_tmp;
				break;
					
				case FLOAT_T:
					
				float_tmp = *(float*)screen_vars[pIntrMange->selectedVarID].ptr + 0.01 * count;
				SCREEN_VAR_ID_Typedef updateID = varValueParser(pIntrMange->selectedVarID, pPM, &float_tmp, FLOAT_T);
				if()
				*(float*)screen_vars[pIntrMange->selectedVarID].ptr = float_tmp;
				strFill(updateID, false, true);
				strFill(pIntrMange->selectedVarID, true);

				case STRING:
					break;
				case WAVE:
					break;
			}
			if(screen_vars[pIntrMange->selectedVarID].rvar_id != 0)
			{
				strFill(pIntrMange, screen_vars[pIntrMange->selectedVarID].rvar_id, strLen, true);
			}			
		}
	}
}

void TASK_DISPLAY(void *pdata)
{
  CPU_SR cpu_sr=0;
	u8 recv;
	static u8 IsNeg = 0;
	int count = 0;
	pdata = pdata; 
	

	ENC_KEY_Init();
/************initial************/
	OLED_Init();	
	OLED_Clear(); 
	
/***********************************/
	
	while(1)
	{
		s32 encPos;
		delay_ms(100);
		encPos = ENC_Read(ENC_KEY1);
		MySnprintfInt(encPos, Buff, 20);
		
		/************************/
		//实时显示
		OLED_ShowString(0,0, SCREEN_BUFFER[count%6],0);
		OLED_ShowString(0,2, SCREEN_BUFFER[count%6+1],0);
		count+=2;
			delay_ms(1000);
		/***********************/
	}
}

