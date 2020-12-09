#include "IIC.h"
#include "stdbool.h"

#define UNUSED(x) (void)(x)

static bool Is_IIC_TX_Stop = true;
static bool Is_IIC_DMA_TX_Stop = true;

void IIC_PORT_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	#ifdef HARD_IIC
	I2C_InitTypeDef I2C_InitStructure;
	
	I2C_DeInit(IIC_USING);	
	#endif
	
	//CLK
	RCC_AHB1PeriphClockCmd(IIC_BSP_CLK, ENABLE);
	
	#ifdef HARD_IIC
	RCC_APB1PeriphClockCmd(IIC_BSP_PERIPH, ENABLE);	
	#endif

  //IO Common Config
	#ifdef HARD_IIC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	#else
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	#endif
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//SCL
	#ifdef HARD_IIC
	GPIO_PinAFConfig(IIC_SCL_PORT, IIC_SCL_SRC, IIC_BSP_AF); 
	#endif

	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);
	
	//SDA
	#ifdef HARD_IIC
	GPIO_PinAFConfig(IIC_SDA_PORT, IIC_SDA_SRC, IIC_BSP_AF); 
	#endif

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
	

	#ifdef HARD_IIC
	I2C_InitStructure.I2C_Mode                = IIC_MODE;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1         = IIC_ADDR;
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = 200000;
	
	I2C_Cmd(IIC_USING, ENABLE);
	//I2C_ITConfig(IIC_USING, I2C_IT_EVT | I2C_IT_ERR , ENABLE);
	I2C_Init(IIC_USING, &I2C_InitStructure);
	I2C_AcknowledgeConfig(IIC_USING, ENABLE);
	IIC_USING->CR1 |= 0x80;
	#else
	IIC_SCL =1 ;
	IIC_SDA = 1;
	#endif
}   



void IIC_DMA_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
 
	NVIC_InitStruct.NVIC_IRQChannel                   = IIC_TX_DMA_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
  
	NVIC_InitStruct.NVIC_IRQChannel                   = IIC_RX_DMA_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

#define Conn(x,y) x##y

void IIC_TX_DMA_Config(const uint8_t* pbuff, uint8_t len)
{
	DMA_InitTypeDef DMA_InitStruct;
  RCC_AHB1PeriphClockCmd(IIC_DMA_PERIPH, ENABLE);
 
  DMA_ClearFlag(IIC_TX_DMA_STREAM,  DMA_FLAG_FEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_TEIF6 | DMA_FLAG_HTIF6 | DMA_FLAG_TCIF6);                              
	DMA_Cmd(IIC_TX_DMA_STREAM, DISABLE);
  while (DMA_GetCmdStatus(IIC_TX_DMA_STREAM) != DISABLE);
  
  DMA_InitStruct.DMA_Channel            = IIC_TX_DMA_CHANNEL;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&IIC_USING->DR;   
	DMA_InitStruct.DMA_Memory0BaseAddr    = (u32)&pbuff[0];
	DMA_InitStruct.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
	DMA_InitStruct.DMA_BufferSize         = len+1;//IIC_BUFF_LEN;
	DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority           = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	DMA_Init(IIC_TX_DMA_STREAM, &DMA_InitStruct); 
  
  DMA_ITConfig(IIC_TX_DMA_STREAM, DMA_IT_TC|DMA_IT_TE, ENABLE);
  I2C_DMACmd(IIC_USING, ENABLE);
  
  while (DMA_GetCmdStatus(IIC_TX_DMA_STREAM) != DISABLE);
  DMA_Cmd(IIC_TX_DMA_STREAM, ENABLE);
}
 
void IIC_RX_DMA_Config(u8 *pbuff, u16 len, u8 it_mode)
{
  DMA_InitTypeDef DMA_InitStruct;
	RCC_AHB1PeriphClockCmd(IIC_DMA_PERIPH, ENABLE);
  
  DMA_Cmd(IIC_RX_DMA_STREAM, DISABLE);
  while (DMA_GetCmdStatus(IIC_RX_DMA_STREAM) != DISABLE);
  
  DMA_InitStruct.DMA_Channel            = IIC_RX_DMA_CHANNEL;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&IIC_USING->DR;   
	DMA_InitStruct.DMA_Memory0BaseAddr    = (u32)&pbuff[0];
	DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_BufferSize         = len;
	DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStruct.DMA_Priority           = DMA_Priority_VeryHigh;
	DMA_InitStruct.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
	DMA_InitStruct.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	DMA_Init(IIC_RX_DMA_STREAM, &DMA_InitStruct); 
 
  DMA_ITConfig(IIC_RX_DMA_STREAM, DMA_IT_TC|DMA_IT_TE, ENABLE);
  
  I2C_DMACmd(IIC_USING, ENABLE);
  
  while (DMA_GetCmdStatus(IIC_RX_DMA_STREAM) != DISABLE);
  I2C_DMALastTransferCmd(IIC_USING, ENABLE);
 
  DMA_Cmd(IIC_RX_DMA_STREAM, ENABLE);
}


void IIC_BSP_Init(void)
{
	IIC_PORT_Init();
	#ifdef HARD_IIC
	IIC_DMA_Init();
	#endif
}
 
#define I2C_TIME		       ((u32)128)


u32 I2C_DMA_Write(u8 SlaveAddr, u8 WriteAddr, const u8 * pData, u8 len)
{
	u32 I2C_TimeCnt;
	u16 sta;
	u32 err = 0;
	UNUSED(sta);
  
	stopIIC_DMA_TX();

 
	I2C_TimeCnt = I2C_TIME;
	while(I2C_GetFlagStatus(IIC_USING, I2C_FLAG_BUSY))
  {
    if((I2C_TimeCnt--) == 0)
    {
      err |= (0x01<<0);
      I2C_GenerateSTOP(IIC_USING, ENABLE);
			return err;
    }   
  }
  
	I2C_GenerateSTART(IIC_USING, ENABLE);
	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(IIC_USING, I2C_EVENT_MASTER_MODE_SELECT))
	{
    if((I2C_TimeCnt--) == 0)
    {
      err |= (0x01<<1);
      I2C_GenerateSTOP(IIC_USING, ENABLE);
			return err;
    }      
  }
  
	I2C_Send7bitAddress(IIC_USING, SlaveAddr, I2C_Direction_Transmitter);
	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(IIC_USING, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2C_TimeCnt--) == 0)
    {
      err |= (0x01<<2);
      I2C_GenerateSTOP(IIC_USING, ENABLE);
			return err;
    } 
  }
  sta = IIC_USING->SR2;
  I2C_SendData(IIC_USING, WriteAddr);
	I2C_TimeCnt = I2C_TIME;
	while(!I2C_CheckEvent(IIC_USING, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((I2C_TimeCnt--) == 0)
    {
      err |= (0x01<<3);
      I2C_GenerateSTOP(IIC_USING, ENABLE);
			return err;
    } 
  }

	Is_IIC_DMA_TX_Stop = false;
	Is_IIC_TX_Stop = false;
	IIC_TX_DMA_Config(pData, len);
	return err;
}
 
 

 
 void stopIIC_DMA_TX(void)
{
	while(!Is_IIC_DMA_TX_Stop);
	if(!Is_IIC_TX_Stop)
	{
		u32 timeOut = I2C_TIME;
		while(!I2C_GetFlagStatus(IIC_USING, I2C_FLAG_BTF))
    {
      if((timeOut--) == 0) 
      {
				break;
      }
    }

		DMA_Cmd(IIC_TX_DMA_STREAM, DISABLE);
		I2C_GenerateSTOP(IIC_USING, ENABLE);
		Is_IIC_TX_Stop = true;
	}
}

void IIC_TX_IRQ_HANDLER(void)
{
	if(DMA_GetFlagStatus(IIC_TX_DMA_STREAM, IIC_TX_FLAG) != RESET)
  {
		DMA_Cmd(IIC_TX_DMA_STREAM, DISABLE);
		DMA_ClearFlag(IIC_TX_DMA_STREAM, IIC_TX_FLAG);

		I2C_GenerateSTOP(IIC_USING, ENABLE);
	}
  else if(DMA_GetITStatus(IIC_TX_DMA_STREAM, IIC_TX_ERR_FLAG) != RESET)
  {
		DMA_ClearITPendingBit(IIC_TX_DMA_STREAM,IIC_TX_ERR_FLAG);
    stopIIC_DMA_TX();
  }
	Is_IIC_DMA_TX_Stop = true;
}


#include "delay.h"

void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;
}	  

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;
	delay_us(4);							   	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN(); 
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;	   
	return 0;  
} 

void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
   
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     

void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				delay_us(2);
				IIC_SCL=1;
				delay_us(2); 
				IIC_SCL=0;	
				delay_us(2);
    }	 
} 	    

u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}

u32 I2C_WriteReg(u8 SlaveAddr, u8 WriteAddr,const u8 data)
{
	#ifdef HARD_IIC
	return I2C_DMA_Write(SlaveAddr, WriteAddr, &data, 1);
	#else
	IIC_Start();
	IIC_Send_Byte(SlaveAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
	return 0;
	#endif
}




 

