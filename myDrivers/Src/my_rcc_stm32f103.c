#include "my_rcc_stm32f103.h"

uint16_t AHB_Prescaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_Prescaler[4] = {2,4,8,16};
uint8_t APB2_Prescaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;
	uint8_t clksrc, temp, preAHB, preAPB1;

	clksrc = ((RCC->CFGR >> 2) & 0x03);

	if(clksrc == 0 || clksrc == 1)
	{
		SystemClk = 8000000;
	}

	temp = ((RCC->CFGR >> 4) & 0xF);

	if(temp < 8)
	{
		preAHB = 1;
	}else
	{
		preAHB = AHB_Prescaler[temp - 8];
	}

	temp = ((RCC->CFGR >> 10) & 0x7);

	if(temp < 4)
	{
		preAPB1 = 1;
	}else
	{
		preAPB1 = APB1_Prescaler[temp - 4];
	}

	pclk1 = (SystemClk/preAHB)/preAPB1;
	return pclk1;
}

uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk;
	uint8_t clksrc, temp, preAHB, preAPB2;

	clksrc = ((RCC->CFGR >> 2) & 0x03);

	if(clksrc == 0 || clksrc == 1)
	{
		SystemClk = 8000000;
	}

	temp = ((RCC->CFGR >> 4) & 0xF);

	if(temp < 8)
	{
		preAHB = 1;
	}else
	{
		preAHB = AHB_Prescaler[temp - 8];
	}

	temp = ((RCC->CFGR >> 13) & 0x7);

	if(temp < 4)
	{
		preAPB2 = 1;
	}else
	{
		preAPB2 = APB2_Prescaler[temp - 4];
	}

	pclk2 = (SystemClk/preAHB)/preAPB2;
	return pclk2;
}




