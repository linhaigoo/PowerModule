#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "includes.h"
#include "TASK_Init.h"
#include "System_Init.h"


int main(void)
{
    OS_ERR err;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    System_Init();
    delay_init();
    OSInit(&err);

    /*创建起始任务*/
    OSTaskCreate ((OS_TCB       *)&TASK_START_TCB,
                  (CPU_CHAR     *)"Task Start",
                  (OS_TASK_PTR   )TaskStart,
                  (void         *)0,
                  (OS_PRIO       )TASK_START_PRIO,
                  (CPU_STK      *)&TASK_START_Stk[0],
                  (CPU_STK_SIZE  )TASK_START_STK_SIZE / 10,
                  (CPU_STK_SIZE  )TASK_START_STK_SIZE,
                  (OS_MSG_QTY    )0,
                  (OS_TICK       )0,
                  (void         *)0,
                  (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                  (OS_ERR       *)&err);
    OSStart(&err);
    (void)&err;
}

void TaskStart(void *p_arg)
{
    OS_ERR err;
    CPU_SR cpu_sr = 0;
    p_arg = p_arg;
    CPU_Init();    // Initialize the uC/CPU services
    Mem_Init();    // Initialize Memory Management Module
    Math_Init();
    
	
	  OS_CRITICAL_ENTER();
    OSTaskCreate  ((OS_TCB       *)&TASK_LED_TCB,
                   (CPU_CHAR     *)"LED Task",
                   (OS_TASK_PTR   )TASK_LED,
                   (void         *)0,
                   (OS_PRIO       )TASK_LED_PRIO,
                   (CPU_STK      *)&TASK_LED_Stk[0],
                   (CPU_STK_SIZE  )TASK_LED_STK_SIZE / 10,
                   (CPU_STK_SIZE  )TASK_LED_STK_SIZE,
                   (OS_MSG_QTY    )0,
                   (OS_TICK       )0,
                   (void         *)0,
                   (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                   (OS_ERR       *)&err);

    OSTaskCreate  ((OS_TCB       *)&TASK_UART_TCB,
                   (CPU_CHAR     *)"USART Task",
                   (OS_TASK_PTR   )TASK_UART,
                   (void         *)0,
                   (OS_PRIO       )TASK_UART_PRIO,
                   (CPU_STK      *)&TASK_UART_Stk[0],
                   (CPU_STK_SIZE  )TASK_UART_STK_SIZE / 10,
                   (CPU_STK_SIZE  )TASK_UART_STK_SIZE,
                   (OS_MSG_QTY    )0,
                   (OS_TICK       )0,
                   (void         *)0,
                   (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                   (OS_ERR       *)&err);
									 
		OSTaskCreate  ((OS_TCB       *)&TASK_DISPLAY_TCB,
							 (CPU_CHAR     *)"DISPLAY Task",
							 (OS_TASK_PTR   )TASK_DISPLAY,
							 (void         *)0,
							 (OS_PRIO       )TASK_DISPLAY_PRIO,
							 (CPU_STK      *)&TASK_DISPLAY_Stk[0],
							 (CPU_STK_SIZE  )TASK_DISPLAY_STK_SIZE / 10,
							 (CPU_STK_SIZE  )TASK_DISPLAY_STK_SIZE,
							 (OS_MSG_QTY    )0,
							 (OS_TICK       )0,
							 (void         *)0,
							 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 (OS_ERR       *)&err);
								 
		
    OSTaskSuspend((OS_TCB *)&TASK_START_TCB, (OS_ERR *) &err);  //挂起起始任务
    
		OS_CRITICAL_EXIT();							 
	 
}



void assert_failed(uint8_t* file, uint32_t line)
{
	
}

