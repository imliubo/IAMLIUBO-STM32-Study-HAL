/*
*���ߣ�imliubo ʱ�䣺2017-01-04
*��ҳ:http://www.imliubo.me
*˵������ʱ��������HAL�Ȿ��û��΢�뼶��ʱ������΢�뼶��ʱ��
*/


#include "delay.h"

static uint8_t  fac_us=0;																		//us��ʱ������
static uint16_t fac_ms=0;																		//ms��ʱ������

//��ʱ������ʼ��
void Delay_Init(uint8_t SYSCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK/8; //Ӳ��8��Ƶ,fac_us�ó���ֵ��Ҫ�������ʱ�Ӻ����õ�  
	fac_ms =(uint16_t)fac_us*1000; 	
}	

//΢����ʱ	    								   
void Delay_us(uint32_t nus)
{		
		uint32_t temp;  
		SysTick->LOAD = nus*fac_us;  //��ʱ10us�Ļ�����  10*9=90,װ��load�Ĵ�����  
		SysTick->VAL=0x00;//��������0,��Ϊcurrrent�ֶα��ֶ�����ʱ,load���Զ���װ��VAL��  
		SysTick->CTRL = 0x01;//����ʹ�쳣��Ч,Ҳ���Ǽ�����������0ʱ�������쳣֪ͨ  
		do  
		{  
			 temp = SysTick->CTRL;  //ʱ�䵽��֮��,��λ����Ӳ����1,������ѯ���Զ���0  
		}  
		while(temp & 0x01 && !(temp &(1<<16))); //��ѯ  	
		SysTick->CTRL = 0x00;  //�رռ�����  
		SysTick->VAL = 0x00;   //���val      																//��ռ�����	 
}

//������ʱ
void Delay_ms(uint16_t nms)
{	 		  	  
		uint32_t temp;  
		SysTick->LOAD = nms*fac_ms;  //��ʱ10us�Ļ�����  10*9=90,װ��load�Ĵ�����  
		SysTick->VAL=0x00;//��������0,��Ϊcurrrent�ֶα��ֶ�����ʱ,load���Զ���װ��VAL��  
		SysTick->CTRL = 0x01;//����ʹ�쳣��Ч,Ҳ���Ǽ�����������0ʱ�������쳣֪ͨ  
		do  
		{  
			 temp = SysTick->CTRL;  //ʱ�䵽��֮��,��λ����Ӳ����1,������ѯ���Զ���0  
		}  
		while(temp & 0x01 && !(temp &(1<<16))); //��ѯ  
	
		SysTick->CTRL = 0x00;  //�رռ�����  
		SysTick->VAL = 0x00;   //���val   	  	    
} 
