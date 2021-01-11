/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-10-19 14:06:19
 * @modify date 2020-10-19 14:06:19
 * @desc [description]
 */
#ifndef __SCREEN_COM_H__
#define __SCREEN_COM_H__

#include "stdint.h"

void screenComInit(void);
void screenComUnitest(void);
void screenInfoRec(void);

void screenCmd(uint16_t cmd);
void screenInfoSet(uint32_t value_idx, void* pdata);

#endif

