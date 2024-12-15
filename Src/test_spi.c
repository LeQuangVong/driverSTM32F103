#include "my_stm32f103.h"
#include "my_spi_stm32f103.h"
#include "my_gpio_stm32f103.h"
#include "string.h"
/*
 * PA4 -> NSS
 * PA5 -> SCK
 * PA6 -> MISO
 * PA7 -> MOSI
 */
void SPI_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOA;
	SPIPins.GPIO_PinConfig.PinNumber = PIN4;
	SPIPins.GPIO_PinConfig.PinConfig = CONFIG_OUTPUT_ALTERNAL_FUNCTION_PUSH_PULL;
	SPIPins.GPIO_PinConfig.PinMode = MODE_OUTPUT_SPEED_50;
	//NSS
	GPIO_Init(&SPIPins);

	//SCK
	SPIPins.GPIO_PinConfig.PinNumber = PIN5;
	GPIO_Init(&SPIPins);

	//MISO
	SPIPins.GPIO_PinConfig.PinNumber = PIN6;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.PinNumber = PIN7;
	GPIO_Init(&SPIPins);
}

void SPI1_Init(void)
{
	 SPI_Handle_t SPI1Handle;
	 SPI1Handle.pSPIx = SPI1;
	 SPI1Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	 SPI1Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	 SPI1Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	 SPI1Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	 SPI1Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	 SPI1Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	 SPI1Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	 SPI_Init(&SPI1Handle);
}
int main()
{
	char user_data[] = "Hello";
	SPI_GPIOInits();
	SPI1_Init();
	SPI_SSIConfig(SPI1, ENABLE);
	SPI_PeripheralControl(SPI1, ENABLE);
	SPI_SendData(SPI1,(uint8_t *)user_data, strlen(user_data));
	while(SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));
	SPI_PeripheralControl(SPI1, DISABLE);
	while(1);
	return 0;
}


