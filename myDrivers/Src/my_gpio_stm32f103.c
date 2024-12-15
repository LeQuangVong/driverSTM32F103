#include "my_gpio_stm32f103.h"
/*
	Description: Hàm này Enable hoặc Disable các Port

*/
void PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
	}else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
	}
}

void PeriClockAFIOControl(AFIO_RegDef_t *pAFIO, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pAFIO == AFIO)
		{
			AFIO_PCLK_EN();
		}
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);//Enable Port

	uint32_t modeConfig = 0;
	uint8_t pinPosition = 0;
	uint32_t temp = 0;

	uint32_t finalConfig = pGPIOHandle->GPIO_PinConfig.PinConfig;//Lưu config vào biến

	/*
		Description: Kiểm tra xem mode phải là mode input hay không và thuộc pullup hay pulldown
	*/

	if(pGPIOHandle->GPIO_PinConfig.PinMode == MODE_INPUT && (pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP || pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_DOWN))
	{
		if(pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP)
		{
			pGPIOHandle->pGPIOx->ODR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_DOWN)
		{
			pGPIOHandle->pGPIOx->ODR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}
		//finalConfig = CONFIG_INPUT_PULL_UP_PULL_DOWN;
	}
	/*
	 * Description:Kiểm tra xem có yêu cầu ngắt hay không
	 	 	 	 - Nếu có thì Enable AFIO
	 	 	 	 - Nếu ngắt cạnh lên thì set bit tương ứng với chân yêu cầu ngắt vào thanh ghi EXTI_FTSR
	 	 	 	 - Nếu ngắt cạnh xuống thì set bit tương ứng với chân yêu cầu ngắt vào thanh ghi EXTI_RTSR
	 	 	 	 - Nếu ngắt cạnh lên và cạnh xuống thì set bit tương ứng với chân yêu cầu ngắt vào cả 2 thanh ghi
	 	 	 	 - Cấu hình ngắt với thanh ghi AFIO_EXTICR[x]

	*/
	if(pGPIOHandle->GPIO_PinConfig.PinMode == MODE_INPUT && (pGPIOHandle->GPIO_PinConfig.Interrupt == IT_FALLING_EDGE || pGPIOHandle->GPIO_PinConfig.Interrupt == IT_RISING_EDGE
																|| pGPIOHandle->GPIO_PinConfig.Interrupt == IT_FALLING_AND_RISING_EDGE))
	{
		PeriClockAFIOControl(pGPIOHandle->pAFIO,ENABLE);//Enable AFIO

		if(pGPIOHandle->GPIO_PinConfig.Interrupt == IT_FALLING_EDGE)
		{
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.Interrupt == IT_RISING_EDGE)
		{
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.Interrupt == IT_FALLING_AND_RISING_EDGE)
		{
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}

		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.PinNumber / 4;//tìm số thanh ghi EXTICR tương ứng với số chân
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.PinNumber % 4;//tìm số Pin trong thanh ghi EXTICR
		uint8_t code = GPIO_BASSADDR_TO_CODE(pGPIOHandle->pGPIOx);//giá trị tương ứng với Port
		AFIO->EXTICR[temp1] = code << (temp2*4);
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.PinNumber;
	}

	/*
		Description: Cấu hình cho thanh ghi GPIOx_CRL, Port x configuration bits (y= 0 .. 7)
	*/

	if(pGPIOHandle->GPIO_PinConfig.PinNumber < PIN8)
	{
		pinPosition = pGPIOHandle->GPIO_PinConfig.PinNumber;
		temp = pGPIOHandle->pGPIOx->CRL & ~(MODE_CONFIG_CLEAR << (pinPosition * MODE_CONFIG_SIZE));
		modeConfig = pGPIOHandle->GPIO_PinConfig.PinMode | finalConfig;
		temp |= modeConfig << (pinPosition * MODE_CONFIG_SIZE);
		pGPIOHandle->pGPIOx->CRL = temp;
	}
	else
	{
		pinPosition = pGPIOHandle->GPIO_PinConfig.PinNumber - PIN8;
		temp = pGPIOHandle->pGPIOx->CRH & ~(MODE_CONFIG_CLEAR << (pinPosition * MODE_CONFIG_SIZE));
		pGPIOHandle->pGPIOx->CRH = temp;
		modeConfig = pGPIOHandle->GPIO_PinConfig.PinMode | finalConfig;
		temp |= modeConfig << (pinPosition * MODE_CONFIG_SIZE);
		pGPIOHandle->pGPIOx->CRH = temp;
	}

	/*
		Description: Cấu hình cho thanh ghi GPIOx_CRH, Port x configuration bits (y= 8 .. 15)
	*/
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	pGPIOx->ODR ^= (1 << pinNumber);
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

/*
 * Description: Thiết lập mức độ ưu tiên cho ngắt ngoài
 */

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t ipr = IRQNumber /4;
	uint8_t irq = IRQNumber %4;
	*(NVIC_PR_BASE_ADDR + ipr) |= (IRQPriority << (8*irq + 4));
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	if(EXTI->PR & (1 << PinNumber))
	{
		EXTI->PR |= (1 << PinNumber);
	}
}
