#include "stm32f103_spi.h"

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

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t temp = 0;

	temp |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		temp &=~ pSPIHandle->SPIConfig.SPI_BusConfig << SPI_CR1_BIDIMODE;
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		temp |= pSPIHandle->SPIConfig.SPI_BusConfig << SPI_CR1_BIDIMODE;
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX)
	{
		temp &=~ pSPIHandle->SPIConfig.SPI_BusConfig << SPI_CR1_BIDIMODE;
		temp |= pSPIHandle->SPIConfig.SPI_BusConfig << SPI_CR1_RXONLY;
	}

	temp |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;
	temp |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;
	temp |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;
	temp |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	pSPIHandle->pSPIx->CR1 = temp;
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			pSPIx->DR = *((uint16_t *)pTxBuffer);
			Len--;
			Len--;
			(uint16_t *)pTxBuffer++;
		}
		else
		{
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			*((uint16_t *)pTxBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pTxBuffer++;
		}
		else
		{
			*pTxBuffer = pSPIx->DR;
			Len--;
			pTxBuffer++;
		}
	}
}
