/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-10-19 14:06:32
 * @modify date 2020-10-19 14:06:32
 * @desc [description]
 */
#include "screenCom.h"
#include "usart.h"

#include "stdlib.h"

#include "screenComController.h"

SCREEN_SET_Typedef screenSet;

bool RecValueUpdated[SCREEN_SET_ORDER_LENGTH];
	
void recPackAnalyse(uint8_t * pRecBuffer, uint8_t length)
{
	if (pRecBuffer[0] == 0xB1 && pRecBuffer[1] == 0x11)
	{
		uint32_t value_idx = pRecBuffer[2] << 24 | pRecBuffer[3] << 16 | pRecBuffer[4] << 8 | pRecBuffer[5];
		for(int i = 0; i < ValueIdAddressArrayLength; i++)
		{
			const VALUE_ID_ADDRESS_Typedef *pTemp = &ValueIdAddressArray[i];
			if(value_idx == pTemp->idx)
			{
//				uint8_t ControlType =  pRecBuffer[6];
				switch(pTemp->ValueType)
				{
					case TXT_FLOAT:
						*(float*)(ValueIdAddressArray[i].addr) = atof((char*)&pRecBuffer[7]);
						break;
					case TXT_UINT32_T:
						*(uint32_t*)(ValueIdAddressArray[i].addr) = atoi((char*)&pRecBuffer[7]);
						break;
					case UINT8_T:
						*(uint8_t*)(ValueIdAddressArray[i].addr) = pRecBuffer[8];
						break;
					default:
						break;
				}
				RecValueUpdated[pTemp->Order] = true;
				break;
			}
		}	
	}
}

void screenInfoRec()
{
	#ifdef LCD_COM
	comRecPack();
	#else
	
	#endif
}

uint8_t SendBuffer[UART_SEND_BUFFER_SIZE];
void screenInfoSet(uint32_t value_idx, void* pdata)
{
	#ifdef LCD_COM
	for(int i = 0; i < ValueIdAddressArrayLength; i++)
		{
			const VALUE_ID_ADDRESS_Typedef *pTemp = &ValueIdAddressArray[i];
			if(value_idx == pTemp->idx)
			{
				SendBuffer[0]=0xEE; //frame head
				SendBuffer[1]=0xB1;
				SendBuffer[2]=0x10;
				SendBuffer[3]=	(value_idx >> 24) & 0xff;
				SendBuffer[4]=	(value_idx >> 16) & 0xff;
				SendBuffer[5]=	(value_idx >> 8) & 0xff;
				SendBuffer[6]=	value_idx & 0xff;
				
				uint8_t bufferIdx = 7;		
				switch(pTemp->ValueType)
				{
					case TXT_FLOAT:
						sprintf((char*)&SendBuffer[bufferIdx], "%.1f",*(float*)(pdata));
						while(SendBuffer[bufferIdx++] != 0 && bufferIdx < 0xff);
						break;
					case TXT_UINT32_T:
						sprintf((char*)&SendBuffer[bufferIdx], "%.0f", (float)(*(uint32_t*)(pdata)));
						while(SendBuffer[bufferIdx++] != 0 && bufferIdx < 0xff);
						break;
					case UINT8_T:
						SendBuffer[bufferIdx++] =  *(uint8_t *)pdata;
						break;
					default:
						break;
				}
				SendBuffer[bufferIdx++] = 0xFF;
				SendBuffer[bufferIdx++] = 0xFC;
				SendBuffer[bufferIdx++] = 0xFF;
				SendBuffer[bufferIdx++] = 0xFF;
				ComWrite(SendBuffer, bufferIdx);
				break;
			}
		}	
		#endif
}

void screenCmd(uint16_t cmd)
{
	#ifdef LCD_COM
    SendBuffer[0]=0xEE; //frame head
    SendBuffer[1]=0xB1;
    SendBuffer[2]=0x00;
    SendBuffer[3]=(cmd >> 8) & 0xFF;
    SendBuffer[4]=cmd & 0xFF;
    SendBuffer[5] = 0xFF;
    SendBuffer[6] = 0xFC;
    SendBuffer[7] = 0xFF;
    SendBuffer[8] = 0xFF;
    ComWrite(SendBuffer, 9);
	#endif
}



