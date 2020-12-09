#ifndef _GPIO_H_
#define _GPIO_H_
#include "sys.h"

#define USE_IO19

void GPIO_Config();


//GPIO1
#ifdef USE_IO0
#define IO0_STATE PFin(14)
#endif

#ifdef USE_IO1
#define IO1_STATE PFin(15)
#endif

#ifdef USE_IO2
#define IO2_STATE PGin(0)
#endif

#ifdef USE_IO3
#define IO3_STATE PGin(1)
#endif

#ifdef USE_IO4
#define IO4_STATE PGin(2)
#endif

#ifdef USE_IO5
#define IO5_STATE PGin(3)
#endif

#ifdef USE_IO6
#define IO6_STATE PGin(4)
#endif

#ifdef USE_IO7
#define IO7_STATE PGin(5)
#endif

#ifdef USE_IO8
#define IO8_STATE PGin(6)
#endif

#ifdef USE_IO9
#define IO9_STATE PGin(7)
#endif


//GPIO2
#ifdef USE_IO19
#define IO19_STATE PFout(10)
#endif
#endif
