#ifndef __SI2C_H__
#define __SI2C_H__

#include "gpio.h"

typedef struct{
	GPIO_TypeDef* Si2c_SDA_Port;
	uint16_t      Si2c_SDA_GPIO;
	
	GPIO_TypeDef* Si2c_SCL_Port;
	uint16_t      Si2c_SCL_GPIO;
}SI2C_HW_t;

//IIC所有操作函数
void Si2c_SCL_HIGH(SI2C_HW_t * Si2c_hw);
void Si2c_SCL_LOW(SI2C_HW_t * Si2c_hw);

void Si2c_SDA_HIGH(SI2C_HW_t * Si2c_hw);
void Si2c_SDA_LOW(SI2C_HW_t * Si2c_hw);

GPIO_PinState Si2c_SDA_Status(SI2C_HW_t * Si2c_hw);

void Si2c_SDA_IN(SI2C_HW_t * Si2c_hw);
void Si2c_SDA_OUT(SI2C_HW_t * Si2c_hw);

void Si2c_Init(SI2C_HW_t * Si2c_hw);                //初始化IIC的IO口				 
void Si2c_Start(SI2C_HW_t * Si2c_hw);				//发送IIC开始信号
void Si2c_Stop(SI2C_HW_t * Si2c_hw);	 			//发送IIC停止信号

uint8_t Si2c_Wait_ACK(SI2C_HW_t * Si2c_hw); 				//IIC等待ACK信号
void Si2c_Send_ACK(SI2C_HW_t * Si2c_hw);									//IIC发送ACK信号
void Si2c_N0_ACK(SI2C_HW_t * Si2c_hw);										//IIC不发送ACK信号

void Si2c_Send_Byte(SI2C_HW_t * Si2c_hw,uint8_t buf);			//IIC发送一个字节
uint8_t Si2c_Read_Byte(SI2C_HW_t * Si2c_hw,uint8_t ACK);//IIC读取一个字节

void Si2c_Write_One_Byte(uint8_t daddr,unsigned char addr,uint8_t data);
uint8_t Si2c_Read_One_Byte(uint8_t daddr,uint8_t addr);	 

#endif

