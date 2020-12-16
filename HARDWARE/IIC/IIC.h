#ifndef __IIC_H__
#define __IIC_H__
#include "sys.h"

//BSP Interface
//IIC
//#define HARD_IIC

#define IIC_USING		 						I2C2
#define IIC_MODE     						I2C_Mode_I2C 

#define IIC_BSP_PERIPH					RCC_APB1Periph_I2C2 
#define IIC_BSP_AF   						GPIO_AF_I2C2


#define IIC_BSP_CLK  						RCC_AHB1Periph_GPIOB
//SCL  PB10
#define IIC_SCL_PORT GPIOB
#define IIC_SCL_PIN  GPIO_Pin_10
#define IIC_SCL_SRC  GPIO_PinSource10
//SDA PB11
#define IIC_SDA_PORT GPIOB
#define IIC_SDA_PIN  GPIO_Pin_11
#define IIC_SDA_SRC  GPIO_PinSource11

#define IIC_SCL PBout(10)
#define IIC_SDA PBout(11)
#define READ_SDA PBin(11)

#define SDA_IN()  {GPIOB->MODER&=~(3<<(2*11));GPIOB->MODER|=GPIO_Mode_IN<<2*11;}
#define SDA_OUT() {GPIOB->MODER&=~(3<<(2*11));GPIOB->MODER|=GPIO_Mode_OUT<<2*11;}


//#define IIC_BSP_CLK  						RCC_AHB1Periph_GPIOH
////SCL  PH4
//#define IIC_SCL_PORT GPIOH
//#define IIC_SCL_PIN  GPIO_Pin_4
//#define IIC_SCL_SRC  GPIO_PinSource4
////SDA PH5
//#define IIC_SDA_PORT GPIOH
//#define IIC_SDA_PIN  GPIO_Pin_5
//#define IIC_SDA_SRC  GPIO_PinSource5


//#define IIC_SCL PHout(4)
//#define IIC_SDA PHout(5)
//#define READ_SDA PHin(5)

//#define SDA_IN()  {GPIOH->MODER&=~(3<<(2*5));GPIOH->MODER|=GPIO_Mode_IN<<2*5;}
//#define SDA_OUT() {GPIOH->MODER&=~(3<<(2*5));GPIOH->MODER|=GPIO_Mode_OUT<<2*5;}

#define IIC_ADDR        				0x01
#define IIC_DMA_ID         			1
#define IIC_DMA         				DMA1
#define IIC_DMA_PERIPH     			RCC_AHB1Periph_DMA1
#define IIC_TX_DMA_STREAM_ID  	7
#define IIC_TX_DMA_STREAM      	DMA1_Stream7
#define IIC_TX_DMA_CHANNEL  		DMA_Channel_7

#define IIC_RX_DMA_STREAM_ID  	2
#define IIC_RX_DMA_STREAM      	DMA1_Stream2
#define IIC_RX_DMA_CHANNEL  	 	DMA_Channel_7
//IRQ
#define IIC_TX_DMA_IRQ 				  DMA1_Stream7_IRQn
#define IIC_TX_IRQ_HANDLER 			DMA1_Stream7_IRQHandler
#define IIC_TX_FLAG             DMA_FLAG_TCIF7
#define IIC_TX_ERR_FLAG         DMA_IT_TEIF7

#define IIC_RX_DMA_IRQ 				  DMA1_Stream2_IRQn
#define IIC_RX_IRQ_HANDLER 			DMA1_Stream2_IRQHandler



void IIC_BSP_Init(void);
void stopIIC_DMA_TX(void);
u32 I2C_WriteReg(u8 SlaveAddr, u8 WriteAddr,const u8 data);
u32 I2C_DMA_Write(u8 SlaveAddr, u8 WriteAddr, const u8 * pData, u8 len);
#endif


