#include "task_init.h"
#include "usart.h"

/******ȫ��״̬��������********/

/***************************/


void TASK_UART(void*pdata)
{
	OS_ERR err;
	while(1)
	{
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}
