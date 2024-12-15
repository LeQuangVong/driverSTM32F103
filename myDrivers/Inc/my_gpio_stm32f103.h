#ifndef INC_MY_GPIO_STM32F103_H_
#define INC_MY_GPIO_STM32F103_H_

#include "my_stm32f103.h"

typedef struct{
	uint32_t PinNumber;
	uint8_t PinMode;
	uint8_t PinConfig;
	uint8_t Interrupt;
}GPIO_PinConfig_t;

typedef struct{
	GPIO_RegDef_t *pGPIOx;
	AFIO_RegDef_t *pAFIO;
	GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

#define PIN0 	0
#define PIN1 	1
#define PIN2 	2
#define PIN3 	3
#define PIN4 	4
#define PIN5 	5
#define PIN6 	6
#define PIN7 	7
#define PIN8 	8
#define PIN9 	9
#define PIN10 	10
#define PIN11 	11
#define PIN12 	12
#define PIN13 	13
#define PIN14 	14
#define PIN15 	15


#define CONFIG_INPUT_ANALOG								0x00000000 //MODE: 00, CNF: 00
#define CONFIG_INPUT_FLOATING							      0x00000004 //MODE: 00, CNF: 01
#define CONFIG_INPUT_PULL_UP							      0x00000008 //MODE: 00, CNF: 10, ODR: 1
#define CONFIG_INPUT_PULL_DOWN							0x00000008 //MODE: 00, CNF: 10, ODR: 0
//#define CONFIG_INPUT_PULL_UP_PULL_DOWN					0x00000008 //MODE: 00, CNF: 10

#define CONFIG_OUTPUT_GENERAL_PUSH_PULL						0x00000000 //MODE: 00, CNF: 00
#define CONFIG_OUTPUT_GENERAL_OPEN_DRAIN 					0x00000004 //MODE: 00, CNF: 01
#define CONFIG_OUTPUT_ALTERNAL_FUNCTION_OPEN_DRAIN 			0x0000000C //MODE: 00, CNF: 11
#define CONFIG_OUTPUT_ALTERNAL_FUNCTION_PUSH_PULL			0x00000008 //MODE: 00, CNF: 10

#define MODE_INPUT									0x00000000 //MODE: 00
#define MODE_OUTPUT_SPEED_10						0x00000001 //MODE: 01
#define MODE_OUTPUT_SPEED_2							0x00000002 //MODE: 10
#define MODE_OUTPUT_SPEED_50						0x00000003 //MODE: 11

#define MODE_CONFIG_CLEAR 0xF
#define MODE_CONFIG_SIZE 4

#define IT_FALLING_EDGE									0x00000005
#define IT_RISING_EDGE									0x00000006
#define IT_FALLING_AND_RISING_EDGE							0x00000007

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

#endif /* INC_MY_GPIO_STM32F103_H_ */
