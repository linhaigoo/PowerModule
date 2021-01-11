/**
  ******************************************************************************
  * 文件名程: bsp_usartx.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: 板载串口底层驱动程序
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "usart/bsp_usartx.h"
#include "string.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/

UART_HandleTypeDef hUARTx;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: hUART：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspInit(UART_HandleTypeDef* phUART)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(phUART == &hUARTx)
  {
    /* 使能串口功能引脚GPIO时钟 */
		USART_RCC_CLK_ENABLE();
    USARTx_GPIO_ClK_ENABLE();
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USARTx_Tx_GPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USARTx_Rx_GPIO, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 串口硬件反初始化配置
  * 输入参数: hUART：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* phUART)
{
  if(phUART == &hUARTx)
  {
    USART_RCC_CLK_DISABLE();
		
    HAL_GPIO_DeInit(USARTx_Tx_GPIO, USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(USARTx_Rx_GPIO, USARTx_Rx_GPIO_PIN);

    HAL_NVIC_DisableIRQ(USARTx_IRQn);
  }
}


/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void USARTx_Config(UART_HandleTypeDef* phUART)
{
	
  /* 串口外设时钟使能 */
  USART_RCC_CLK_ENABLE();
  
  phUART->Instance = USARTx;
  phUART->Init.BaudRate = USARTx_BAUDRATE;
  phUART->Init.WordLength = UART_WORDLENGTH_8B;
  phUART->Init.StopBits = UART_STOPBITS_1;
  phUART->Init.Parity = UART_PARITY_NONE;
  phUART->Init.Mode = UART_MODE_TX_RX;
  phUART->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  phUART->Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(phUART);
  
	HAL_NVIC_SetPriority(USARTx_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
  
  __HAL_UART_CLEAR_FLAG(phUART, UART_FLAG_RXNE);
//  __HAL_UART_DISABLE_IT(phUART, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(phUART, UART_IT_RXNE);
}

void USARTxInit(void)
{
	HAL_UART_MspInit(&hUARTx);
	USARTx_Config(&hUARTx);
}


#ifdef LCD_COM

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

extern UART_RecManagerTypedef UART_RecManager[UART_REC_BUFFER_QUEUE_SIZE];

UART_RecManagerTypedef UART_RecManager[UART_REC_BUFFER_QUEUE_SIZE];

#define FRAME_HEAD  0xEE
#define FRAME_TAIL0 0xFF
#define FRAME_TAIL1 0xFC
#define FRAME_TAIL2 0xFF

uint8_t WARN_uart_rec_comp_not_used = 0;
uint8_t WARN_uart_rec_risk = 0;

#else

REC_MANAGE_Typedef recManage;

#endif

//unsafe : todo rewrite
void USARTx_IRQHANDLER(void)
{
	#ifdef LCD_COM
  uint8_t rec;
	static uint8_t RecManagerIdx = 0;
	UART_RecManagerTypedef* pRecManager =  &UART_RecManager[RecManagerIdx];
  
  if(__HAL_UART_GET_FLAG(&hUARTx, UART_FLAG_RXNE) != RESET)
  { 
    rec = USARTx->DR;	
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
	
	#else
	if(__HAL_UART_GET_FLAG(&hUARTx, UART_FLAG_RXNE) != RESET)
  { 
    uint8_t rec=USARTx->DR;	
		recQueuePush(rec, &recManage.queue);
	}
	
	#endif
	
}

#ifdef LCD_COM

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
		uint8_t recBuffer[UART_REC_BUFFER_SIZE];
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

uint8_t SendBuffer[UART_SEND_BUFFER_SIZE];
void comSendPack(uint8_t * pSendBuffer, uint8_t length)
{
	HAL_UART_Transmit(&hUARTx, pSendBuffer, length, 1200);
	//DMA todo
}

#endif


/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
#ifdef _RS485_
  RS485_TX_MODE();
#endif 
  HAL_UART_Transmit(&hUARTx, (uint8_t *)&ch, 1, 0xffff);

  return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
#ifdef _RS485_
  RS485_RX_MODE();
#endif 
  while(HAL_UART_Receive(&hUARTx,&ch, 1, 0xffff)!=HAL_OK);
  return ch;
}
