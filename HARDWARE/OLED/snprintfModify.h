#ifndef __MY_SNPRINTF__
#define __MY_SNPRINTF__
#include "stdint.h"
int MySnprintfInt(int value, char   Buff[], uint8_t const BuffSize);

int MySnprintfFloat(float value, char   Buff[], uint8_t const BuffSize, int8_t const DecimalNum);
#endif

