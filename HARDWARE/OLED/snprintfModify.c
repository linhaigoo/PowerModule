/*
* Description : transform unsigned int to string
* Argument(s) : IntNum    unsigned int var
*               Buff      output buffer
*               BuffSize  size of output buffer
*               Index     used byte in fact
* Return(s)   : 0         exceed interna buffer
*               -2        exceed output buffer
*               > 0       length &&  transform successfully  
*/
#include "stdint.h"

int Int2Str(unsigned int IntNum,char Buff[], uint8_t const BuffSize, uint8_t*Index,  uint8_t MinumdigitNum)
{
	uint8_t IndexTemp = 0;
	uint8_t BuffIndex;
	char NumChar[33];
	
	if (BuffSize > 30)
	{
		return  0;
	}
	if (IntNum == 0)
	{
		NumChar[IndexTemp] =  '0';
		IndexTemp++;
	}
	while (IntNum > 0)
	{
		unsigned int Temp = IntNum / 10;
		NumChar[IndexTemp] = (IntNum - Temp * 10) + '0';
		IntNum = Temp;
		IndexTemp++;
		if (IndexTemp >= BuffSize-1)
		{
			return -2;
		}
	}
	if (MinumdigitNum > IndexTemp)
	{
		uint8_t zerofill = MinumdigitNum - IndexTemp;
		for (; zerofill > 0; zerofill--)
		{
			NumChar[IndexTemp] =  '0';
			IndexTemp++;
			if (IndexTemp >= BuffSize - 1)
			{
				return -2;
			}
		}
	}
	for (BuffIndex = 0;IndexTemp > 0; IndexTemp--, BuffIndex++)
	{
		Buff[BuffIndex] = NumChar[IndexTemp - 1];
	}
	*Index += BuffIndex;
	Buff[BuffIndex] = '\0';
	return BuffIndex;
}

/*
* Description : transform    unsigned int to string
* Argument(s) : value     	 unsigned int var
*               Buff         output buffer
*               BuffSize     size of output buffer
*               DecimalNum   decimal char   needed
* Return(s)   : 0            exceed interna buffer
*               -2           exceed output buffer
*               1            transform successfully  
* Note:         each of the maxium Decimal  number and integers number is nine
*               fabs(value)  must less than 0xffffffff
*/

int MySnprintf(float value, char   Buff[], uint8_t const BuffSize,uint8_t const DecimalNum)
{
	uint8_t BuffIndex = 0;
	int TransState;

	unsigned int IntPart;

	unsigned int DecimalPart;
	uint8_t DecimalNumTemp;


	if (value < 0)
	{
		value = -value;
		Buff[0] = '-';
		BuffIndex++;
	}

	if (value > 0xffffffff)
	{
		return -1;
	}
	IntPart = (unsigned int)(value);
	
	TransState = Int2Str(IntPart, &Buff[BuffIndex], BuffSize - BuffIndex, &BuffIndex,0);
	if (TransState  <= 0)
	{
		return TransState;
	}
	
	value -= IntPart;

	for (DecimalNumTemp = DecimalNum;DecimalNumTemp > 0; DecimalNumTemp--)
	{
		value *= 10;
	}

	if (value > 0xffffffff)
	{
		return -1;
	}
	DecimalPart = (unsigned int)(value);

	if (DecimalNum > 0)
	{
		Buff[BuffIndex] = '.';
		BuffIndex++;
		if (DecimalPart == 0)
		{
			for (DecimalNumTemp = DecimalNum; DecimalNumTemp > 0; DecimalNumTemp--)
			{
				Buff[BuffIndex] = '0';
				BuffIndex++;
				if (BuffIndex >= BuffSize - 2)
				{
					return -2;
				}
			}
			Buff[BuffIndex] = '\0';
		}
		else
		{
			TransState = Int2Str(DecimalPart, &Buff[BuffIndex], BuffSize - BuffIndex, &BuffIndex, DecimalNum);
			if (TransState <= 0)
			{
				return TransState;
			}
		}		
	}
	return TransState;
}

int MySnprintfInt(int value, char   Buff[], uint8_t const BuffSize)
{
	uint8_t BuffIndex = 0;
	if (value < 0)
	{
		value = -value;
		Buff[0] = '-';
		BuffIndex++;
	}
	return Int2Str(value, &Buff[BuffIndex], BuffSize, 0,  0);
}

int MySnprintfFloat(float value, char   Buff[], uint8_t const BuffSize,uint8_t const DecimalNum)
{
	return MySnprintf(value, Buff, BuffSize, DecimalNum);
}

