#include "USART.h"

void COM_USART_Init(void)
{
	USART_InitTypeDef 	USART_InitStructure;
  NVIC_InitTypeDef  	NVIC_InitStructure;
  GPIO_InitTypeDef  	GPIO_InitStructure;
	
	/* Enable GPIO(AHB1) clock */
  RCC_AHB1PeriphClockCmd(USART_TX_GPIO_CLK | USART_RX_GPIO_CLK, ENABLE);
  
  /* Enable USART clock */
  USART_CLK_INIT(USART_CLK, ENABLE);
  
  /* Connect USART pins to USART_AF */
  GPIO_PinAFConfig(USART_TX_GPIO_PORT, USART_TX_SOURCE, USART_TX_AF);
  GPIO_PinAFConfig(USART_RX_GPIO_PORT, USART_RX_SOURCE, USART_RX_AF);
  
  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 		= 	GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin 		= 	USART_TX_PIN;
  GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin 		= 	USART_RX_PIN;
  GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);

  /* Enable the USART OverSampling by 8 */
  USART_OverSampling8Cmd(USART, ENABLE);
	
	USART_InitStructure.USART_BaudRate 		= 	115200;
  USART_InitStructure.USART_WordLength 	= 	USART_WordLength_8b;
  USART_InitStructure.USART_StopBits 		= 	USART_StopBits_1;
  USART_InitStructure.USART_Parity 			= 	USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode 				= 	USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART, &USART_InitStructure);
	
	USART_ITConfig(USART, USART_IT_RXNE, ENABLE);

	/* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel 										= 	USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 	0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 	0;
  NVIC_InitStructure.NVIC_IRQChannelCmd 								= 	ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	/* Enable USART */
  USART_Cmd(USART, ENABLE);
}

void ComWrite(uint8_t* SendBuf,uint8_t lengthTemp)
{
	uint8_t i = 0;
	for(i = 0;i < lengthTemp;i++)
	{
		while(USART_GetFlagStatus(USART,USART_FLAG_TXE)!= SET);
		USART_SendData(USART,*SendBuf++);
	}
}

void (USART_IRQHandler)(void)
{
	uint16_t temp = 0;

	if(USART_GetITStatus(USART, USART_IT_RXNE) == SET)
  {
    temp = USART_ReceiveData(USART);

  }	
}



