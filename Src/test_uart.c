#include "my_stm32f103.h"
#include "my_gpio_stm32f103.h"
#include "my_uart_stm32f103.h"
#include <string.h>

char test[1024] = "Hello";

USART_Handle_t usart1_handle;

void USART1_Init(void)
{
	usart1_handle.pUSARTx = USART1;
	usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart1_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart1_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart1_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart1_handle);
}

/*
 * PA9 - TX
 * PA10 - RX
 */
void USART1_GPIOInit(void)
{
	GPIO_Handle_t USARTPins;

	USARTPins.pGPIOx = GPIOA;
	USARTPins.GPIO_PinConfig.PinNumber = PIN9;
	USARTPins.GPIO_PinConfig.PinConfig = CONFIG_OUTPUT_ALTERNAL_FUNCTION_PUSH_PULL;
	USARTPins.GPIO_PinConfig.PinMode = MODE_OUTPUT_SPEED_50;
	//TX
	GPIO_Init(&USARTPins);

	//RX
	USARTPins.GPIO_PinConfig.PinNumber = PIN10;
	GPIO_Init(&USARTPins);
}
int main()
{
	USART1_GPIOInit();
	USART1_Init();
	USART_PeripheralControl(USART1, ENABLE);

	while(1)
	{
		USART_SendData(&usart1_handle, (uint8_t*)test, strlen(test));
		while(1);
	}

	return 0;
}


