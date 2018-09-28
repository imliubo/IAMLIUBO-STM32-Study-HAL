#include "AT24Cxx_Si2c.h"


//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
uint8_t AT24CXX_Si2c_ReadOneByte(SI2C_HW_t * Si2c_hw,uint8_t ReadAddr)
{				  
	uint8_t temp=0;		  	    																 
	Si2c_Start(Si2c_hw);
	if(AT24_TYPE>AT24C16)
	{
		Si2c_Send_Byte(Si2c_hw,AT24CXX_ADDRESS);	                         //发送写命令
		Si2c_Wait_ACK(Si2c_hw);
		Si2c_Send_Byte(Si2c_hw,ReadAddr>>AT24CXX_PAGE_SIZE);               //发送高地址	    
	}else{
		Si2c_Send_Byte(Si2c_hw,(AT24CXX_ADDRESS+((ReadAddr/256)<<1)));     //发送器件地址0XA0,写数据 	   
	}
	Si2c_Wait_ACK(Si2c_hw); 
	Si2c_Send_Byte(Si2c_hw,(ReadAddr%256));   //发送低地址
	Si2c_Wait_ACK(Si2c_hw);	    
	Si2c_Start(Si2c_hw);  	 	   
	Si2c_Send_Byte(Si2c_hw,0XA1);             //进入接收模式			   
	Si2c_Wait_ACK(Si2c_hw);	 
	temp=Si2c_Read_Byte(Si2c_hw,0);		   
	Si2c_Stop(Si2c_hw);                       //产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_Si2c_WriteOneByte(SI2C_HW_t * Si2c_hw,uint8_t WriteAddr,uint8_t DataToWrite)
{	
  Si2c_Start(Si2c_hw); 
	if(AT24_TYPE>AT24C16)
	{
		Si2c_Send_Byte(Si2c_hw,AT24CXX_ADDRESS);	   					              //发送写命令
		Si2c_Wait_ACK(Si2c_hw);
		Si2c_Send_Byte(Si2c_hw,WriteAddr>>AT24CXX_PAGE_SIZE);			          //发送高地址	 
	}else{
		Si2c_Send_Byte(Si2c_hw,(AT24CXX_ADDRESS+((WriteAddr/256)<<1)));     //发送器件地址0XA0,写数据 	 
	}		
	Si2c_Wait_ACK(Si2c_hw); 
  Si2c_Send_Byte(Si2c_hw,(WriteAddr%256));  	 //发送低地址
	Si2c_Wait_ACK(Si2c_hw); 	 										  		   
	Si2c_Send_Byte(Si2c_hw,DataToWrite);    		 //发送字节							   
	Si2c_Wait_ACK(Si2c_hw);  		    	   
  Si2c_Stop(Si2c_hw);													 //产生一个停止条件 
	HAL_Delay(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_Si2c_WriteLenByte(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint16_t DataToWrite,uint8_t Len)
{  	
	uint8_t t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,(WriteAddr+t),((DataToWrite>>(8*t))&0xff));
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址来存储标志字.
//返回1:检测失败
//返回0:检测成功
uint8_t AT24CXX_Si2c_Check(SI2C_HW_t * Si2c_hw)
{
	uint8_t temp;
	temp=AT24CXX_Si2c_ReadOneByte(Si2c_hw,AT24_TYPE);          //避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else{																				               //排除第一次初始化的情况
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,AT24_TYPE,0X55);
	  temp=AT24CXX_Si2c_ReadOneByte(Si2c_hw,AT24_TYPE);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Si2c_Write(SI2C_HW_t * Si2c_hw,uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_Si2c_WriteOneByte(Si2c_hw,WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Si2c_Read(SI2C_HW_t * Si2c_hw,uint16_t ReadAddr,uint8_t *pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_Si2c_ReadOneByte(Si2c_hw,(ReadAddr++));	
		NumToRead--;
	}
}  

