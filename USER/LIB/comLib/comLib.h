/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-11-11 10:09:50
 * @modify date 2020-11-11 10:09:50
 * @desc [description]
 */

#ifndef COMLIB_H
#define COMLIB_H

#include "stdint.h"
#include "stdbool.h"


#define UART_REC_DATA_LENGTH_MAX  256
#define UART_SEND_BUFFER_LENGTH_MAX  512
#define REC_QUEUE_SIZE 512

typedef struct
{
    uint8_t recQueue[REC_QUEUE_SIZE];
    uint16_t head;
    uint16_t tail;
}REC_QUEUE_Typedef;

#pragma pack(4)
typedef struct
{
    uint8_t dataLength;
    uint8_t cmdProperty;
    uint8_t data[UART_REC_DATA_LENGTH_MAX];
}REC_FRAME_Typedef;
#pragma pack()



typedef struct
{
    uint8_t recState;
    REC_QUEUE_Typedef queue;
    bool escapeByteDetected;
    uint8_t dataIdx;
}REC_MANAGE_Typedef;


typedef struct
{
    uint8_t dataLength;
    uint8_t cmdProperty;
    uint8_t* data;
}SEND_FRAME_Typedef;

typedef struct
{
    uint8_t rawPackBuffer[UART_SEND_BUFFER_LENGTH_MAX];
    uint16_t bufferLength;
}SEND_MANAGE_Typedef;


void recQueuePush(uint8_t uart_rec_byte, REC_QUEUE_Typedef *pQueue);

bool rawSendPacking(SEND_MANAGE_Typedef *pManage, SEND_FRAME_Typedef* pDataBuffer);
bool rawRecPacking(REC_MANAGE_Typedef *pManage, REC_FRAME_Typedef* pDataBuffer);
//serial.write((char*)sendManage.rawPackBuffer, sendManage.bufferLength);
#endif // COMLIB_H
