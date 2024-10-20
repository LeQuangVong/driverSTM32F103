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
	PeriClockControl(pGPIOHandle->pGPIOx,ENABLE);//Enable Port

	uint32_t modeConfig;
	uint32_t updatedPin;
	uint32_t pinLoopPosition, realPosition, currentPosition;
	uint32_t temp;

	uint32_t finalConfig = pGPIOHandle->GPIO_PinConfig.PinConfig;//Lưu config vào biến

	/*
		Description: Kiểm tra xem mode phải là mode input hay không và thuộc pullup hay pulldown
					- Nếu pullup thì set bit tương ứng với chân lên bằng 1
					- Nếu là pulldown thì reset bit tương ứng với chân bằng 0.
	*/

	if(pGPIOHandle->GPIO_PinConfig.PinMode == MODE_INPUT && (pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP || pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_DOWN))
	{
		if(pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_UP)
		{
			//pullUpFlag = 1;
			pGPIOHandle->pGPIOx->ODR |= (1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}else if(pGPIOHandle->GPIO_PinConfig.PinConfig == CONFIG_INPUT_PULL_DOWN)
		{
			pGPIOHandle->pGPIOx->ODR &= ~(1 << pGPIOHandle->GPIO_PinConfig.PinNumber);
		}
		finalConfig = CONFIG_INPUT_PULL_UP_PULL_DOWN;
	}

	/*
			Description: Cấu hình cho thanh ghi GPIOx_CRL, Port x configuration bits (y= 0 .. 7)
						- lặp từ 0->7 tương ứng với 8 pin: 1pin có 4 bits
						- Tìm vị trí của vòng lặp tại pin nào
						- Tìm vị trí hiện tại của pin đang cài đặt
						- nếu vị trí của vòng lặp bằng vị trí hiện tại của pin đang cài đặt
							+ Xóa cấu hình hiện tại của chân
							+ Thiết lập cấu hình mới
							+ cập nhật thanh ghi GPIOx_CRL

			Example: realPosition = 0x04  (0x01 << 2): 0x0100
					pGPIOHandle->GPIO_PinConfig.PinNumber = 2 (Pin_2): 0x0100
					currentPosition = 0x0100 : realPosition & 0x0100 = 0x0100

					GPIOx_CRL: 0000 0000 0000 0000 0000 0000 0000 0000
					GPIO_PinConfig.PinMode = MODE_INPUT: 0x00
					finalConfig = 0x10 ==> modeConfig = 0x10 | 0x00 = 0x10
					temp = 0x100000000000;
					GPIOx_CRL: 0000 0000 0000 0000 0000 1000 0000 0000
	*/

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

	/*
				Description: Cấu hình cho thanh ghi GPIOx_CRH, Port x configuration bits (y= 8 .. 15)
							- lặp từ 8->15 tương ứng với 8 pin: 1pin có 4 bits
							- Tìm vị trí của vòng lặp tại pin nào
							- Tìm vị trí hiện tại của pin đang cài đặt
							- nếu vị trí của vòng lặp bằng vị trí hiện tại của pin đang cài đặt
								+ Xóa cấu hình hiện tại của chân
								+ Thiết lập cấu hình mới
								+ cập nhật thanh ghi GPIOx_CRH

				Example: realPosition = 0x04  (0x01 << 10): 0x10000000000
						pGPIOHandle->GPIO_PinConfig.PinNumber = 10 (Pin_10): 0x10000000000
						currentPosition = 0x10000000000 : realPosition & 0x10000000000 = 0x10000000000

						GPIOx_CRH: 0000 0000 0000 0000 0000 0000 0000 0000
						GPIO_PinConfig.PinMode = MODE_INPUT: 0x00
						finalConfig = 0x10 ==> modeConfig = 0x10 | 0x00 = 0x10
						temp = 0x100000000000;
						GPIOx_CRH: 0000 0000 0000 0000 0000 1000 0000 0000
		*/

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

/*
 * Description: Đọc trạng thái của các chân
 * Example: pinNumber = 5
			pGPIOx->IDR = 0x0000000000100000
			pGPIOx->IDR >> 5 = 0x01 & 0x00000001 = 1
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber)
{
	uint8_t value;
	value = (uint8_t)(pGPIOx->IDR >> pinNumber)& 0x00000001;
	return value;
}

/*
 * Description: Đọc trạng thái của Port
 * Example: pinNumber = 5
			pGPIOx->IDR = 0x0000000000100000
 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

/*
 * Description: Viết giá trị vào pin
				+ Nếu value == 1, thì set bit tương ứng với chân trong thanh ghi ODR
				+ Ngược lại thì reset bit tương ứng
 */

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

/*
 * Description: Viết giá trị vào Port
				+ Nếu bit nào trong value ở low hoặc high thì sẽ reset hoặc set bit ứng với chân
 */

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
