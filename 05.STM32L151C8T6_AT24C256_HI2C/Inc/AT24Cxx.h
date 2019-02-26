/*
MIT License

Copyright (c) 2018 imliubo  Github  https://github.com/imliubo 
				    Website https://www.makingfun.xyz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef _AT24CXX_H
#define _AT24CXX_H



#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l1xx_hal.h"
#include "i2c.h"
#include "main.h"
 
#define AT24C01		127		//PAGE_SIZE	8	byte
#define AT24C02		255		//PAGE_SIZE	8	byte
#define AT24C04		511		//PAGE_SIZE	16	byte
#define AT24C08		1023	//PAGE_SIZE	16	byte
#define AT24C16		2047	//PAGE_SIZE	16	byte
#define AT24C32		4095    //PAGE_SIZE	16	byte
#define AT24C64	  8191    //PAGE_SIZE	16	byte
#define AT24C128	16383   //PAGE_SIZE	16	byte
#define AT24C256	32767   //PAGE_SIZE	16	byte

	 
#define AT24CXX_TYPE						AT24C256
#define AT24CXX_ADDRESS						0XA0
#define AT24CXX_PAGE_SIZE					8

#if	(AT24CXX_TYPE < AT24C04)
	#define AT24CXX_MEMADD_SIZE				I2C_MEMADD_SIZE_8BIT	
#else
	#define AT24CXX_MEMADD_SIZE				I2C_MEMADD_SIZE_16BIT	
#endif

#define I2C_TIMEOUT  						100

uint8_t AT24CXX_ReadByte(I2C_HandleTypeDef *hi2c,uint8_t Reg);
HAL_StatusTypeDef AT24CXX_IsDeviceReady(I2C_HandleTypeDef *hi2c,uint32_t Trials);
void AT24CXX_WriteByte(I2C_HandleTypeDef *hi2c,uint8_t Reg, uint8_t Value);
HAL_StatusTypeDef AT24CXX_PageWrite(I2C_HandleTypeDef *hi2c,uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef AT24CXX_WriteData(I2C_HandleTypeDef *hi2c,uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef AT24CXX_ReadData(I2C_HandleTypeDef *hi2c,uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);


#ifdef __cplusplus
}
#endif
#endif

