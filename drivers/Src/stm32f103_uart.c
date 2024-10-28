#include "stm32f103_uart.h"

void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_PCLK_EN();
		}
		else if(pUSARTx == USART2)
		{
			USART2_PCLK_EN();
		} else if(pUSARTx == USART3)
		{
			USART3_PCLK_EN();
		} else if(pUSARTx == UART4)
		{
			UART4_PCLK_EN();
		} else if(pUSARTx == UART5)
		{
			UART5_PCLK_EN();
		}
	}
}


