/*
*作者：imliubo   时间：2018-01-04
*个人主页：http://www.imliubo.me  Github:http://github.com/imliubo
*说明：DHT11模块驱动程序
*DHT11_GPIO_Port，DHT11_Pin相关定义在main.h文件
*/

#include "dht11.h"


//私有函数声明
static uint8_t DHT11_ReadByte(void);
static void DHT11_IO_IN(void);
static void DHT11_IO_OUT(void);

//将主机设置为输入模式
void DHT11_IO_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

//将主机设置为输出模式
void DHT11_IO_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

//DHT11初始化
void DHT11_Init(void)
{
	DHT11_IO_HIGH();	//将DHT11_Pin拉高
}

//从DHT11读取一个字节,返回值byte

uint8_t DHT11_ReadByte(void)
{
	uint8_t i,byte=0;
	for(i=0;i<8;i++)
	{
		//每bit数据以50us低电平开始，等待低电平结束
		while(DHT11_IO_Read()==GPIO_PIN_RESET);
		/*
		*DHT11以高电平的时间长短表示0跟1,26~28us表示0，70us表示1。
		*/
		
		Delay_us(40);//延时 x us 大于数据0的时间即可
		
		if(DHT11_IO_Read()==GPIO_PIN_SET)//延时x us过后仍为高电平表示数据1
		{
			//等待数据1高电平结束
			while(DHT11_IO_Read()==GPIO_PIN_SET);
			//把7-i为置1，
			byte|=(uint8_t)(0x01<<(7-i));
		}else{//延时 x us后为低电平  表示数据0
			//把7-i位置0，
			byte&=(uint8_t)~(0x01<<(7-i));
		}
	}
	
	return byte;//返回一个bit的值
}

uint8_t DHT11_Read_Data(DHT11_ReadTypeDef *DHT11_DATA)
{
		uint8_t i;
		uint8_t buf[5];
		
		DHT11_IO_OUT();//主机设置为输出模式
		
		DHT11_IO_LOW();//主机拉低
		
		Delay_ms(18);//延时18ms
		
		DHT11_IO_HIGH();//拉高
		
		Delay_us(30);//延时30us
		
		DHT11_IO_IN();//主机设置为输入模式
	
		if(DHT11_IO_Read()==GPIO_PIN_RESET)//判断DHT11是否有低电平响应信号，如果有向下运行，没有跳出
		{
			while(DHT11_IO_Read()==GPIO_PIN_RESET);//等待低电平响应信号结束
			
			while(DHT11_IO_Read()==GPIO_PIN_SET);//等待高电平响应信号结束
			
			for(i=0;i<5;i++)
			{
				buf[i]=DHT11_ReadByte();//读取5bit的数据。存入到buf数组中
			}
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])//校验接收到的数据是否正确
			{
				printf("read data success!\r\n");
				DHT11_DATA->Humidity=buf[0];//buf[0]存入到Humidity
				DHT11_DATA->Temperature=buf[2];//buf[2]存入到Temperature
				//DHT11精度为1，buf[1],buf[3]存取的小数位，所以不用处理 直接舍去
			}else{
				printf("read data error!\r\n");
			}
		}else{
			printf("DHT11 NO DATA!\r\n");
		}
		return 1;
}


