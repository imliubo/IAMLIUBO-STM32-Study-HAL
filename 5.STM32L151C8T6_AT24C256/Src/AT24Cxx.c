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
#include "AT24Cxx.h"

/**
  * @brief  Manages error callback by re-initializing I2C.
  * @param  Addr: I2C Address
  */
static void I2Cx_Error(I2C_HandleTypeDef *hi2c, uint8_t Addr)
{
	/* De-initialize the IOE comunication BUS */
  HAL_I2C_DeInit(hi2c);
  
  /* Re-Initiaize the IOE comunication BUS */
  MX_I2C1_Init();  
}

/**
  * @brief  Reads a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @retval Data to be read
  */
static uint8_t I2Cx_Read(I2C_HandleTypeDef *hi2c, uint8_t Addr, uint8_t Reg, uint16_t MemAddSize)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t Value = 0;
  
  status = HAL_I2C_Mem_Read(hi2c, Addr, Reg, MemAddSize, &Value, 1, I2C_TIMEOUT);//2018-08-23
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    I2Cx_Error(hi2c, Addr);//2018-8-23
  }
  
  return Value;   
}

/**
  * @brief  Writes a single data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @param  Value: Data to be written
  */
static void I2Cx_Write(I2C_HandleTypeDef *hi2c, uint8_t Addr, uint8_t Reg, uint16_t MemAddSize, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(hi2c, Addr, (uint16_t)Reg, MemAddSize, &Value, 1, I2C_TIMEOUT); //2018-08-23
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(hi2c,Addr);//2018-08-23
  }
}

/**
  * @brief  Reads multiple data.
  * @param  Addr: I2C address
  * @param  Reg: Reg address 
  * @param  MemAddSize Size of internal memory address
  * @param  Buffer: Pointer to data buffer
  * @param  Length: Length of the data
  * @retval Number of read data
  */
static HAL_StatusTypeDef I2Cx_ReadMultiple(I2C_HandleTypeDef *hi2c, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read(hi2c, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(hi2c, Addr);
  }
  return status;    
}

/**
  * @brief  Write a value in a register of the device through BUS in using DMA mode
  * @param  Addr: Device address on BUS Bus.  
  * @param  Reg: The target register address to write
  * @param  MemAddSize Size of internal memory address
  * @param  Buffer: The target register value to be written 
  * @param  Length: buffer size to be written
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_WriteMultiple(I2C_HandleTypeDef *hi2c, uint8_t Addr, uint16_t Reg, uint16_t MemAddSize, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(hi2c, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(hi2c, Addr);
  }
  return status;
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  DevAddress: Target device address
  * @param  Trials: Number of trials
  * @retval HAL status
  */
static HAL_StatusTypeDef I2Cx_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(hi2c, DevAddress, Trials, I2C_TIMEOUT));//2018-08-23
}


/**************************************************************** 
**										LINK I2C AT24CXX 												 **
*****************************************************************/

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  */
/*
void AT24CXX_Init(void)
{
  I2Cx_Init();
}
*/
/**
  * @brief  Camera writes single data.
  * @param  Reg: Reg address 
  * @param  Value: Data to be written
  */
void AT24CXX_WriteByte(I2C_HandleTypeDef *hi2c, uint8_t Reg, uint8_t Value)
{
  I2Cx_Write(hi2c,AT24CXX_ADDRESS, Reg, AT24CXX_MEMADD_SIZE, Value);
	HAL_Delay(2);	
}

/**
  * @brief  Camera reads single data.
  * @param  Reg: Reg address 
  * @retval Read data
  */
uint8_t AT24CXX_ReadByte(I2C_HandleTypeDef *hi2c, uint8_t Reg)
{
  return I2Cx_Read(hi2c, AT24CXX_ADDRESS, Reg, AT24CXX_MEMADD_SIZE);
}

/** @brief: You can write bytes to EEPROM at one time, but do not exceeds the number of 
	*					bytes per page of an EEPROM, AT24C04 16byte/page
	* @param: pBuffer: the pointer to data buffer
	*       	MemAddress: the EEPROM address of recieve data
	*       	BufferSize: Amount of data to be writed to EEPROM
	*/
HAL_StatusTypeDef AT24CXX_PageWrite(I2C_HandleTypeDef *hi2c, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = I2Cx_WriteMultiple(hi2c, AT24CXX_ADDRESS,MemAddress,AT24CXX_MEMADD_SIZE, pBuffer,BufferSize);
	HAL_Delay(10);
	
	return status;
}

/**
  * @brief Write data to I2C EEPROM driver in using DMA channel
  * @param MemAddress: memory address
  * @param pBuffer: Pointer to data buffer
  * @param BufferSize: Amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef AT24CXX_WriteData(I2C_HandleTypeDef *hi2c, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
	HAL_StatusTypeDef err = HAL_OK;

	Addr = MemAddress % AT24CXX_PAGE_SIZE;
	count = AT24CXX_PAGE_SIZE - Addr;
	NumOfPage =  BufferSize / AT24CXX_PAGE_SIZE;
	NumOfSingle = BufferSize % AT24CXX_PAGE_SIZE;
 
	/* If WriteAddr is I2C_PageSize aligned  */
	if(Addr == 0) 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(NumOfPage == 0) 
		{
			return (AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,BufferSize));
		}
		/* If NumByteToWrite > I2C_PageSize */
		else  
		{
			while(NumOfPage--)
			{
				err = AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,AT24CXX_PAGE_SIZE);
				if(err != HAL_OK)
					return err; 
				MemAddress +=  AT24CXX_PAGE_SIZE;
				pBuffer += AT24CXX_PAGE_SIZE;
			}

			if(NumOfSingle!=0)
			{
				return (AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,NumOfSingle)); 
			}
		}
	}
	  /* If WriteAddr is not I2C_PageSize aligned  */
	else 
	{
		/* If NumByteToWrite < I2C_PageSize */
		if(BufferSize <= count) 
		{
			return (AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,NumOfSingle));
		}
		/* If NumByteToWrite > I2C_PageSize */
		else
		{
			BufferSize -= count;
			NumOfPage =  BufferSize / AT24CXX_PAGE_SIZE;
			NumOfSingle = BufferSize % AT24CXX_PAGE_SIZE;	
		  
			err = AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,count);
			if(err != HAL_OK)
				return err; 
			MemAddress += count;
			pBuffer += count;
		  
			while(NumOfPage--)
			{
				err = AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,AT24CXX_PAGE_SIZE);
				if(err != HAL_OK)
					return err;
				MemAddress +=  AT24CXX_PAGE_SIZE;
				pBuffer += AT24CXX_PAGE_SIZE;  
			}
			
			if(NumOfSingle != 0)
			{
				return (AT24CXX_PageWrite(hi2c, MemAddress,pBuffer,NumOfSingle)); 
			}
		}
	}	
/*
	while(BufferSize--)
	{
		AT24CXX_WriteByte(MemAddress,*pBuffer);
		MemAddress++;
		pBuffer++;
	}
*/
	return err;
}

/**
  * @brief  Reads data from I2C EEPROM driver in using DMA channel.
  * @param  MemAddress: memory address
  * @param  pBuffer: Pointer to data buffer
  * @param  BufferSize: Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef AT24CXX_ReadData(I2C_HandleTypeDef *hi2c, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_ReadMultiple(hi2c, AT24CXX_ADDRESS, MemAddress, AT24CXX_MEMADD_SIZE, pBuffer, BufferSize));
}

/**
  * @brief  Checks if target device is ready for communication. 
  * @note   This function is used with Memory devices
  * @param  Trials: Number of trials
  * @retval HAL status
  */
HAL_StatusTypeDef AT24CXX_IsDeviceReady(I2C_HandleTypeDef *hi2c, uint32_t Trials)
{ 
  return (I2Cx_IsDeviceReady(hi2c, AT24CXX_ADDRESS, Trials));
}

