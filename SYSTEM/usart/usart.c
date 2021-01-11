#include "USART.h"
#include "stdbool.h"
#define DEBUG_COM

#ifdef DEBUG_COM
#include "comLib.h"
#endif

USART_TypeDef * USARTX[COM_NUM]={USART, LCD_USART};
const uint32_t USARTX_CLK[COM_NUM]={USART_CLK, LCD_USART_CLK};
void (*USARTX_CLK_CMD[COM_NUM])(uint32_t RCC_APB2Periph, FunctionalState NewState) = {USART_CLK_CMD, LCD_USART_CLK_CMD};
const IRQn_Type USARTX_IRQ[COM_NUM] = {USART_IRQn, LCD_USART_IRQn};

const uint16_t USARTX_TX_PIN[COM_NUM] = {USART_TX_PIN, LCD_USART_TX_PIN};
GPIO_TypeDef * USARTX_TX_PORT[COM_NUM] = {USART_TX_GPIO_PORT, LCD_USART_TX_GPIO_PORT};
const uint32_t USARTX_TX_CLK[COM_NUM] = {USART_TX_GPIO_CLK, LCD_USART_TX_GPIO_CLK};
const uint8_t USARTX_TX_SOURCE[COM_NUM] = {USART_TX_SOURCE, LCD_USART_TX_SOURCE};
const uint8_t USARTX_TX_AF[COM_NUM] = {USART_TX_AF, LCD_USART_TX_AF};

const uint16_t USARTX_RX_PIN[COM_NUM] = {USART_RX_PIN, LCD_USART_RX_PIN};
GPIO_TypeDef * USARTX_RX_PORT[COM_NUM] = {USART_RX_GPIO_PORT, LCD_USART_RX_GPIO_PORT};
const uint32_t USARTX_RX_CLK[COM_NUM] = {USART_RX_GPIO_CLK, LCD_USART_RX_GPIO_CLK};
const uint8_t USARTX_RX_SOURCE[COM_NUM] = {USART_RX_SOURCE, LCD_USART_RX_SOURCE};
const uint8_t USARTX_RX_AF[COM_NUM] = {USART_RX_AF, LCD_USART_RX_AF};


void COM_USART_Init(COM_Typedef COM_ID)
{
	USART_InitTypeDef 	USART_InitStructure;
  NVIC_InitTypeDef  	NVIC_InitStructure;
  GPIO_InitTypeDef  	GPIO_InitStructure;
	
	/* Enable GPIO(AHB1) clock */
  RCC_AHB1PeriphClockCmd(USARTX_TX_CLK[COM_ID] | USARTX_RX_CLK[COM_ID], ENABLE);
  
  /* Enable USART clock */
  (USARTX_CLK_CMD[COM_ID])(USARTX_CLK[COM_ID], ENABLE);
  
  /* Connect USART pins to USART_AF */
  GPIO_PinAFConfig(USARTX_TX_PORT[COM_ID], USARTX_TX_SOURCE[COM_ID], USARTX_TX_AF[COM_ID]);
  GPIO_PinAFConfig(USARTX_RX_PORT[COM_ID], USARTX_RX_SOURCE[COM_ID], USARTX_RX_AF[COM_ID]);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin 		= 	USARTX_TX_PIN[COM_ID];
  GPIO_Init(USARTX_TX_PORT[COM_ID], &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin 		= 	USARTX_RX_PIN[COM_ID];
  GPIO_Init(USARTX_RX_PORT[COM_ID], &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USARTX[COM_ID], ENABLE);
	
	USART_InitStructure.USART_BaudRate 		= 	57600;
  USART_InitStructure.USART_WordLength 	= 	USART_WordLength_8b;
  USART_InitStructure.USART_StopBits 		= 	USART_StopBits_1;
  USART_InitStructure.USART_Parity 			= 	USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode 				= 	USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTX[COM_ID], &USART_InitStructure);
	
	USART_ITConfig(USARTX[COM_ID], USART_IT_RXNE, ENABLE);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel 										= 	USARTX_IRQ[COM_ID];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 	2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 	0;
  NVIC_InitStructure.NVIC_IRQChannelCmd 								= 	ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	/* Enable USART */
  USART_Cmd(USARTX[COM_ID], ENABLE);
}

void ComWrite(uint8_t* SendBuf,uint8_t lengthTemp)
{
	uint8_t i = 0;
	for(i = 0;i < lengthTemp;i++)
	{
		while(USART_GetFlagStatus(USART,USART_FLAG_TXE)!= SET);
		USART_SendData(USART,*SendBuf++);
	}
	while(USART_GetFlagStatus(USART,USART_FLAG_TXE)!= SET);
	USART_SendData(USART,0xAA);
	while(USART_GetFlagStatus(USART,USART_FLAG_TXE)!= SET);
}

REC_MANAGE_Typedef recManage;


void (USART_IRQHandler)(void)
{
  if(USART_GetITStatus(USART, USART_IT_RXNE) == SET)
  { 
    uint16_t rec = USART_ReceiveData(USART);	
		recQueuePush(rec, &recManage.queue);
	}
}
