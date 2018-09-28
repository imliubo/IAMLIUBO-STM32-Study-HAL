#include "AT24Cxx_Si2c.h"


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
uint8_t AT24CXX_Si2c_ReadOneByte(SI2C_HW_t * Si2c_hw,uint8_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
	Si2c_Start(Si2c_hw);
	if(AT24_TYPE>AT24C16)
	{
		Si2c_Send_Byte(Si2c_hw,AT24CXX_ADDRESS);	                         //����д����
		Si2c_Wait_ACK(Si2c_hw);
		Si2c_Send_Byte(Si2c_hw,ReadAddr>>AT24CXX_PAGE_SIZE);               //���͸ߵ�ַ	    
	}else{
		Si2c_Send_Byte(Si2c_hw,(AT24CXX_ADDRESS+((ReadAddr/256)<<1)));     //����������ַ0XA0,д���� 	   
	}
	Si2c_Wait_ACK(Si2c_hw); 
	Si2c_Send_Byte(Si2c_hw,(ReadAddr%256));   //���͵͵�ַ
	Si2c_Wait_ACK(Si2c_hw);	    
	Si2c_Start(Si2c_hw);  	 	   
	Si2c_Send_Byte(Si2c_hw,0XA1);             //�������ģʽ			   
	Si2c_Wait_ACK(Si2c_hw);	 
	temp=Si2c_Read_Byte(Si2c_hw,0);		   
	Si2c_Stop(Si2c_hw);                       //����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_Si2c_WriteOneByte(SI2C_HW_t * Si2c_hw,uint8_t WriteAddr,uint8_t DataToWrite)
{	
  Si2c_Start(Si2c_hw); 
	if(AT24_TYPE>AT24C16)
	{
		Si2c_Send_Byte(Si2c_hw,AT24CXX_ADDRESS);	   					              //����д����
		Si2c_Wait_ACK(Si2c_hw);
		Si2c_Send_Byte(Si2c_hw,WriteAddr>>AT24CXX_PAGE_SIZE);			          //���͸ߵ�ַ	 
	}else{
		Si2c_Send_Byte(Si2c_hw,(AT24CXX_ADDRESS+((WriteAddr/256)<<1)));     //����������ַ0XA0,д���� 	 
	}		
	Si2c_Wait_ACK(Si2c_hw); 
  Si2c_Send_Byte(Si2c_hw,(WriteAddr%256));  	 //���͵͵�ַ
	Si2c_Wait_ACK(Si2c_hw); 	 										  		   
	Si2c_Send_Byte(Si2c_hw,DataToWrite);    		 //�����ֽ�							   
	Si2c_Wait_ACK(Si2c_hw);  		    	   
  Si2c_Stop(Si2c_hw);													 //����һ��ֹͣ���� 
	HAL_Delay(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_Si2c_WriteLenByte(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint16_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,(WriteAddr+t),((DataToWrite>>(8*t))&0xff));
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
uint16_t AT24CXX_Si2c_ReadLenByte(SI2C_HW_t * Si2c_hw,uint16_t ReadAddr,uint8_t Len)
{  	
	uint8_t t;
	uint16_t temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_Si2c_ReadOneByte(Si2c_hw,(ReadAddr+Len-t-1)); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ���洢��־��.
//����1:���ʧ��
//����0:���ɹ�
uint8_t AT24CXX_Si2c_Check(SI2C_HW_t * Si2c_hw)
{
	uint8_t temp;
	temp=AT24CXX_Si2c_ReadOneByte(Si2c_hw,AT24_TYPE);          //����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else{																				               //�ų���һ�γ�ʼ�������
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,AT24_TYPE,0X55);
	  temp=AT24CXX_Si2c_ReadOneByte(Si2c_hw,AT24_TYPE);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Si2c_Write(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Si2c_Read(SI2C_HW_t * Si2c_hw,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_Si2c_ReadOneByte(Si2c_hw,(ReadAddr++));	
		NumToRead--;
	}
}  

