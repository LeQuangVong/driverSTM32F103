#include "my_spi_stm32f103.h"

static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pSPIHandle);
static void SPI_OVR_ERR_Interrupt_Handle(SPI_Handle_t *pSPIHandle);

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
	}else
	{

	}
}

/*
	Description: Configuring the SPI
*/

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	//Enable clock SPI
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t temp = 0;
	//device mode: Master mode: MASTR = 1, Slave mode: MSTR = 0
	if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_MASTER)
	{
		temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;
		//serial clock speed (baud rate)
		temp |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
		//Software Slave Management (SSM)
		if(pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN)
		{
			temp |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;
		}
		else
		{
			temp &= ~(pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);
		}
	}
	else if(pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_DEVICE_MODE_SLAVE)
	{
		temp &= ~(pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);
		temp &= ~(pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);
	}

	//bus
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//Full-duplex: BIDIMODE = 0
		temp &=~ (1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//Half-duplex: BIDIMODE = 1
		temp |= (1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX)
	{
		//Simplex receive only: BIDIMODE = 0, RXONLY = 1
		temp &=~ (1 << SPI_CR1_BIDIMODE);
		temp |= (1 << SPI_CR1_RXONLY);
	}

	//Data frame format (DFF): 8bits: DFF = 0, 16bits: DFF = 1
	temp |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

	//Clock polarity (CPOL)
	temp |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//Clock phase (CPHA)
	temp |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = temp;
}

/*
	Description: The SPI_GetFlagStatus function is used to check the status of a specific flag
				in the SPI Status Register (SR)

				Example: SPI_TXE_FLAG (1 << SPI_SR_TXE) 0b00000010
						 SR:							0b00000010
						 => FLAG_SET
*/

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
	Description: The SPI_SendData function sends a data stream from the transmit buffer (TxBuffer)
				 to the Data Register (DR) until the data transfer is complete (Len < 0)

				 when Len > 0 and SPI_TXE_FLAG = 1 (Tx buffer empty), the data will start transfer
				 with 16bits frame format or 8bits DFF.
*/

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//wait until TXE Flag is set (TXE = 1)
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		//16bits: DFF = 1
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			//access to 16bits(2 byte) data at the memory address pTxBuffer points to.
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;//8bit
			Len--;//8bit
			(uint16_t *)pTxBuffer++;//2 bytes
		}
		else
		{
			//8bits DFF = 0
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

/*
	Description: The SPI_ReceiveData function works for both 8bit or 16 bit data frames
				read data from the SPI Data Register (DR) and save it into the specific buffer
				until all data has been received.
				when Len > 0 and SPI_RXNE_FLAG = 1 (Rx buffer not empty), this flag indicates whether
				there is data available to read from the SPI Data register (DR).
*/

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			*((uint16_t *)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRxBuffer++;
		}
		else
		{
			*pRxBuffer = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
	}
}

/*
	Description: Enable or Disable SPI, Peripheral disable: SPE = 0, Peripheral enable: SPE = 1.
*/

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &=~ (1 << SPI_CR1_SPE);
	}
}

/*
	Description: Enable or Disable SSI (Slave Select Input)
*/

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		 pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &=~ (1 << SPI_CR1_SSI);
	}
}

void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t ipr = IRQNumber /4;
	uint8_t irq = IRQNumber %4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*irq + 4));
}

/*
	Description: The SPI_SentDataIT function is send data over SPI using interrupt.
				 Check the SPI is currently busy in transmission (SPI_BUSY_IN_TX).
				 if it is not busy, set up transmission
				 Enable interrupt request (TXEIE = 1)
*/

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX)
	{
		pSPIHandle->pTxBuffer = pTxBuffer;//store the address of data transmitted
		pSPIHandle->TxLen = Len;//store length
		pSPIHandle->TxState = SPI_BUSY_IN_TX;//update the state
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);//enable interrupt request
	}
	return state;
}

/*
	Description: The SPI_ReceiveDataIT function prepares the SPI for data reception using interrupt.
				 Check the SPI is currently busy in reception (SPI_BUSY_IN_RX).
				 if it is not busy, set up reception
				 Enable interrupt request (RXNEIE = 1)
*/

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}
	return state;
}

/*
	Description: Interrupt event					Event flag				Enable control bit
				 Transmit buffer empty flag				TXE						TXEIE
				 Receive buffer not empty flag			RXNE					RXNEIE
				 Overrun Error							OVR						ERRIE
*/

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;
	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_TXE);//update Event flag

	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);//update Enable control bit

	if(temp1 && temp2)
	{
		SPI_TXE_Interrupt_Handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		SPI_RXNE_Interrupt_Handle(pSPIHandle);
	}

	temp1 = pSPIHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		SPI_OVR_ERR_Interrupt_Handle(pSPIHandle);
	}
}

/*
	Description: The SPI_TXE_Interrupt_Handle function handle the SPI transmit buffer empty (TXE) interrupt
				 When all data has been sent, it clean up and trigger a callback to notify the application
				 that the transmission complete (SPI_EVENT_TX_CMPLT)
*/

static void SPI_TXE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen --;
		pSPIHandle->TxLen --;
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen --;
		pSPIHandle->pTxBuffer++;
	}

	if(!pSPIHandle->TxLen)
	{
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
}

static void SPI_RXNE_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
	{
		*((uint16_t *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen --;
		pSPIHandle->RxLen --;
		(uint16_t *)pSPIHandle->pRxBuffer++;
	}
	else
	{
		*(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen --;
		pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen)
	{
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}

/*
 * Description: The SPI_CloseTransmission function is clean up and complete an SPI transmission,
 	 	 	 	resetting the state of the SPI handle to make it ready for future transmission.
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &=~ (1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &=~ (1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

/*
 * Description: The SPI_OVR_ERR_Interrupt_Handle function handles the overrun error (OVR) that can occur in an SPI
 * 				An overrun error happens when new data arrives in the SPI Data register (DR) before
 * 				the previous data is read, leading to a data loss.
 */
static void SPI_OVR_ERR_Interrupt_Handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
		(void)temp;//prevent compiler warning about temp being unused
	}
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{

}



