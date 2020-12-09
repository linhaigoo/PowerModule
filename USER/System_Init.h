#ifndef  _SYSTEM_INIT_H
#define  _SYSTEM_INIT_H
#include "sys.h"
#include "Led.h"

//#define USE_DialSwitch
//#define USE_jlx_lcd
//#define USE_TFT_lcd
//#define USE_OV7670
//#define USE_OV2640
//#define USE_MOTOR
//#define USE_ENCODER
//#define USE_GPIO
//#define USE_RemoteControl
//#define USE_HC_SR

#ifdef USE_DialSwitch
#include "DialSwitch.h"
#endif

#ifdef USE_TFT_lcd
#include "TFT_Lcd.h"
#endif

#ifdef USE_jlx_lcd
#include "jlx_lcd.h"
#endif

#ifdef USE_OV7670
#include "OV7670.h"
#endif

#ifdef USE_OV2640
#include "OV2640.h"
#endif

#ifdef USE_MOTOR
#include "motor.h"
#endif

#ifdef USE_ENCODER
#include "Encoder.h"
#endif

#ifdef USE_GPIO
#include "GPIO.h"
#endif

#ifdef USE_RemoteControl
#include "remote_control.h"
#endif

#ifdef USE_HC_SR
#include "HC_SR.h"
#endif

void System_Init(void);
#endif