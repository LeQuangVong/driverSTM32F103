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

void USART_Init(USART_Handle_t *pUSARTHandle)
{
	USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

	uint8_t temp = 0;

	//Mode

	if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		temp |= (1 << USART_CR1_TE);
	}else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		temp |= (1 << USART_CR1_RE);
	}else if(pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		temp |= (1 << USART_CR1_TE);
		temp |= (1 << USART_CR1_RE);
	}

	//Word length
	if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_8BITS)
	{
		temp &=~ (1 << USART_CR1_M);
	}else if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
	{
		temp |= (1 << USART_CR1_M);
	}

	//parity
	if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		temp |= (1 << USART_CR1_PCE);
		temp &=~ (1 << USART_CR1_PS);
	} else if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD)
	{
		temp |= (1 << USART_CR1_PCE);
		temp |= (1 << USART_CR1_PS);
	}

	pUSARTHandle->pUSARTx->CR1 = temp;

	temp = 0;
	//Stopbits
	temp |= pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP;
	pUSARTHandle->pUSARTx->CR2 = temp;

	temp = 0;
	if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		temp |= (1 << USART_CR3_CTSE);
	}else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		temp |= (1 << USART_CR3_RTSE);
	}else if(pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		temp |= (1 << USART_CR3_CTSE);
		temp |= (1 << USART_CR3_RTSE);
	}

	pUSARTHandle->pUSARTx->CR3 = temp;

	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}
