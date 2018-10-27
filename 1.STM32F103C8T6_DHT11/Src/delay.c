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
#include "delay.h"

/*
*********************************************************************************************************
*                                             �Ĵ���
*********************************************************************************************************
*/
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitDWT
*	����˵��: ��ʼ��DWT. �ú����� bsp_Init() ���á�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay_Init(void)
{
		DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   /* Enable Cortex-M4's DWT CYCCNT reg.  */
		DWT_CYCCNT      = (unsigned int)0u;
		DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

/*
*********************************************************************************************************
*	�� �� ��: Delay_ms
*	����˵��: Ϊ���õײ������ڴ�RTOS�����������и��õļ�����
*             ר������һ������ʽ���ӳٺ������ڵײ�������ms�����ӳ���Ҫ���ڳ�ʼ����������Ӱ��ʵʱ�ԡ�
*	��    ��: n �ӳٳ��ȣ���λ1 ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay_ms(uint32_t _ulDelayTime)
{
		Delay_us(1000*_ulDelayTime);
}

/*
*********************************************************************************************************
*	�� �� ��: Delay_us
*	����˵��: �������ʱ����CPU���ڲ�����ʵ�֣�32λ������
*             	OSSchedLock(&err);
*				Delay_us(5);
*				OSSchedUnlock(&err); ����ʵ����������Ƿ���Ҫ�ӵ�������ѡ����ж�
*	��    ��: _ulDelayTime  �ӳٳ��ȣ���λ1 us
*	�� �� ֵ: ��
*   ˵    ��: 1. ��Ƶ168MHz������£�32λ������������2^32/168000000 = 25.565��
*                ����ʹ�ñ��������ӳٵĻ����ӳ���1�����¡�  
*             2. ʵ��ͨ��ʾ�������ԣ�΢���ӳٺ�����ʵ������ʵ�ʶ�����0.25us���ҵ�ʱ�䡣
*             �������ݲ���������
*             ��1��. MDK5.15���Ż��ȼ�0, ��ͬ��MDK�Ż��ȼ�����û��Ӱ�졣
*             ��2��. STM32F407IGT6
*             ��3��. ���Է�����
*				 GPIOI->BSRRL = GPIO_Pin_8;
*				 Delay_us(10);
*				 GPIOI->BSRRH = GPIO_Pin_8;
*             -------------------------------------------
*                ����              ʵ��ִ��
*             Delay_us(1)          1.2360us
*             Delay_us(2)          2.256us
*             Delay_us(3)          3.256us
*             Delay_us(4)          4.256us
*             Delay_us(5)          5.276us
*             Delay_us(6)          6.276us
*             Delay_us(7)          7.276us
*             Delay_us(8)          8.276us
*             Delay_us(9)          9.276us
*             Delay_us(10)         10.28us
*            3. ����32λ�޷������������ȡ�Ľ���ٸ�ֵ��32λ�޷�������Ȼ������ȷ�Ļ�ȡ��ֵ��
*              ����A,B,C����32λ�޷�������
*              ���A > B  ��ôA - B = C������ܺ���⣬��ȫû������
*              ���A < B  ��ôA - B = C�� C����ֵ����0xFFFFFFFF - B + A + 1����һ��Ҫ�ر�ע�⣬�������ڱ�������
*********************************************************************************************************
*/
void Delay_us(uint32_t _ulDelayTime)
{
		uint32_t tCnt, tDelayCnt;
		uint32_t tStart;

		tStart = DWT_CYCCNT;                                     /* �ս���ʱ�ļ�����ֵ */
		tCnt = 0;
		tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

		while(tCnt < tDelayCnt)
		{
				tCnt = DWT_CYCCNT - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
		}
}

