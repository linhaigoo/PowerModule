#ifndef  _SYSTEM_INIT_H
#define  _SYSTEM_INIT_H
#include "sys.h"
#include "Led.h"

#define USE_485_COM


#ifdef USE_485_COM
#include "usart.h"
#endif

void System_Init(void);
#endif
