/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-11-13 18:04:21
 * @modify date 2020-11-13 18:04:21
 * @desc [description]
 */
#include "screenCom.h"
#include "screenComController.h"


void screenComUnitest(void)
{
	screenSet.distance = -100.54;
	screenSet.stretch_speed = -200.66;
	screenSet.stretch_count = -500;
	screenSet.manualDir = -12;
	screenSet.manual_start_stop = -13;
	screenSet.manualHome = -14;
	screenSet.stretch_count_clc = 1;
	
	screenComInit();
//	uint32_t test_start_time = HAL_GetTick();
//	while((uint32_t)(HAL_GetTick() - test_start_time) < 60*1000) //30s test_time
//	{
//		for(int i = 0; i < ValueIdAddressArrayLength; i++)
//		{
//			const VALUE_ID_ADDRESS_Typedef *pTemp = &ValueIdAddressArray[i];
//			if(pTemp->idx==0x0052 || pTemp->idx== (2<<16 | 0x0016)|| pTemp->idx == (1<<16 | 0x0035))continue;
//			screenInfoSet(pTemp->idx, (void *)(pTemp->addr));
//				//send
//			HAL_Delay(5);
//		}	
//		screenInfoRec();
//		screenSet.stretch_dec = -screenSet.stretch_acc;
//		screenSet.single_stretch = !screenSet.loop_stretch;
//		HAL_Delay(50);
//	}
}

