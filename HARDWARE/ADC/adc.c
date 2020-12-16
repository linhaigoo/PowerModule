#include "adc.h"

GPIO_TypeDef* ADC_PORT[ADC_USEDn] = {VOUT_SENSE_PORT, VIN_SENSE_PORT,	INA_PORT,	ACS_PORT};
uint16_t ADC_PIN[ADC_USEDn] = {VOUT_SENSE_PIN, VIN_SENSE_PIN,	INA_PIN,	ACS_PIN};
const uint32_t ADC_PORT_CLK[ADC_USEDn] = {VOUT_SENSE_CLK, VIN_SENSE_CLK, INA_CLK, ACS_CLK};
const uint32_t ADC_PERIPH[ADC_USEDn] = {VOUT_SENSE_PERIPH, VIN_SENSE_PERIPH, INA_PERIPH, ACS_PERIPH};
const uint8_t ADC_CHANNEL[ADC_USEDn] ={VOUT_SENSE_CHANNEL,VIN_SENSE_CHANNEL,INA_CHANNEL,ACS_CHANNEL};
ADC_TypeDef * ADC_INSTANCE_USED[ADC_USEDn] = {VOUT_SENSE_ADC,VIN_SENSE_ADC,INA_ADC,ACS_ADC};

void  MY_ADC_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	for(int i = 0; i < ADC_USEDn; i++)
	{
		RCC_AHB1PeriphClockCmd(ADC_PORT_CLK[i] , ENABLE);
		RCC_APB2PeriphClockCmd(ADC_PERIPH[i], ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = ADC_PIN[i];
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(ADC_PORT[i], &GPIO_InitStructure);
		
		RCC_APB2PeriphResetCmd(ADC_PERIPH[i],ENABLE);
		RCC_APB2PeriphResetCmd(ADC_PERIPH[i],DISABLE);
	}
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	for(int i = 0; i < ADC_USEDn; i++)
	{
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC_Init(ADC_INSTANCE_USED[i], &ADC_InitStructure);
		ADC_Cmd(ADC_INSTANCE_USED[i], ENABLE);
	}
}


uint16_t GetAdc(ADC_USED_Typedef ADC_in)   
{
	if(ADC_in >= ADC_USEDn || ADC_in < 0)return 0;
	ADC_RegularChannelConfig(ADC_INSTANCE_USED[ADC_in], ADC_CHANNEL[ADC_in], 1, ADC_SampleTime_56Cycles );

	ADC_SoftwareStartConv(ADC_INSTANCE_USED[ADC_in]);
	while(!ADC_GetFlagStatus(ADC_INSTANCE_USED[ADC_in], ADC_FLAG_EOC ));
//	return ADC_GetConversionValue(ADC_INSTANCE_USED[ADC_in]) & 0xfff;  //todo check
	return ADC_GetConversionValue(ADC_INSTANCE_USED[ADC_in]);  //todo check
}
