#ifndef __ADC_H__
#define __ADC_H__
#include "sys.h" 


//PB1 VOUT_SENSE ADC2ch9
#define VOUT_SENSE_PORT					GPIOB
#define VOUT_SENSE_PIN					GPIO_Pin_1
#define VOUT_SENSE_CLK  				RCC_AHB1Periph_GPIOB
#define VOUT_SENSE_PERIPH    		RCC_APB2Periph_ADC2
#define VOUT_SENSE_CHANNEL      ADC_Channel_9
#define VOUT_SENSE_ADC     	    ADC2

//PC5 VIN_SENSE ADC1ch15
#define VIN_SENSE_PORT				GPIOC
#define VIN_SENSE_PIN					GPIO_Pin_5
#define VIN_SENSE_CLK  				RCC_AHB1Periph_GPIOC
#define VIN_SENSE_PERIPH    	RCC_APB2Periph_ADC1
#define VIN_SENSE_CHANNEL			ADC_Channel_15
#define VIN_SENSE_ADC     	  ADC1

//PA0   INA ADC3ch0
#define INA_PORT				GPIOA
#define INA_PIN					GPIO_Pin_0
#define INA_CLK  				RCC_AHB1Periph_GPIOA
#define INA_PERIPH  		RCC_APB2Periph_ADC3
#define INA_CHANNEL			ADC_Channel_0
#define INA_ADC     	  ADC3

//PC3  ACS ADC3ch13
#define ACS_PORT				GPIOC
#define ACS_PIN					GPIO_Pin_3
#define ACS_CLK  				RCC_AHB1Periph_GPIOC
#define ACS_PERIPH  		RCC_APB2Periph_ADC3
#define ACS_CHANNEL			ADC_Channel_13
#define ACS_ADC         ADC3


typedef enum
{
	VOUT_SENSE = 0,
	VIN_SENSE = 1,
	INA = 2,
	ACS = 3,
	ADC_USEDn = 4,
}ADC_USED_Typedef;
 							   
void MY_ADC_Init(void);
uint16_t  GetAdc(ADC_USED_Typedef ADC_in);
#endif 
