#ifndef __INTERACT_KEY_SCREEN_H__
#define __INTERACT_KEY_SCREEN_H__
#include "stdint.h"
#include "PowerModule.h"

#define LOST_FOCUS_TIME 200
#define OVER_LAP_VAR_TIME 20


typedef struct
{
	uint8_t selectedVarID;
	uint8_t menuID;
	uint8_t prompID;
	uint8_t isModifying;
	uint8_t isFocusLost;
	uint16_t unOperationDuration;
	uint8_t curPage;
	const char * str;
	uint8_t str_InvMode;
}INTERACT_MANGE_Typedef;


void InterectMangeInit(INTERACT_MANGE_Typedef * pIntrMange);
void InteractInit(INTERACT_MANGE_Typedef * pIntrMange);
void interact(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM, uint8_t key_pressed, short count);
void showRealTimeVar(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM);
void dynamicShow(INTERACT_MANGE_Typedef * pIntrMange);
void realTimeInteract(INTERACT_MANGE_Typedef * pIntrMange, PM_Typedef * pPM, uint8_t key_pressed, int16_t count);

extern INTERACT_MANGE_Typedef InteractMange;
#endif
