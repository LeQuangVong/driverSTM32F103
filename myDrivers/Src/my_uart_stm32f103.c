#include "my_uart_stm32f103.h"
#include "my_rcc_stm32f103.h"

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

void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate)
{
	uint32_t PCLKx;
	float usartdiv;
	uint32_t M_part, F_part;

	uint32_t temp = 0;

	if(pUSARTx == USART1)
	{
		PCLKx = RCC_GetPCLK2Value();
	}else
	{
		PCLKx = RCC_GetPCLK1Value();
	}

	//sample

	usartdiv = (PCLKx/(16*BaudRate));

	M_part = (uint32_t)usartdiv;

	temp |= M_part << 4;

	F_part = usartdiv - M_part;
	F_part = (uint32_t)(F_part * 16);
	temp |= (F_part & 0xF);

	pUSARTx->BRR = temp;
}

void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	}else
	{
		pUSARTx->CR1 &=~ (1 << USART_CR1_UE);
	}
}

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
	uint8_t state = RESET;
	if(pUSARTx->SR & StatusFlagName)
	{
		state = SET;
	}

	return state;
}

void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint16_t Data;

	for(uint32_t i = 0; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			Data = *(uint16_t *)pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (Data & 0x01FF);

			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				pTxBuffer++;
			}
		}
		else
		{
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer & 0xFF);

			pTxBuffer++;
		}
	}

	while(!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}

void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	for(uint32_t i = 0; i < Len; i++)
	{
		while(! USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

		if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				*((uint16_t *)pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);

				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
				pRxBuffer++;
			}
		}
		else
		{
			if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
			}
			else
			{
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
			}

			pRxBuffer++;
		}
	}
}

void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint8_t StatusFlagName)
{
	pUSARTx->SR &=~ (StatusFlagName);
}

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
		}
	}
	else
	{
		if(IRQNumber <= 31)
		{
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t ipr = IRQNumber /4;
	uint8_t irq = IRQNumber %4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*irq + 4));
}

uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t TxState = pUSARTHandle->TxBusyState;

	if(TxState != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLen = Len;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}

	return TxState;
}

uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t RxState = pUSARTHandle->RxBusyState;

	if(RxState != USART_BUSY_IN_RX)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		(void)pUSARTHandle->pUSARTx->DR;

		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}
	return RxState;
}

void USART_IRQHandling(USART_Handle_t *pUSARTHandle)
{
	uint32_t temp1, temp2;
	uint16_t *pData;

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TC);

	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);

	if(temp1 && temp2)
	{
		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			if(! pUSARTHandle->TxLen)
			{
				pUSARTHandle->pUSARTx->SR &=~ (1 << USART_SR_TC);
				pUSARTHandle->pUSARTx->CR1 &=~ (1 << USART_CR1_TCIE);

				pUSARTHandle->TxBusyState = USART_READY;
				pUSARTHandle->TxLen = 0;
				pUSARTHandle->pTxBuffer = NULL;

				USART_ApplicationEventCallBack(pUSARTHandle, USART_EVENT_TX_CMPLT);
			}
		}
	}

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_TXE);

	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);

	if(temp1 && temp2)
	{
		if(pUSARTHandle->TxBusyState == USART_BUSY_IN_TX)
		{
			if(pUSARTHandle->TxLen > 0)
			{
				if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
				{
					pData = (uint16_t *)pUSARTHandle->pTxBuffer;
					pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t)0x01FF);

					if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
					{
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->pTxBuffer++;

						pUSARTHandle->TxLen--;
						pUSARTHandle->TxLen--;
					}
					else
					{
						pUSARTHandle->pTxBuffer++;
						pUSARTHandle->TxLen--;
					}
				}
				else
				{
					pUSARTHandle->pUSARTx->DR = (*pUSARTHandle->pTxBuffer & (uint8_t)0xFF);
					pUSARTHandle->pTxBuffer++;
					pUSARTHandle->TxLen--;
				}
			}

			if(pUSARTHandle->TxLen == 0)
			{
				pUSARTHandle->pUSARTx->CR1 &=~ (1 << USART_CR1_TXEIE);
			}
		}
	}

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_RXNE);

	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);

	if(temp1 && temp2)
	{
		if(pUSARTHandle->RxLen > 0)
		{
			if(pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
			{
				if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
				{
					*((uint16_t*)pUSARTHandle->pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
					pUSARTHandle->pRxBuffer++;
					pUSARTHandle->pRxBuffer++;

					pUSARTHandle->RxLen--;
					pUSARTHandle->RxLen--;
				}
				else
				{
					*pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);
					pUSARTHandle->pRxBuffer++;
					pUSARTHandle->RxLen--;
				}
			}
			else
			{
				if(pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
				{
					*pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0x0FF);
				}
				else
				{
					*pUSARTHandle->pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0x07F);
				}
				pUSARTHandle->pRxBuffer++;
				pUSARTHandle->RxLen--;
			}
		}

		if(pUSARTHandle->RxLen == 0)
		{
			pUSARTHandle->pUSARTx->CR1 &=~ (1 << USART_CR1_RXNEIE);
			pUSARTHandle->RxBusyState = USART_READY;
			USART_ApplicationEventCallBack(pUSARTHandle, USART_EVENT_RX_CMPLT);
		}
	}

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_CTS);

	temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_CTSE);

	if(temp1 && temp2)
	{
		pUSARTHandle->pUSARTx->SR &=~ (1 << USART_SR_CTS);

		USART_ApplicationEventCallBack(pUSARTHandle, USART_EVENT_CTS);
	}

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_IDLE);

	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_IDLEIE);

	if(temp1 && temp2)
	{
		pUSARTHandle->pUSARTx->SR &=~ (1 << USART_SR_IDLE);

		USART_ApplicationEventCallBack(pUSARTHandle, USART_EVENT_IDLE);
	}

	temp1 = pUSARTHandle->pUSARTx->SR & (1 << USART_SR_ORE);

	temp2 = pUSARTHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);

	if(temp1 && temp2)
	{
		USART_ApplicationEventCallBack(pUSARTHandle, USART_ERR_ORE);
	}


	temp2 = pUSARTHandle->pUSARTx->CR3 & (1 << USART_CR3_EIE);

	if(temp2)
	{
		temp1 = pUSARTHandle->pUSARTx->SR;

		if(temp1 & (1 << USART_SR_FE))
		{
			USART_ApplicationEventCallBack(pUSARTHandle, USART_ERR_FE);
		}

		if(temp1 & (1 << USART_SR_NE))
		{
			USART_ApplicationEventCallBack(pUSARTHandle, USART_ERR_NE);
		}

		if(temp1 & (1 << USART_SR_ORE))
		{
			USART_ApplicationEventCallBack(pUSARTHandle, USART_ERR_ORE);
		}
	}
}

__attribute__((weak)) void USART_ApplicationEventCallBack(USART_Handle_t *pUSARTHandle, uint8_t AppEv)
{

}



