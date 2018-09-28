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


//����IIC��ʼ�ź�
void Si2c_Start(SI2C_HW_t * Si2c_hw)
{
	Si2c_SDA_OUT(Si2c_hw);     //sda�����
	Si2c_SDA_HIGH(Si2c_hw);	  	  
	Si2c_SCL_HIGH(Si2c_hw);
	HAL_Delay(4);
 	Si2c_SDA_LOW(Si2c_hw);//START:when CLK is high,DATA change form high to low 
	HAL_Delay(4);
	Si2c_SCL_LOW(Si2c_hw);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void Si2c_Stop(SI2C_HW_t * Si2c_hw)
{
	Si2c_SDA_OUT(Si2c_hw);//sda�����
	Si2c_SCL_LOW(Si2c_hw);
	Si2c_SDA_LOW(Si2c_hw);//STOP:when CLK is high DATA change form low to high
 	HAL_Delay(4);
	Si2c_SCL_HIGH(Si2c_hw); 
	Si2c_SDA_HIGH(Si2c_hw);	//����I2C���߽����ź�
	HAL_Delay(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t Si2c_Wait_ACK(SI2C_HW_t * Si2c_hw)
{
	uint8_t ucErrTime=0;
	Si2c_SDA_IN(Si2c_hw);      			//SDA����Ϊ����  
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
	Si2c_SCL_LOW(Si2c_hw);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void Si2c_Send_Byte(SI2C_HW_t * Si2c_hw,uint8_t buf)
{                        
    uint8_t t;   
    Si2c_SDA_OUT(Si2c_hw);	    
    Si2c_SCL_LOW(Si2c_hw);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {       
      if(buf&0x80)
      {
       Si2c_SDA_HIGH(Si2c_hw);
      }else{
       Si2c_SDA_LOW(Si2c_hw);
      }
      buf<<=1; 	  
      HAL_Delay(2);   //��TEA5767��������ʱ���Ǳ����
      Si2c_SCL_HIGH(Si2c_hw);
      HAL_Delay(2); 
      Si2c_SCL_LOW(Si2c_hw);
      HAL_Delay(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t Si2c_Read_Byte(SI2C_HW_t * Si2c_hw,uint8_t ACK)
{
	uint8_t i,buf=0;
	Si2c_SDA_IN(Si2c_hw);//SDA����Ϊ����
	
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
		Si2c_N0_ACK(Si2c_hw);     //����nACK
	}else{
		Si2c_Send_ACK(Si2c_hw);   //����ACK 
	}
	
  return buf;
}
