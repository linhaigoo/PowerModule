#include "oled.h"
#include "bmp.h"

#include "encoder.h"
#include "snprintfModify.h"
#include "screenComponent.h"
#include "InteractKeyScreen.h"


void InterectMangeInit(INTERACT_MANGE_Typedef * pIntrMange)
{
	pIntrMange->selectedVarID = VAR_EMPTY;
	pIntrMange->menuID = 0;
	pIntrMange->prompID = VAR_EMPTY;
	pIntrMange->isModifying = false;
	pIntrMange->unOperationDuration = 0;
	pIntrMange->curPage = 0xFF;
	pIntrMange->str = NULL;
	pIntrMange->str_InvMode = false;
}



#define BLANK_SPACE_PRESERVE  4
char snprintfTwoBuff[2][16] ={{' ',' ',' ', ' ', ' '},{' ',' ',' ', ' ', ' '}};


SCREEN_VAR_ID_Typedef varValueParser(SCREEN_VAR_ID_Typedef varID, PM_Typedef * pPM, void * pVarTmp, VAR_TypeEdef type)
{
	float float_tmp;
	uint8_t uint8_t_tmp;
	uint8_t string_id_tmp;
	SCREEN_VAR_ID_Typedef varUpdateID = VAR_EMPTY;
	switch(type)
	{
		case STRING_VAR:
			{
				string_id_tmp = *(uint8_t*) pVarTmp;
				COMP_BOX_Typedef * ptr = ((COMP_BOX_Typedef *)screen_vars[varID].ptr);
				switch(varID)
				{
					case MODE_VAR:
					{
						string_id_tmp = string_id_tmp % ptr->item_num;					
					}
						break;
					default:
						
						break;
				}
				*(uint8_t*)pVarTmp = string_id_tmp;
				
			}
			break;
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
					float_tmp = (float_tmp < pPM->I_LIM) ? float_tmp : pPM->I_LIM;
					if(pPM->curMODE == VOL_MODE)
					{					
						pPM->isWorking = false;
						pPM->curMODE = AMP_MODE;
						
						varUpdateID = MODE_VAR;
						((COMP_BOX_Typedef*)screen_vars[varUpdateID].ptr)->item_selected = pPM->curMODE;
					}
					break;
				case VOL_VAR:
					float_tmp = (float_tmp > 0) ? float_tmp : 0;
					float_tmp = (float_tmp < pPM->V_LIM) ? float_tmp : pPM->V_LIM;
					if(pPM->curMODE == AMP_MODE)
					{
						pPM->isWorking = false;
						pPM->curMODE = VOL_MODE;
						
						varUpdateID = MODE_VAR;
						((COMP_BOX_Typedef*)screen_vars[varUpdateID].ptr)->item_selected = pPM->curMODE;
					}
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
	const char * strTmp;
	int strLen = 0;
	char * strBuff;
	COMP_BOX_Typedef* pCompBox;
	char * snprintfBuff = snprintfTwoBuff[screen_vars[id].ifOverLap];
	switch(screen_vars[id].type)
	{
		case UINT8_T:
			strLen = MySnprintfInt(*(uint8_t *)screen_vars[id].ptr, &snprintfBuff[BLANK_SPACE_PRESERVE], screen_vars[id].spaceLen+1);
			break;
		case FLOAT_T:
			strLen = MySnprintfFloat(*(float *)screen_vars[id].ptr, &snprintfBuff[BLANK_SPACE_PRESERVE], screen_vars[id].spaceLen+1, -1);
			break;
		case STRING_VAR:
			pCompBox = (COMP_BOX_Typedef*)screen_vars[id].ptr;
		case STRING:
		    strTmp = screen_vars[id].type == STRING ? (char*)screen_vars[id].ptr :
					&pCompBox->boxString[pCompBox->item_selected * pCompBox->stringLen];
			strBuff = &snprintfBuff[BLANK_SPACE_PRESERVE];
			while(*strTmp != '\0' && strLen < 10)
			{
				*(strBuff++) = *(strTmp++);
				strLen++;
			}
			*strBuff = '\0';
			strLen++;
		default:
			break;
	}

	if(strLen > 0)
	{
		int spaceFill = screen_vars[id].spaceLen+1 - strLen;
		if(spaceFill >= 0 && spaceFill < BLANK_SPACE_PRESERVE)
		{
			return &snprintfBuff[BLANK_SPACE_PRESERVE-spaceFill];
		}
	}
	return NULL;
}

bool strDirectShow(SCREEN_VAR_ID_Typedef id)
{
	const char * str = strPrepare(id);
	if(str != NULL)
	{
		OLED_ShowStringAlign(screen_vars[id].pos_x, (screen_vars[id].pos_y % 2), str, false);
		return true;
	}
	return false;
}

uint8_t getPageOfVar(SCREEN_VAR_ID_Typedef id)
{
	return screen_vars[id].pos_y >>1;
}

bool isOnPage(INTERACT_MANGE_Typedef * pIntrMange, SCREEN_VAR_ID_Typedef id)
{
	return id != VAR_EMPTY && (getPageOfVar(id) == pIntrMange->curPage);
}


//sync : one parameter come up in two position

bool strFill(INTERACT_MANGE_Typedef * pIntrMange, SCREEN_VAR_ID_Typedef id, bool sync, bool inv_mode)
{
	if(id != VAR_EMPTY)
	{
		const char * str = strPrepare(id);
		if(str != NULL)
		{
			if(isOnPage(pIntrMange, id))
			{
				pIntrMange->str = str;
				pIntrMange->str_InvMode = inv_mode;
				OLED_ShowStringAlign(screen_vars[id].pos_x, (screen_vars[id].pos_y % 2), str, inv_mode);
			}
			
			if(sync)
			{
				const char * strTmp = str;
				char * pSCREEN_BUFFER_start = &SCREEN_BUFFER[screen_vars[id].pos_y][screen_vars[id].pos_x];
				while(*strTmp!='\0')
				{
					*(pSCREEN_BUFFER_start++) = *(strTmp++);
				}
				uint8_t rvar_id = screen_vars[id].rvar_id;
				if(rvar_id != VAR_EMPTY)
				{
					const char * strTmp = str;
					char * pSCREEN_BUFFER_start = &SCREEN_BUFFER[screen_vars[rvar_id].pos_y][screen_vars[rvar_id].pos_x];
					while(*strTmp!='\0')
					{
						*(pSCREEN_BUFFER_start++) = *(strTmp++);
					}
				}
			}
			return true;
		}
	}
	return false;
}


void interact(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM, uint8_t key_pressed, int16_t count)
{
	if(key_pressed || count != 0)
	{
		pIntrMange->unOperationDuration = 0;
	}
	else
	{
		pIntrMange->unOperationDuration++;
	}
	
	
	if(key_pressed)
	{
		key_pressed = false;
		if(pIntrMange->selectedVarID == VAR_EMPTY)
		{
			pPM->isWorking = false;
		}
		else
		{
			pIntrMange->isModifying = !pIntrMange->isModifying;
			if(pIntrMange->isModifying)
			{
				//menu promp >-->  value 
				OLED_ShowStringAlign(screen_vars[pIntrMange->prompID].pos_x, (screen_vars[pIntrMange->prompID].pos_y % 2), pIntrMange->str, false);
				strFill(pIntrMange, pIntrMange->selectedVarID, false, true);
			}
			else
			{

				strFill(pIntrMange, pIntrMange->selectedVarID, true, false);
				//menu promp <--<  value
				strFill(pIntrMange, pIntrMange->prompID, false, true);
			}
		}
	}
	else if(count != 0)
	{
		if(pIntrMange->isModifying)
		{
			float float_tmp;
			int16_t   int_tmp;
			SCREEN_VAR_ID_Typedef updateID;
			SCREEN_VAR_ID_Typedef name_id;
			switch(screen_vars[pIntrMange->selectedVarID].type)
			{
				case UINT8_T:
					int_tmp = *(uint8_t*)screen_vars[pIntrMange->selectedVarID].ptr + (count>>2);
				
					updateID = varValueParser(pIntrMange->selectedVarID, pPM, &int_tmp, UINT8_T);
					*(uint8_t*)screen_vars[pIntrMange->selectedVarID].ptr = int_tmp;
				break;
					
				case FLOAT_T:
					
				float_tmp = *(float*)screen_vars[pIntrMange->selectedVarID].ptr + 0.01 * count;
				
				updateID = varValueParser(pIntrMange->selectedVarID, pPM, &float_tmp, FLOAT_T);
				*(float*)screen_vars[pIntrMange->selectedVarID].ptr = float_tmp;
				
				break;

				case STRING_VAR:
				{
					COMP_BOX_Typedef * ptr = (COMP_BOX_Typedef *)screen_vars[pIntrMange->selectedVarID].ptr;
					
					name_id = ptr->item_selected + count;
					SCREEN_VAR_ID_Typedef updateID = varValueParser(pIntrMange->selectedVarID, pPM, &name_id, STRING_VAR);
					
					ptr->item_selected = name_id;
					*(ptr->varPtr) = name_id;
				}
					break;
				case WAVE:
					break;
				
				default:
					break;
			}

			if(updateID != VAR_EMPTY)
			{
				strFill(pIntrMange, updateID, true, false);
			}
			strFill(pIntrMange, pIntrMange->selectedVarID, false, true);
		}

		else
		{
			int16_t count_scale = count>>2;
			pIntrMange->menuID += count_scale;
			pIntrMange->menuID = pIntrMange->menuID % MENU_SIZE;
			pIntrMange->selectedVarID = screen_menu[pIntrMange->menuID];
			
			if(isOnPage(pIntrMange, pIntrMange->selectedVarID))
			{
				if(isOnPage(pIntrMange, pIntrMange->prompID))
				{
					OLED_ShowStringAlign(screen_vars[pIntrMange->prompID].pos_x, (screen_vars[pIntrMange->prompID].pos_y % 2), pIntrMange->str, false);
				}
			}
			else
			{
				//page switch
				pIntrMange->curPage = getPageOfVar(pIntrMange->selectedVarID);
				OLED_ShowStringAlign(0, 0, SCREEN_BUFFER[pIntrMange->curPage<<1], false);
				OLED_ShowStringAlign(0, 1, SCREEN_BUFFER[(pIntrMange->curPage<<1) + 1], false);
			}
			
			pIntrMange->prompID = screen_vars[pIntrMange->selectedVarID].promp_id;
			strFill(pIntrMange, pIntrMange->prompID, false, true);
		}
	}
	else
	{
		if(pIntrMange->unOperationDuration > LOST_FOCUS_TIME)
		{
			pIntrMange->unOperationDuration = LOST_FOCUS_TIME;
			if(pIntrMange->selectedVarID != VAR_EMPTY)
			{
				pIntrMange->selectedVarID = VAR_EMPTY;

				//lost focus display
				if(pIntrMange->isModifying)
				{
					pIntrMange->isModifying =false;
					strFill(pIntrMange, pIntrMange->selectedVarID, false, false);
				}
				else
				{
					OLED_ShowStringAlign(screen_vars[pIntrMange->prompID].pos_x, (screen_vars[pIntrMange->prompID].pos_y % 2), pIntrMange->str, false);
				}
			}
		}
		else if(pIntrMange->selectedVarID != VAR_EMPTY && !pIntrMange->isModifying)
		{
			//blink
			pIntrMange->str_InvMode = !pIntrMange->str_InvMode;

//			uint8_t id = pIntrMange->isModifying ? pIntrMange->selectedVarID : pIntrMange->prompID;
			uint8_t id = pIntrMange->prompID;

			OLED_ShowStringAlign(screen_vars[id].pos_x, (screen_vars[id].pos_y % 2), pIntrMange->str, pIntrMange->str_InvMode);
		}
		
	}
}


void showRealTimeVar(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM)
{
	if(pIntrMange->curPage == 0)
	{
		pPM->isWorking = true;//todo debug
		if(pPM->isWorking)
		{
			if(pIntrMange->isModifying && screen_vars[pIntrMange->selectedVarID].ifOverLap)
			{
				if(pIntrMange->unOperationDuration > OVER_LAP_VAR_TIME)
				{
					//exit var fune state
					pIntrMange->isModifying = false;
					strFill(pIntrMange, pIntrMange->prompID, false, true);
				}
			}

			if(!(pIntrMange->isModifying && pIntrMange->selectedVarID == CURRENT_VAR))
			{
				strDirectShow(CURRENT_CUR_VAR);
			}
			if(!(pIntrMange->isModifying && pIntrMange->selectedVarID == VOL_VAR))
			{
				strDirectShow(VOL_CUR_VAR);
			}
		}
		else
		{
				strDirectShow(CURRENT_VAR);
				strDirectShow(VOL_VAR);

				//specific
				OLED_ShowStringAlign(screen_vars[WAVE_DIAPLAY].pos_x, (screen_vars[WAVE_DIAPLAY].pos_y % 2), "___OFF__", false);
		}
		
	}
}

void InteractInit(INTERACT_MANGE_Typedef * pIntrMange)
{
	ENC_KEY_Init();
	OLED_Init();	
	OLED_Clear(); 
	
	InterectMangeInit(pIntrMange);
	
	strFill(pIntrMange, I_LIM_VAR, true, false);
	strFill(pIntrMange, V_LIM_VAR, true, false);
	strFill(pIntrMange, I_LIM_VAR, true, false);
	strFill(pIntrMange, MODE_VAR, true, false);
	
	pIntrMange->curPage = 0;
	OLED_ShowStringAlign(0, 0, SCREEN_BUFFER[pIntrMange->curPage<<1], false);
	OLED_ShowStringAlign(0, 1, SCREEN_BUFFER[(pIntrMange->curPage<<1) + 1], false);
}

INTERACT_MANGE_Typedef InteractMange;
