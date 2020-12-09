#include "task_init.h"
#include "usart.h"

/******全局状态变量声明********/

/***************************/


void TASK_UART(void*pdata)
{
	OS_ERR err;
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}
