#ifndef _DHT11_H
#define _DHT11_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "delay.h"

typedef struct	
{
	uint8_t             Temperature;
	uint8_t             Humidity;	                 
}DHT11_ReadTypeDef;

#define  DHT11_IO_Read() HAL_GPIO_ReadPin(DHT11_GPIO_Port,DHT11_Pin)

#define  DHT11_IO_LOW()  HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_RESET)

#define  DHT11_IO_HIGH() HAL_GPIO_WritePin(DHT11_GPIO_Port,DHT11_Pin,GPIO_PIN_SET)

void DHT11_Init(void);

uint8_t DHT11_Read_Data(DHT11_ReadTypeDef *DHT11_DATA);


#endif

