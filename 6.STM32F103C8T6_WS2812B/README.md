# DHT11 example

## Hardware:
* STM32F103C8T6
* WS2812B

## Tool:
* keil
* STM32CubeMx

## Download:
* J-link OB

## Options:
* Crystal: 8Mhz
* HCLK: 56Mhz
* USART1:
	* Mode: Asynchronous
	* BAUD: 115200

* SPI2:
	* Mode: Transmit Only Master
	* Frame Format: Motorola
	* Data size: 8 Bits
	* First Bit: MSB First
	* Prescaler: 8
	* Baud Rate: 7M
	* Clock Polarity: Low
	* Clock Phase: 1 Edge

## Hardware connection:
WS2812B|<---->|STM32
-|-|-
Data In|<---->| SPI1_MOSI(PA7)

## Result:
