/**
 * @author [kianliu]
 * @email [linhaigoo2013@gmail.com]
 * @create date 2020-11-11 10:47:53
 * @modify date 2020-11-11 10:47:53
 * @desc [description]
 */
#ifndef __COM_LIB_APP_H__
#define __COM_LIB_APP_H__
#include "comLib.h"

#ifndef QT
#include "usart.h"
extern REC_FRAME_Typedef  recDataFrame;
#endif

bool FrameRecPolling(REC_FRAME_Typedef* pDataBuffer);

bool FrameSend(SEND_FRAME_Typedef* pDataBuffer);
#endif
