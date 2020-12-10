#include "task_init.h"
#include "encoder.h"
#include "InteractKeyScreen.h"

void TASK_DISPLAY(void *pdata)
{
  CPU_SR cpu_sr=0;

		s32 encCount;
		u8 key_press = false;
	
	pdata = pdata; 
	
	InteractInit(&InteractMange);

	
	while(1)
	{

		encCount = ENC_Read(ENC_KEY1);
		key_press = KEY_Read(ENC_KEY1);
		interact(&InteractMange, &PM_module, key_press, encCount);
		showRealTimeVar(&InteractMange, &PM_module);
		
			delay_ms(200);
		/***********************/
	}
}

