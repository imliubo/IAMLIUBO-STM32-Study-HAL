
#include "Adafruit_NeoPixel.h"

#define PIXEL_MAX 10

void Send_8bits(uint8_t dat) 
{
  uint8_t i=0;
  uint8_t CodeOne=0x7c;//7c£¬3e
  uint8_t CodeZero=0x70;//70,38
  
  for (i=0;i<8;i++)
  {
    if((dat&0x80)==0x80)
    {
      HAL_SPI_Transmit_DMA(&hspi1, &CodeOne, 1);
    }
    else
    {
      HAL_SPI_Transmit_DMA(&hspi1, &CodeZero, 1); 
    }
    dat=dat<<1;
  }
}
//G--R--B
//MSB first	
void Send_2811_24bits(uint8_t RData,uint8_t GData,uint8_t BData)
{   
  Send_8bits(GData);  
  Send_8bits(RData); 
  Send_8bits(BData);
} 


uint8_t rBuffer[PIXEL_MAX]={0};
uint8_t gBuffer[PIXEL_MAX]={0};
uint8_t bBuffer[PIXEL_MAX]={0};

void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b)
{ 
  uint8_t i=0;
  for(i=0;i<PIXEL_MAX;i++)
  {
    rBuffer[i]=r;
    gBuffer[i]=g;
    bBuffer[i]=b;
  }
  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
  }
  PixelUpdate();
}
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{	 
  uint8_t i=0;
  for(i=0;i<PIXEL_MAX;i++)
  {
    rBuffer[i]=0;
    gBuffer[i]=0;
    bBuffer[i]=0;
  }
  rBuffer[n]=r;
  gBuffer[n]=g;
  bBuffer[n]=b;
  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
  }
  PixelUpdate();
}
void SetPixelColor(uint16_t n, uint32_t c)
{	 
  uint8_t i=0;
   
  rBuffer[n]=(uint8_t)(c>>16);
  gBuffer[n]=(uint8_t)(c>>8);
  bBuffer[n]=(uint8_t)c;

  for(i=0;i<PIXEL_MAX;i++)
  {							  
    Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
  }
   PixelUpdate();
}
void PixelUpdate(void)//should >24us
{
  uint8_t rst[25]={0};
  HAL_SPI_Transmit_DMA(&hspi1, rst, 25);
}
uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}
uint32_t Wheel(uint8_t WheelPos)
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) 
  {
    return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
//2¨ºo?
void rainbow(uint8_t wait)
{
  uint16_t i, j;
  
  for(j=0; j<256; j++) 
  {
    for(i=0; i<PIXEL_MAX; i++)
    {
      SetPixelColor(i, Wheel((i+j) & 255));
    }
    PixelUpdate();
    HAL_Delay (wait);
  }
}
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;
  
  for(j=0; j<256*5; j++) 
  { // 5 cycles of all colors on wheel
    for(i=0; i< PIXEL_MAX; i++) 
    {
      SetPixelColor(i, Wheel(((i * 256 / PIXEL_MAX) + j) & 255));
    }
    PixelUpdate();
    HAL_Delay (wait);
  }
}
//Theatre-style crawling lights.o??¨¹¦Ì?
void theaterChase(uint32_t c, uint8_t wait) 
{
  for (int j=0; j<10; j++) 
  {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) 
    {
      for (uint16_t i=0; i < PIXEL_MAX; i=i+1)//turn every one pixel on
      {
        SetPixelColor(i+q, c);    
      }
      PixelUpdate();
      HAL_Delay(wait);
      
      for (uint16_t i=0; i < PIXEL_MAX; i=i+1) //turn every one pixel off
      {
        SetPixelColor(i+q, 0);        
      }
      PixelUpdate();
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) 
{
  for (int j=0; j < 256; j++) 
  {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++)
    {
      for (uint16_t i=0; i < PIXEL_MAX; i=i+1) //turn every one pixel on
      {
        SetPixelColor(i+q, Wheel( (i+j) % 255));    
      }
      PixelUpdate();
      
      HAL_Delay(wait);
      
      for (uint16_t i=0; i < PIXEL_MAX; i=i+1)//turn every one pixel off
      {
        SetPixelColor(i+q, 0);        
      }
      PixelUpdate();
    }
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) 
{
  uint16_t i=0;
  for( i=0; i<PIXEL_MAX; i++) 
  {
    SetPixelColor(i, c);
    PixelUpdate();
    HAL_Delay(wait);
  }
}

void WS2812B_Init(void)//should >50us
{
  uint8_t ResCode[50]={0};
  HAL_SPI_Transmit_DMA(&hspi1, ResCode, 50);
}

void WS2812B_Test(void)
{
    // Some example procedures showing how to display to the pixels:
    colorWipe(Color(255, 0, 0), 5); // Red
    colorWipe(Color(0, 255, 0), 5); // Green
    colorWipe(Color(0, 0, 255), 5); // Blue
    // Send a theater pixel chase in...
    theaterChase(Color(127, 127, 127), 50); // White
    theaterChase(Color(127, 0, 0), 50); // Red
    theaterChase(Color(0, 127, 0), 50); // Green   
    theaterChase(Color(0, 0, 127), 50); // Blue   
    rainbow(20);//rainbow
    rainbowCycle(20);//rainbowCycle
    theaterChaseRainbow(100);//
    //test code over
}




