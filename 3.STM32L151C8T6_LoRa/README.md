# LoRa Receive and Transmit example

## Hardware:
* STM32L151C8T6
* SX1278

## Tool:
* keil
* STM32CubeMx

## Download:
* J-link OB

## Options:
* Crystal: 8Mhz
* Crystal: 8Mhz
* HCLK: 32Mhz
* USART1:
	* Mode: Asynchronous
	* BAUD: 115200

* SPI2:
	* Mode: Full-Duples Master
	* Frame Format: Motorola
	* Data size: 8 Bits
	* First Bit: MSB First
	* Prescaler: 256
	* Baud Rate: 125k
	* Clock Polarity: Low
	* Clock Phase: 1 Edge

## Hardware connection:

SX1278|<---->|STM32
-|-|-
MOSI|<---->| (SPI2_MOSI)PB15
MISO|<---->| (SPI2_MISO)PB14
SCK|<---->| (SPI2_SCK)PB13
NSS|<---->|(SPI2_NSS)PB12
DI0|<---->|PB10
RESET|<---->|PB11

Optional:
LED1-->PB0
LED2-->PB1

## Result: