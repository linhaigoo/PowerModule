#include "dac.h"
void MY_DAC_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(DAC_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	   
  GPIO_InitStructure.GPIO_Pin = DAC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(DAC_PORT, &GPIO_InitStructure);

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable;
  DAC_Init(DAC_CHANNEL,&DAC_InitType);

	DAC_Cmd(DAC_CHANNEL, ENABLE); 
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

bool DacSetVol(float vol)
{
	bool flag = true;
	if(vol >3.29f)
	{
		vol = 3.29f;
		flag = false;
	}
	if(vol <0.0f)
	{
		vol = 0.0f;
		flag = false;
	}
	uint16_t data = vol / 3.3f * 4096;
	DAC_SetChannel1Data(DAC_Align_12b_R, data);
	return flag;
}