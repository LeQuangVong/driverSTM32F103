#include "my_i2c_stm32f103.h"
#include "my_rcc_stm32f103.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhaseWrite(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr &= ~(1);
	pI2Cx->DR = SlaveAddr;
}

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummy_read;
	//check device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		//master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}
		else
		{
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;
		}
	}
	else
	{
		//slave mode
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}

static void I2C_ExecuteAddressPhaseRead(I2C_RegDef_t *pI2Cx, uint8_t SlaveAddr)
{
	SlaveAddr = SlaveAddr << 1;
	SlaveAddr |= 1;
	pI2Cx->DR = SlaveAddr;
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 0)
	{
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);
		pI2CHandle->TxLen--;
		pI2CHandle->pTxBuffer++;
	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->RxLen == 1)
	{
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxLen > 1)
	{
		if(pI2CHandle->RxLen == 2)
		{
			I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
		}
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLen--;
	}

	if(pI2CHandle->RxLen == 0)
	{
		if(pI2CHandle->Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

		I2C_CloseReceiveData(pI2CHandle);

		I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_RX_CMPLT);
	}
}
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}else if(pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
	}
}

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}
	else
	{
		pI2Cx->CR1 &=~ (1 << I2C_CR1_PE);
	}
}

void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp = 0;

	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	//ACK
	temp |= pI2CHandle->I2C_Config.I2C_AckControl << 10;
	pI2CHandle->pI2Cx->CR1 = temp;

	//FREQ CR2
	temp = 0;
	temp |= RCC_GetPCLK1Value()/1000000U;
	pI2CHandle->pI2Cx->CR2 = (temp & 0x3F);

	//Address
	temp = 0;
	temp |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	temp |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = temp;

	//CCR
	temp = 0;
	uint16_t crr_value = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM)
	{
		crr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
	}
	else
	{
		temp |= (1 << 15);
		temp |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);

		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			crr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		else
		{
			crr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
		}
		temp |= (crr_value & 0xFFF);
	}

	pI2CHandle->pI2Cx->CCR = temp;

	//TRISE
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed == I2C_SCL_SPEED_SM)
	{
		temp = (RCC_GetPCLK1Value() / 1000000U) + 1;//số chu kỳ của PCLK1 tương ứng với 1 µs.
	}
	else
	{
		temp = ((RCC_GetPCLK1Value() * 300)/ 1000000U) + 1;//số chu kỳ clock của PCLK1 tương ứng với thời gian tăng tối đa là 300 ns
	}

	pI2CHandle->pI2Cx->TRISE = (temp & 0x3F);
}

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint8_t StatusFlagName)
{
	if(pI2Cx->SR1 & StatusFlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}
	else
	{
		pI2Cx->CR1 &=~ (1 << I2C_CR1_ACK);
	}
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	//Start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	//check SB Flag SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));
	//send addr and w/r bit
	I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, SlaveAddr);
	//Check Addr Flag SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));
	//Clear Addr Flag, SCL low
	I2C_ClearADDRFlag(pI2CHandle);
	//send data until len becomes 0
	while(Len > 0)
	{
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Len--;
	}

	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	if(Sr == I2C_DISABLE_SR)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	//Start condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);
	//check SB Flag SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));
	//send addr and w/r bit
	I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, SlaveAddr);
	//Check Addr Flag SR1
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	if(Len == 1)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
		I2C_ClearADDRFlag(pI2CHandle);
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		if(Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}

		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	if(Len > 1)
	{
		I2C_ClearADDRFlag(pI2CHandle);

		for(uint32_t i = Len; i > 0; i--)
		{
			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(i == 2)
			{
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
				if(Sr == I2C_DISABLE_SR)
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}
			}

			*pRxBuffer = pI2CHandle->pI2Cx->DR;
			pRxBuffer++;
		}
	}

	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}
}

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t ipr = IRQNumber /4;
	uint8_t irq = IRQNumber %4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*irq + 4));
}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;
	if((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}
	return busystate;
}

uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;
	if((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}
	return busystate;
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &=~ (1 << I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &=~ (1 << I2C_CR2_ITEVTEN);

	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
	pI2CHandle->TxRxState = I2C_READY;
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->pI2Cx->CR2 &=~ (1 << I2C_CR2_ITBUFEN);
	pI2CHandle->pI2Cx->CR2 &=~ (1 << I2C_CR2_ITEVTEN);

	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->TxLen = 0;
	pI2CHandle->RxSize = 0;
	pI2CHandle->TxRxState = I2C_READY;

	if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1, temp2, temp3;
	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);

	//SB flag
	if(temp1 && temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);

	if(temp1 && temp3)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);

	if(temp1 && temp3)
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE))
			{
				if(pI2CHandle->TxLen == 0)
				{
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

					I2C_CloseSendData(pI2CHandle);

					I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_TX_CMPLT);
				}
			}
		}
		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE))
			{
				if(pI2CHandle->RxLen == 0)
				{
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

					I2C_CloseReceiveData(pI2CHandle);

					I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_RX_CMPLTX);
				}
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);

	if(temp1 && temp3)
	{
		pI2CHandle->pI2Cx->CR1 |= 0x0000;
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_STOP);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TxE);

	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}else
		{
			if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
			{
				I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RxNE);

	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}else
		{
			if(! (pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}

}

void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1, temp2;
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITERREN);
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BERR);

	if(temp1 && temp2)
	{
		pI2CHandle->pI2Cx->SR1 &=~ (1 << I2C_SR1_BERR);
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_BERR);

	}

	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ARLO);

	if(temp1 && temp2)
	{
		pI2CHandle->pI2Cx->SR1 &=~ (1 << I2C_SR1_ARLO);
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_ARLO);
	}

	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_AF);

	if(temp1 && temp2)
	{
		pI2CHandle->pI2Cx->SR1 &=~ (1 << I2C_SR1_AF);
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_AF);
	}

	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_OVR);

	if(temp1 && temp2)
	{
		uint32_t temp = I2C1->SR1;
		(void)temp = I2C1->DR;
		pI2CHandle->pI2Cx->SR1 &=~ (1 << I2C_SR1_OVR);
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_OVR);
	}

	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TIMEOUT);

	if(temp1 && temp2)
	{
		pI2CHandle->pI2Cx->SR1 &=~ (1 << I2C_SR1_TIMEOUT);
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_TIMEOUT);
	}
}

__attribute__((weak)) void I2C_ApplicationEventCallBack(I2C_Handle_t *pUSARTHandle, uint8_t AppEv)
{

}



