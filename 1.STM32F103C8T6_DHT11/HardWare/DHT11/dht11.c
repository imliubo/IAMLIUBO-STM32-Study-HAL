/*
*���ߣ�imliubo   ʱ�䣺2018-01-04
*������ҳ��http://www.imliubo.me  Github:http://github.com/imliubo
*˵����DHT11ģ����������
*DHT11_GPIO_Port��DHT11_Pin��ض�����main.h�ļ�
*/

#include "dht11.h"


//˽�к�������
static uint8_t DHT11_ReadByte(void);
static void DHT11_IO_IN(void);
static void DHT11_IO_OUT(void);

//����������Ϊ����ģʽ
void DHT11_IO_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

//����������Ϊ���ģʽ
void DHT11_IO_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

//DHT11��ʼ��
void DHT11_Init(void)
{
	DHT11_IO_HIGH();	//��DHT11_Pin����
}

//��DHT11��ȡһ���ֽ�,����ֵbyte

uint8_t DHT11_ReadByte(void)
{
	uint8_t i,byte=0;
	for(i=0;i<8;i++)
	{
		//ÿbit������50us�͵�ƽ��ʼ���ȴ��͵�ƽ����
		while(DHT11_IO_Read()==GPIO_PIN_RESET);
		/*
		*DHT11�Ըߵ�ƽ��ʱ�䳤�̱�ʾ0��1,26~28us��ʾ0��70us��ʾ1��
		*/
		
		Delay_us(40);//��ʱ x us ��������0��ʱ�伴��
		
		if(DHT11_IO_Read()==GPIO_PIN_SET)//��ʱx us������Ϊ�ߵ�ƽ��ʾ����1
		{
			//�ȴ�����1�ߵ�ƽ����
			while(DHT11_IO_Read()==GPIO_PIN_SET);
			//��7-iΪ��1��
			byte|=(uint8_t)(0x01<<(7-i));
		}else{//��ʱ x us��Ϊ�͵�ƽ  ��ʾ����0
			//��7-iλ��0��
			byte&=(uint8_t)~(0x01<<(7-i));
		}
	}
	
	return byte;//����һ��bit��ֵ
}

uint8_t DHT11_Read_Data(DHT11_ReadTypeDef *DHT11_DATA)
{
		uint8_t i;
		uint8_t buf[5];
		
		DHT11_IO_OUT();//��������Ϊ���ģʽ
		
		DHT11_IO_LOW();//��������
		
		Delay_ms(18);//��ʱ18ms
		
		DHT11_IO_HIGH();//����
		
		Delay_us(30);//��ʱ30us
		
		DHT11_IO_IN();//��������Ϊ����ģʽ
	
		if(DHT11_IO_Read()==GPIO_PIN_RESET)//�ж�DHT11�Ƿ��е͵�ƽ��Ӧ�źţ�������������У�û������
		{
			while(DHT11_IO_Read()==GPIO_PIN_RESET);//�ȴ��͵�ƽ��Ӧ�źŽ���
			
			while(DHT11_IO_Read()==GPIO_PIN_SET);//�ȴ��ߵ�ƽ��Ӧ�źŽ���
			
			for(i=0;i<5;i++)
			{
				buf[i]=DHT11_ReadByte();//��ȡ5bit�����ݡ����뵽buf������
			}
			if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])//У����յ��������Ƿ���ȷ
			{
				printf("read data success!\r\n");
				DHT11_DATA->Humidity=buf[0];//buf[0]���뵽Humidity
				DHT11_DATA->Temperature=buf[2];//buf[2]���뵽Temperature
				//DHT11����Ϊ1��buf[1],buf[3]��ȡ��С��λ�����Բ��ô��� ֱ����ȥ
			}else{
				printf("read data error!\r\n");
			}
		}else{
			printf("DHT11 NO DATA!\r\n");
		}
		return 1;
}


