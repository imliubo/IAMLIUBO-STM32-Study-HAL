
#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#include "stm32f1xx_hal.h"
#include "spi.h"

void Send_8bits(uint8_t dat);
void Send_2811_24bits(uint8_t GData,uint8_t RData,uint8_t BData);
void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b);
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
void SetPixelColor(uint16_t n, uint32_t c);
void PixelUpdate(void);
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
uint32_t Wheel(uint8_t WheelPos);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);
void theaterChaseRainbow(uint8_t wait) ;
void colorWipe(uint32_t c, uint8_t wait);

void WS2812B_Init(void);
void WS2812B_Test(void);

#endif // ADAFRUIT_NEOPIXEL_H
