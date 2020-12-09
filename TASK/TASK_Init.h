#ifndef __TASK_INIT_H
#define __TASK_INIT_H
//#include "TASK.h"
#include "includes.h"
#include "delay.h"
/*********************************************************************/
/*TASK_START*/
/*********************************************************************/
#define  TASK_START_PRIO                        2u            
#define  TASK_START_STK_SIZE                    256u
static  OS_TCB  TASK_START_TCB;
static  CPU_STK  TASK_START_Stk[TASK_START_STK_SIZE];
static  void  TaskStart(void     *p_arg);
/*********************************************************************/
/*UART*/
/*********************************************************************/
#define  TASK_UART_PRIO                         24u
#define  TASK_UART_STK_SIZE                     1024u
static  OS_TCB   TASK_UART_TCB;
static  CPU_STK   TASK_UART_Stk[TASK_UART_STK_SIZE];
void TASK_UART(void *pdata);
/*********************************************************************/
/*TASK_LED*/
/*********************************************************************/
#define  TASK_LED_PRIO                          22u
#define  TASK_LED_STK_SIZE                      64u
static  OS_TCB   TASK_LED_TCB;
static  CPU_STK   TASK_LED_Stk[TASK_LED_STK_SIZE];
void TASK_LED(void *pdata);
/*********************************************************************/
/*TASK_DISPLAY*/
/*********************************************************************/
#define  TASK_DISPLAY_PRIO                       13u
#define  TASK_DISPLAY_STK_SIZE                   512u
static  OS_TCB   TASK_DISPLAY_TCB;
static  CPU_STK   TASK_DISPLAY_Stk[TASK_DISPLAY_STK_SIZE];
void TASK_DISPLAY(void *pdata);
/*********************************************************************/
/**************************END***************************************/
#endif
