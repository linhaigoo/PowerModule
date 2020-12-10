#include "task_init.h"
#include "PowerModule.h"

void TASK_REGULATE(void *pdata)
{
  CPU_SR cpu_sr=0;

	int count = 0;

	pdata = pdata; 
	bool dir = true;
	while(1)
	{
		if(dir)
		{
			PM_module.vCur += 0.2;
			PM_module.iCur += 0.15;
		}
		
		else
		{
			PM_module.vCur -= 0.2;
			PM_module.iCur -= 0.15;
		}
		
		
		if(PM_module.vCur > 10.0f)
		{
			dir = false;
		}
		if(PM_module.vCur < 0.2f)
		{
			dir = true;
		}
		delay_ms(1000);
	}
}

