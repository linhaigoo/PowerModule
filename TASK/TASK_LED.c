#include "task_init.h"
#include "led.h"
void TASK_LED(void *pdata)
{
  CPU_SR cpu_sr=0;
	pdata = pdata; 
	while(1)
	{
		delay_ms(500);
		LED1_SET = ~ LED1_SET; 
	}
}

