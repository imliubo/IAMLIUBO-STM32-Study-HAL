/*
MIT License

Copyright (c) 2018 imliubo

Github  https://github.com/imliubo 
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
#ifndef __DHT11_H__
#define __DHT11_H__
 
#include <stdio.h>
#include <stm32f1xx_hal.h>


//Set GPIO Direction

#define DHT11_GPIO_PIN   DHT11_Pin
#define DHT11_GPIO_PORT  DHT11_GPIO_Port
#define	DHT11_DQ_OUT_1	 	HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define	DHT11_DQ_OUT_0 		HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define	DHT11_DQ_IN   	 	HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

#define MEAN_NUM            10



typedef struct
{
    uint8_t curI;
    uint8_t thAmount; 
    uint8_t thBufs[10][2];
}thTypedef_t; 

/* Function declaration */
uint8_t dht11Init(void); //Init DHT11
uint8_t dht11Read(uint8_t *temperature, uint8_t *humidity); //Read DHT11 Value
static uint8_t dht11ReadData(uint8_t *temperature, uint8_t *humidity);
static uint8_t dht11ReadByte(void);//Read One Byte
static uint8_t dht11ReadBit(void);//Read One Bit
static uint8_t dht11Check(void);//Chack DHT11
static void dht11Rst(void);//Reset DHT11    
void dht11SensorTest(void);

#endif /*_HAL_HEMP_HUM_H*/



