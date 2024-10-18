#include "stm32f103_gpio.h"

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

/*
	Description: Hàm này cài đặt mode và config các Pin của Port
*/

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);

	uint32_t modeConfig;
	uint32_t updatedPin;
	uint32_t pinLoopPosition, realPosition, currentPosition;
	uint32_t temp;

	uint8_t pullUpFlag = 0;
	uint32_t finalConfig = pGPIOHandle->GPIO_PinConfig.PinConfig;

	if(pGPIOHandle->GPIO_PinConfig.PinMode == MODE_INPUT && (pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP || pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_DOWN))
	{
		if(pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP)
		{
			pullUpFlag = 1;
		}
		finalConfig = CONFIG_INPUT_PULL_DOWN;
	}

	for(pinLoopPosition = 0x00; pinLoopPosition < 0x08; pinLoopPosition++)
	{
		realPosition = (0x01)<<pinLoopPosition;
		currentPosition = realPosition & pGPIOHandle->GPIO_PinConfig.PinNumber;

		if(currentPosition == realPosition)
		{
			temp = pGPIOHandle->pGPIOx->CRL & ~(MODE_CONFIG_CLEAR << (pinLoopPosition * MODE_CONFIG_SIZE));
			modeConfig = pGPIOHandle->GPIO_PinConfig.PinMode | finalConfig;
			temp |= modeConfig << (pinLoopPosition * MODE_CONFIG_SIZE);
			pGPIOHandle->pGPIOx->CRL = temp;
		}
	}

	for(pinLoopPosition = 0x08; pinLoopPosition < 0x10; pinLoopPosition++)
		{
			realPosition = (0x01)<<pinLoopPosition;
			currentPosition = realPosition & pGPIOHandle->GPIO_PinConfig.PinNumber;

			if(currentPosition == realPosition)
			{
				updatedPin = pinLoopPosition - 0x08;
				temp = pGPIOHandle->pGPIOx->CRH & ~(MODE_CONFIG_CLEAR << (updatedPin * MODE_CONFIG_SIZE));
				modeConfig = pGPIOHandle->GPIO_PinConfig.PinMode | finalConfig;
				temp |= modeConfig << (updatedPin * MODE_CONFIG_SIZE);
				pGPIOHandle->pGPIOx->CRH = temp;
			}
		}

	if(pullUpFlag == 1)
	{
		pullUpFlag = 0;
		pGPIOHandle->pGPIOx->BSRR = pGPIOHandle->GPIO_PinConfig.PinNumber;
	}
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	uint8_t value;
	value = (uint8_t)(pGPIOx->IDR >> pinNumber)& 0x00000001;
	return value;
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t value, uint8_t pinNumber)
{
	if(value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1<<pinNumber);
	}else
	{
		pGPIOx->ODR &=~ (1<<pinNumber);
	}
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t value)
{
	pGPIOx->ODR = value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	pGPIOx->ODR ^= (1<<pinNumber);
}

void PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t value, uint8_t pinNumber);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);
