/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-11-09 08:43:09
 * @modify date 2020-11-09 08:43:09
 * @desc [description]
 */

#include "comLib.h"

#define FRAME_HEAD_TAIL  0x7E
#define FRAME_ESCAPE 0x7D
#define FRAME_ESCAPE_INV_BIT 0x20

typedef enum
{
   REC_HEAD_WAIT = 0,
   REC_LENGTH_WAIT = 1,
   REC_PROPERTY_WAIT = 2,
   REC_TAIL_WAIT = 3,
}REC_STATE_Typedef;

__inline bool recQueueIsEmpty(REC_QUEUE_Typedef *pQueue)
{
    return (pQueue->tail == pQueue->head);
}

void recQueuePush(uint8_t uart_rec_byte, REC_QUEUE_Typedef *pQueue)
{
    pQueue->recQueue[(pQueue->head++) % REC_QUEUE_SIZE] = uart_rec_byte;
}

__inline bool recQueuePop(uint8_t *pRec_byte, REC_QUEUE_Typedef *pQueue)
{
    if(!recQueueIsEmpty(pQueue))
    {
        *pRec_byte = pQueue->recQueue[(pQueue->tail++) % REC_QUEUE_SIZE];
        return true;
    }
    return false;
}

uint8_t ERR_RAW_PACKING_ANALYSE_Count = 0;
uint8_t ERR_RAW_PACKING_LENGTH_Count = 0;
uint8_t WARN_RAW_PACKING_ESCAPE_Count = 0;

bool rawRecPacking(REC_MANAGE_Typedef *pManage, REC_FRAME_Typedef* pDataBuffer)
{
    uint8_t recByte;
    while(recQueuePop(&recByte, &pManage->queue))
    {
        if(recByte == FRAME_HEAD_TAIL)
        {
            if(pManage->recState == REC_TAIL_WAIT)
            {
                if(pManage->dataIdx == pDataBuffer->dataLength)
                {
                    pManage->recState = REC_HEAD_WAIT;
                    return true;
                }
            }
            pManage->recState = REC_LENGTH_WAIT;
            pManage->dataIdx = 0;
            pManage->escapeByteDetected = false; //redudant
            continue;
        }
        else if(recByte == FRAME_ESCAPE && pManage->recState > REC_HEAD_WAIT)
        {
            pManage->escapeByteDetected = true;
            continue;
        }

        if(pManage->escapeByteDetected)
        {
            recByte ^= FRAME_ESCAPE_INV_BIT;
            pManage->escapeByteDetected = false;
            if(recByte!=(FRAME_ESCAPE) || recByte!=(FRAME_HEAD_TAIL))
            {
                WARN_RAW_PACKING_ESCAPE_Count++;
            }
        }

        switch(pManage->recState)
        {
            case REC_LENGTH_WAIT:
                pDataBuffer->dataLength = recByte;
                pManage->recState = REC_PROPERTY_WAIT;
                break;
            case REC_PROPERTY_WAIT:
                pDataBuffer->cmdProperty = recByte;
                pManage->recState = REC_TAIL_WAIT;
                break;
            case REC_TAIL_WAIT:
                if(pManage->dataIdx < pDataBuffer->dataLength)
                {
                    pDataBuffer->data[pManage->dataIdx++] = recByte;
                }
                else
                {
                    pManage->recState = REC_HEAD_WAIT;
                    ERR_RAW_PACKING_LENGTH_Count++;
                }
                break;
            default:
                ERR_RAW_PACKING_ANALYSE_Count++;
                break;
        }
    }
    return false;
}



__forceinline void sendBufferFill(uint8_t data, uint8_t ** ppBuffer)
{
    if((data) == FRAME_HEAD_TAIL || (data)== FRAME_ESCAPE)
    {
        *((*ppBuffer)++)= FRAME_ESCAPE;
        (data) ^= FRAME_ESCAPE_INV_BIT;
    }
    *((*ppBuffer)++) = (data);
}




bool rawSendPacking(SEND_MANAGE_Typedef *pManage, SEND_FRAME_Typedef* pDataBuffer)
{
    uint8_t *pBuffer = pManage->rawPackBuffer;
    *pBuffer++ = FRAME_HEAD_TAIL;
    sendBufferFill(pDataBuffer->dataLength,  &pBuffer);
    sendBufferFill(pDataBuffer->cmdProperty, &pBuffer);
    for(int i = 0; i< pDataBuffer->dataLength; i++)
    {
        sendBufferFill(pDataBuffer->data[i],&pBuffer);
    }
    *pBuffer++ = FRAME_HEAD_TAIL;
    pManage->bufferLength = (uint16_t)(pBuffer - pManage->rawPackBuffer);
    return true;
}
