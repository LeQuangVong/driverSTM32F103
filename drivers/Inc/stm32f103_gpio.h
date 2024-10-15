#ifndef INC_STM32F103_GPIO_H_
#define INC_STM32F103_GPIO_H_

#include "stm32f103.h"

typedef struct{
	uint8_t PinNumber;
	uint8_t PinMode;
	uint8_t PinConfig;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;


#endif
