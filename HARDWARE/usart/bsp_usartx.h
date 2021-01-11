#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdbool.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
//#define _RS485_
//#define _RS232_
#define _USART_

#ifdef _USART_

  #define USARTx                                 USART1
  #define USARTx_BAUDRATE                        115200
  #define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
  #define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

  #define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
  #define USARTx_Tx_GPIO_PIN                     GPIO_PIN_9
  #define USARTx_Tx_GPIO                         GPIOA
  #define USARTx_Rx_GPIO_PIN                     GPIO_PIN_10   
  #define USARTx_Rx_GPIO                         GPIOA

  //#define USARTx_AFx                             GPIO_AF7_USART1

  #define USARTx_IRQHANDLER                      USART1_IRQHandler
  #define USARTx_IRQn                            USART1_IRQn
#else 
  #define USARTx                                 USART3
  #define USARTx_BAUDRATE                        115200
  #define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART3_CLK_ENABLE()
  #define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART3_CLK_DISABLE()

  #define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
  #define USARTx_Tx_GPIO_PIN                     GPIO_PIN_10
  #define USARTx_Tx_GPIO                         GPIOB
  #define USARTx_Rx_GPIO_PIN                     GPIO_PIN_11
  #define USARTx_Rx_GPIO                         GPIOB

  #define USARTx_AFx                             GPIO_AF7_USART3
  
  #define USARTx_IRQHANDLER                      USART3_IRQHandler
  #define USARTx_IRQn                            USART3_IRQn
  
  #ifdef _RS485_  
    #define RS485_REDE_GPIO_ClK_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()
    #define RS485_REDE_PORT                              GPIOC
    #define RS485_REDE_PIN                               GPIO_PIN_12
    #define RS485_RX_MODE()                              HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_RESET)
    #define RS485_TX_MODE()                              HAL_GPIO_WritePin(RS485_REDE_PORT,RS485_REDE_PIN,GPIO_PIN_SET)
  #endif

#endif
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define USARTx                                 USART1
#define USARTx_BAUDRATE                        115200
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()

#define USARTx_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_PORT                            GPIOA
#define USARTx_Tx_PIN                          GPIO_PIN_9
#define USARTx_Rx_PIN                          GPIO_PIN_10

#define USARTx_IRQHANDLER                      USART1_IRQHandler
#define USARTx_IRQn                            USART1_IRQn

#define UART_REC_BUFFER_SIZE                   0xff
#define UART_REC_BUFFER_QUEUE_SIZE             8
#define UART_SEND_BUFFER_SIZE                  0xff

/* 函数声明 ------------------------------------------------------------------*/
void USARTxInit(void);
void comRecPack(void);
void comSendPack(uint8_t * pSendBuffer, uint8_t length);
extern uint8_t SendBuffer[UART_SEND_BUFFER_SIZE];
extern UART_HandleTypeDef hUARTx;

//todo board tobe redesigned
#define LCD_COM
//#define DEBUG_COM

#ifdef DEBUG_COM
#include "comLib.h"
#endif

#endif  /* __BSP_USARTX_H__ */

