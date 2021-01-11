/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-11-11 10:39:03
 * @modify date 2020-11-11 10:39:03
 * @desc [description]
 */
#include "comLibApp.h"


#ifndef QT
static SEND_MANAGE_Typedef sendManage;
extern REC_MANAGE_Typedef recManage;
REC_FRAME_Typedef  recDataFrame;
#endif

bool FrameRecPolling(REC_FRAME_Typedef* pRecDataFrame)
{
    return rawRecPacking(&recManage, pRecDataFrame);
}

bool FrameSend(SEND_FRAME_Typedef* pSendDataFrame)
{
    if(rawSendPacking(&sendManage, pSendDataFrame))
    {
        #ifdef QT
        serial.write((char*)sendManage.rawPackBuffer, sendManage.bufferLength);
        #else
        ComWrite((uint8_t*)sendManage.rawPackBuffer, sendManage.bufferLength);
	    //DMA todo
        #endif
			return true;
    }
		return false;
}

