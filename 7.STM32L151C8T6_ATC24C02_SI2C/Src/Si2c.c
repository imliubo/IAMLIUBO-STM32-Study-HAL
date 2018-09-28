#include "Si2c.h"



void Si2c_SCL_HIGH(SI2C_HW_t * Si2c_hw)
{
	 HAL_GPIO_WritePin(Si2c_hw->Si2c_SCL_Port,Si2c_hw->Si2c_SCL_GPIO,GPIO_PIN_SET);
}

void Si2c_SCL_LOW(SI2C_HW_t * Si2c_hw)
{
	 HAL_GPIO_WritePin(Si2c_hw->Si2c_SCL_Port,Si2c_hw->Si2c_SCL_GPIO,GPIO_PIN_RESET);
}

void Si2c_SDA_HIGH(SI2C_HW_t * Si2c_hw)
{
	 HAL_GPIO_WritePin(Si2c_hw->Si2c_SDA_Port,Si2c_hw->Si2c_SDA_GPIO,GPIO_PIN_SET);
}

void Si2c_SDA_LOW(SI2C_HW_t * Si2c_hw)
{
	 HAL_GPIO_WritePin(Si2c_hw->Si2c_SDA_Port,Si2c_hw->Si2c_SDA_GPIO,GPIO_PIN_RESET);
}

GPIO_PinState Si2c_SDA_Status(SI2C_HW_t * Si2c_hw)
{
	 return(HAL_GPIO_ReadPin(Si2c_hw->Si2c_SDA_Port,Si2c_hw->Si2c_SDA_GPIO));
}

void Si2c_SDA_IN(SI2C_HW_t * Si2c_hw)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin = Si2c_hw->Si2c_SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(Si2c_hw->Si2c_SDA_Port, &GPIO_InitStruct);
}

void Si2c_SDA_OUT(SI2C_HW_t * Si2c_hw)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = Si2c_hw->Si2c_SDA_GPIO;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(Si2c_hw->Si2c_SDA_Port, &GPIO_InitStruct);
}


//产生IIC起始信号
void Si2c_Start(SI2C_HW_t * Si2c_hw)
{
	Si2c_SDA_OUT(Si2c_hw);     //sda线输出
	Si2c_SDA_HIGH(Si2c_hw);	  	  
	Si2c_SCL_HIGH(Si2c_hw);
	HAL_Delay(4);
 	Si2c_SDA_LOW(Si2c_hw);//START:when CLK is high,DATA change form high to low 
	HAL_Delay(4);
	Si2c_SCL_LOW(Si2c_hw);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void Si2c_Stop(SI2C_HW_t * Si2c_hw)
{
	Si2c_SDA_OUT(Si2c_hw);//sda线输出
	Si2c_SCL_LOW(Si2c_hw);
	Si2c_SDA_LOW(Si2c_hw);//STOP:when CLK is high DATA change form low to high
 	HAL_Delay(4);
	Si2c_SCL_HIGH(Si2c_hw); 
	Si2c_SDA_HIGH(Si2c_hw);	//发送I2C总线结束信号
	HAL_Delay(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t Si2c_Wait_ACK(SI2C_HW_t * Si2c_hw)
{
	uint8_t ucErrTime=0;
	Si2c_SDA_IN(Si2c_hw);      			//SDA设置为输入  
	Si2c_SDA_HIGH(Si2c_hw);
	HAL_Delay(1);	   
	Si2c_SCL_HIGH(Si2c_hw);
	HAL_Delay(1);	 
	while(Si2c_SDA_Status(Si2c_hw))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Si2c_Stop(Si2c_hw);
			return 1;
		}
	}
	Si2c_SCL_LOW(Si2c_hw);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void Si2c_Send_ACK(SI2C_HW_t * Si2c_hw)
{
	Si2c_SCL_LOW(Si2c_hw);
	Si2c_SDA_OUT(Si2c_hw);
	Si2c_SDA_LOW(Si2c_hw);
	HAL_Delay(2);
	Si2c_SCL_HIGH(Si2c_hw);
	HAL_Delay(2);
	Si2c_SCL_LOW(Si2c_hw);
}
//不产生ACK应答		    
void Si2c_N0_ACK(SI2C_HW_t * Si2c_hw)
{
	Si2c_SCL_LOW(Si2c_hw);
	Si2c_SDA_OUT(Si2c_hw);
	Si2c_SDA_HIGH(Si2c_hw);
	HAL_Delay(2);
	Si2c_SCL_HIGH(Si2c_hw);
	HAL_Delay(2);
	Si2c_SCL_LOW(Si2c_hw);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void Si2c_Send_Byte(SI2C_HW_t * Si2c_hw,uint8_t buf)
{                        
    uint8_t t;   
    Si2c_SDA_OUT(Si2c_hw);	    
    Si2c_SCL_LOW(Si2c_hw);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {       
      if(buf&0x80)
      {
       Si2c_SDA_HIGH(Si2c_hw);
      }else{
       Si2c_SDA_LOW(Si2c_hw);
      }
      buf<<=1; 	  
      HAL_Delay(2);   //对TEA5767这三个延时都是必须的
      Si2c_SCL_HIGH(Si2c_hw);
      HAL_Delay(2); 
      Si2c_SCL_LOW(Si2c_hw);
      HAL_Delay(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t Si2c_Read_Byte(SI2C_HW_t * Si2c_hw,uint8_t ACK)
{
	uint8_t i,buf=0;
	Si2c_SDA_IN(Si2c_hw);//SDA设置为输入
	
  for(i=0;i<8;i++ )
  {
    Si2c_SCL_LOW(Si2c_hw);
    HAL_Delay(2);
    Si2c_SCL_HIGH(Si2c_hw);
    buf<<=1;
    if(Si2c_SDA_Status(Si2c_hw))
		{
			buf++; 
		}  
    HAL_Delay(1); 
  }
	
  if(!ACK){
		Si2c_N0_ACK(Si2c_hw);     //发送nACK
	}else{
		Si2c_Send_ACK(Si2c_hw);   //发送ACK 
	}
	
  return buf;
}
