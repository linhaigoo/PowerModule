#include "task_init.h"
#include "usart.h"
#include "PowerModule.h"
#include "screenComponent.h"
#include "InteractKeyScreen.h"
#include "comLibApp.h"
#include "gpio.h"

/******全局状态变量声明********/

/***************************/

#define RS_485_FRAME_PROPERTY 0x68 
#define RS_CTRL_READ()       (RS_CTRL=0)
#define RS_CTRL_WRITE()       (RS_CTRL=1)

typedef enum 
{
	SRC_DEVICE_ID = 0,
	DES_DEVICE_ID = 1,
	VAR_ID = 2,
	VALUE = 3,
}RS485_PROC_Typedef;

bool trig_request = false;;

REC_FRAME_Typedef rec_frame;
SEND_FRAME_Typedef send_frame;

void TASK_UART(void*pdata)
{
	OS_ERR err;

	RS_CTRL_READ();
	while(1)
	{
		if(trig_request)
		{
			trig_request = false;
			if(PM_module.ID == 0x00)
			{
				uint8_t data[6];
				send_frame.cmdProperty = RS_485_FRAME_PROPERTY;
				send_frame.dataLength = 4;
				data[SRC_DEVICE_ID]	= PM_module.ID;
				data[DES_DEVICE_ID]	= 0xff;	
				data[VAR_ID]	= COMPONENT_NUM;
				data[VALUE]	= PM_module.isWorking == NORM;
				send_frame.data = data;
				RS_CTRL_WRITE();
				FrameSend(&send_frame);
				RS_CTRL_READ();
			}
		}
		if(FrameRecPolling(&rec_frame))
		{
			if(rec_frame.cmdProperty == RS_485_FRAME_PROPERTY && rec_frame.dataLength >= 4 && rec_frame.data[SRC_DEVICE_ID] == 0x00)
			{
				if(rec_frame.data[DES_DEVICE_ID] == PM_module.ID || rec_frame.data[DES_DEVICE_ID] == 0xff)
				{
					uint8_t value_id = rec_frame.data[VAR_ID];
					if(value_id == COMPONENT_NUM)
					{
						PM_module.isWorking = rec_frame.data[VALUE];
					}
					else if(value_id < COMPONENT_NUM)
					{
						varChange(value_id, &rec_frame.data[VALUE], &PM_module);
					}
				}
			}
		}		
		OSTimeDlyHMSM(0, 0, 0, 10, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}
