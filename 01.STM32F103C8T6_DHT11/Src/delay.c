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
*                                             寄存器
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
*	函 数 名: bsp_InitDWT
*	功能说明: 初始化DWT. 该函数被 bsp_Init() 调用。
*	形    参: 无
*	返 回 值: 无
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
*	函 数 名: Delay_ms
*	功能说明: 为了让底层驱动在带RTOS和裸机情况下有更好的兼容性
*             专门制作一个阻塞式的延迟函数，在底层驱动中ms毫秒延迟主要用于初始化，并不会影响实时性。
*	形    参: n 延迟长度，单位1 ms
*	返 回 值: 无
*********************************************************************************************************
*/
void Delay_ms(uint32_t _ulDelayTime)
{
		Delay_us(1000*_ulDelayTime);
}

/*
*********************************************************************************************************
*	函 数 名: Delay_us
*	功能说明: 这里的延时采用CPU的内部计数实现，32位计数器
*             	OSSchedLock(&err);
*				Delay_us(5);
*				OSSchedUnlock(&err); 根据实际情况看看是否需要加调度锁或选择关中断
*	形    参: _ulDelayTime  延迟长度，单位1 us
*	返 回 值: 无
*   说    明: 1. 主频168MHz的情况下，32位计数器计满是2^32/168000000 = 25.565秒
*                建议使用本函数做延迟的话，延迟在1秒以下。  
*             2. 实际通过示波器测试，微妙延迟函数比实际设置实际多运行0.25us左右的时间。
*             下面数据测试条件：
*             （1）. MDK5.15，优化等级0, 不同的MDK优化等级对其没有影响。
*             （2）. STM32F407IGT6
*             （3）. 测试方法：
*				 GPIOI->BSRRL = GPIO_Pin_8;
*				 Delay_us(10);
*				 GPIOI->BSRRH = GPIO_Pin_8;
*             -------------------------------------------
*                测试              实际执行
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
*            3. 两个32位无符号数相减，获取的结果再赋值给32位无符号数依然可以正确的获取差值。
*              假如A,B,C都是32位无符号数。
*              如果A > B  那么A - B = C，这个很好理解，完全没有问题
*              如果A < B  那么A - B = C， C的数值就是0xFFFFFFFF - B + A + 1。这一点要特别注意，正好用于本函数。
*********************************************************************************************************
*/
void Delay_us(uint32_t _ulDelayTime)
{
		uint32_t tCnt, tDelayCnt;
		uint32_t tStart;

		tStart = DWT_CYCCNT;                                     /* 刚进入时的计数器值 */
		tCnt = 0;
		tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* 需要的节拍数 */ 		      

		while(tCnt < tDelayCnt)
		{
				tCnt = DWT_CYCCNT - tStart; /* 求减过程中，如果发生第一次32位计数器重新计数，依然可以正确计算 */	
		}
}

