#include "USART.h"
#include "stdbool.h"


void screenComInit(void)
{
	COM_USART_Init(LCD_COM);
}

void ScreenComWrite(uint8_t* SendBuf,uint8_t lengthTemp)
{
	uint8_t i = 0;
	for(i = 0;i < lengthTemp;i++)
	{
		while(USART_GetFlagStatus(LCD_USART,USART_FLAG_TXE)!= SET);
		USART_SendData(LCD_USART,*SendBuf++);
	}
}


typedef enum
{
	REC_HEAD_WAIT,
	REC_TAIL0_WAIT,
	REC_TAIL1_WAIT,
	REC_TAIL2_WAIT,
	REC_COMP,
}USART_REC_STATE_Typedef;

typedef struct 
{
	uint8_t recBuffer[UART_REC_BUFFER_SIZE];
	volatile USART_REC_STATE_Typedef recState;
	uint8_t data_length;
	volatile bool rec_using;
}UART_RecManagerTypedef;


UART_RecManagerTypedef UART_RecManager[UART_REC_BUFFER_QUEUE_SIZE];

#define FRAME_HEAD  0xEE
#define FRAME_TAIL0 0xFF
#define FRAME_TAIL1 0xFC
#define FRAME_TAIL2 0xFF

uint8_t WARN_uart_rec_comp_not_used = 0;
uint8_t WARN_uart_rec_risk = 0;


void (LCD_USART_IRQHandler)(void)
{
  uint16_t rec;
	static uint8_t RecManagerIdx = 0;
	UART_RecManagerTypedef* pRecManager =  &UART_RecManager[RecManagerIdx];
  
  if(USART_GetITStatus(LCD_USART, USART_IT_RXNE) == SET)
  { 
    rec = USART_ReceiveData(LCD_USART);	
		if(rec == FRAME_HEAD)
		{
			if(pRecManager->recState == REC_COMP)
			{
				WARN_uart_rec_comp_not_used = 1;
			}
			pRecManager->recState=REC_TAIL0_WAIT;
			pRecManager->data_length = 0;
		}
		else if(pRecManager->recState == REC_TAIL0_WAIT)
		{
			if(rec == FRAME_TAIL0)
			{
				pRecManager->recState=REC_TAIL1_WAIT;
			}
			else
			{
				pRecManager->recBuffer[pRecManager->data_length] = rec;
				pRecManager->data_length++;
			} 
		}
		else if(pRecManager->recState == REC_TAIL1_WAIT && rec == FRAME_TAIL1)
		{
			pRecManager->recState=REC_TAIL2_WAIT;
		}
		else if(pRecManager->recState == REC_TAIL2_WAIT && rec == FRAME_TAIL2)
		{
			pRecManager->recState=REC_COMP;
			RecManagerIdx = (RecManagerIdx + 1) % UART_REC_BUFFER_QUEUE_SIZE;
		}			
	}
}


uint32_t WARN_uart_rec_data_unready = 0;
uint32_t data_ready_static = 0;

__weak void recPackAnalyse(uint8_t * pRecBuffer, uint8_t length)
{
	
}



//mutex for thread safe
volatile static uint8_t gRecManagerIdx = 0;
void comRecPack(void)
{
  UART_RecManagerTypedef * pRecManager =  &UART_RecManager[gRecManagerIdx];

	if(pRecManager->recState == REC_COMP)
	{
		recPackAnalyse(pRecManager->recBuffer,pRecManager->data_length);
		if(pRecManager->recState != REC_COMP)
		{
			WARN_uart_rec_risk++;
		}
		pRecManager->recState = REC_HEAD_WAIT;
		
		data_ready_static++;
		
		gRecManagerIdx = (gRecManagerIdx + 1) % UART_REC_BUFFER_QUEUE_SIZE;	
	}
	else
	{
		WARN_uart_rec_data_unready++;
	}
	pRecManager->rec_using = false;	
}



