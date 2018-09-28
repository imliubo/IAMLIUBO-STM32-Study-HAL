#ifndef _AT24CXX_SI2C_H
#define _AT24CXX_SI2C_H

#include "Si2c.h"

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	  8191
#define AT24C128	16383
#define AT24C256	32767  

#define AT24_TYPE AT24C02

#define AT24CXX_ADDRESS						0XA0
#define AT24CXX_PAGE_SIZE					8
					  
uint8_t 	AT24CXX_Si2c_ReadOneByte(SI2C_HW_t * Si2c_hw,uint8_t ReadAddr);																			//指定地址读取一个字节
void 			AT24CXX_Si2c_WriteOneByte(SI2C_HW_t * Si2c_hw,uint8_t WriteAddr,uint8_t DataToWrite);									  //指定地址写入一个字节
void 			AT24CXX_Si2c_WriteLenByte(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint16_t DataToWrite,uint8_t Len);	//指定地址开始写入指定长度的数据
uint16_t 	AT24CXX_Si2c_ReadLenByte(SI2C_HW_t * Si2c_hw,uint16_t ReadAddr,uint8_t Len);										  //指定地址开始读取指定长度数据
void 			AT24CXX_Si2c_Write(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);	  //从指定地址开始写入指定长度的数据
void 			AT24CXX_Si2c_Read(SI2C_HW_t * Si2c_hw,uint16_t ReadAddr,uint8_t  *pBuffer,uint16_t NumToRead);   	  //从指定地址开始读出指定长度的数据

uint8_t 	AT24CXX_Si2c_Check(SI2C_HW_t * Si2c_hw);  //检查器件
void 			AT24CXX_Si2c_Init(SI2C_HW_t * Si2c_hw);            //初始化IIC
#endif
