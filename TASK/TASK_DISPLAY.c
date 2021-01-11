#include "task_init.h"
#include "encoder.h"
#include "InteractKeyScreen.h"

void TASK_DISPLAY(void *pdata)
{
  CPU_SR cpu_sr=0;

		s32 encCount;
		u8 key_press = false;
	
	pdata = pdata; 
//	delay_ms(100);
	InteractInit(&InteractMange);

	
	while(1)
	{

		encCount = -ENC_Read(ENC_KEY1);
		key_press = KEY_Read(ENC_KEY1);
		if(key_press)PM_module.on = !PM_module.on;
		
		interact(&InteractMange, &PM_module, key_press, encCount);
		encCount = -ENC_Read(ENC_KEY2);
		key_press = KEY_Read(ENC_KEY2);
		realTimeInteract(&InteractMange, &PM_module, key_press, encCount);
		showRealTimeVar(&InteractMange, &PM_module);
		dynamicShow(&InteractMange, &PM_module);
		extComDisplay(&InteractMange);
		
			delay_ms(200);
		/***********************/
	}
}

