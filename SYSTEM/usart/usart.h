// Author:
// Date: 
// File brief:

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"

typedef enum
{
	BOARD_COM = 0,
	LCD_COM = 1,
	COM_NUM = 2,
}COM_Typedef;

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
//bsp com
#define USART                           USART1                            
#define USART_CLK                       RCC_APB2Periph_USART1
#define USART_CLK_CMD                   RCC_APB2PeriphClockCmd
#define USART_IRQn                      USART1_IRQn
#define USART_IRQHandler                USART1_IRQHandler

//PA9
#define USART_TX_PIN                    GPIO_Pin_9                
#define USART_TX_GPIO_PORT              GPIOA                       
#define USART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USART_TX_SOURCE                 GPIO_PinSource9
#define USART_TX_AF                     GPIO_AF_USART1
//PA10
#define USART_RX_PIN                    GPIO_Pin_10                
#define USART_RX_GPIO_PORT              GPIOA                   
#define USART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOA
#define USART_RX_SOURCE                 GPIO_PinSource10
#define USART_RX_AF                     GPIO_AF_USART1

#define UART_REC_BUFFER_SIZE                   0xff
#define UART_REC_BUFFER_QUEUE_SIZE             8
#define UART_SEND_BUFFER_SIZE                  0xff

//lcd com
#define LCD_USART                           USART6                            
#define LCD_USART_CLK                       RCC_APB2Periph_USART6
#define LCD_USART_CLK_CMD                   RCC_APB2PeriphClockCmd
#define LCD_USART_IRQn                      USART6_IRQn
#define LCD_USART_IRQHandler                USART6_IRQHandler

//PC6
#define LCD_USART_TX_PIN                    GPIO_Pin_6                
#define LCD_USART_TX_GPIO_PORT              GPIOC
#define LCD_USART_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define LCD_USART_TX_SOURCE                 GPIO_PinSource6
#define LCD_USART_TX_AF                     GPIO_AF_USART6

//PC7
#define LCD_USART_RX_PIN                    GPIO_Pin_7                
#define LCD_USART_RX_GPIO_PORT              GPIOC                   
#define LCD_USART_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define LCD_USART_RX_SOURCE                 GPIO_PinSource7
#define LCD_USART_RX_AF                     GPIO_AF_USART6

#define UART_REC_BUFFER_SIZE                   0xff
#define UART_REC_BUFFER_QUEUE_SIZE             8
#define UART_SEND_BUFFER_SIZE                  0xff



/* Exported functions --------------------------------------------------------*/
void COM_USART_Init(COM_Typedef);
void ComWrite(uint8_t* SendBuf,uint8_t lengthTemp);




#endif 


