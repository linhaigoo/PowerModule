// Author:
// Date: 
// File brief:

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define USART                           USART1                            
#define USART_CLK                       RCC_APB2Periph_USART1
#define USART_CLK_INIT                  RCC_APB2PeriphClockCmd
#define USART_IRQn                      USART1_IRQn
#define USART_IRQHandler                USART1_IRQHandler

#define USART_TX_PIN                    GPIO_Pin_6                
#define USART_TX_GPIO_PORT              GPIOB                       
#define USART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define USART_TX_SOURCE                 GPIO_PinSource6
#define USART_TX_AF                     GPIO_AF_USART1

#define USART_RX_PIN                    GPIO_Pin_7                
#define USART_RX_GPIO_PORT              GPIOB                   
#define USART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define USART_RX_SOURCE                 GPIO_PinSource7
#define USART_RX_AF                     GPIO_AF_USART1

/* Exported functions --------------------------------------------------------*/
void COM_USART_Init(void);
void ComWrite(uint8_t* SendBuf,uint8_t lengthTemp);

#endif 

/******************* (C) COPYRIGHT 2014 HUST-Robocon *****END OF FILE**********/


