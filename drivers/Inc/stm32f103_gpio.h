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

/*
    GPIO_t options are:
    - pin: PINx, where x = 0 .. 15
    - mode: 1) MODE_INPUT
            2) MODE_OUTPUT_SPEED_10
            3) MODE_OUTPUT_SPEED_2
            4) MODE_OUTPUT_SPEED_50
    - configuration:
      for input mode:
            1) CONFIG_INPUT_ANALOG
            2) CONFIG_INPUT_FLOATING
            3) CONFIG_INPUT_PULL_UP_DOWN
      for output mode:
            1) CONFIG_OUTPUT_GENERAL_PUSH_PULL
            2) CONFIG_OUTPUT_GENERAL_OPEN_DRAIN
            3) CONFIG_OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL
            4) CONFIG_OUTPUT_ALTERNATE_FUNCTION_OPEN_DRAIN
    - port: PORTx where x = A B ... G
*/

#define CONFIG_INPUT_ANALOG									0x00000000 //MODE: 00, CNF: 00
#define CONFIG_INPUT_FLOATING								0x00000004 //MODE: 00, CNF: 01
#define CONFIG_INPUT_PULL_UP								0x00000010 //MODE: 00, CNF: 10, ODR: 1
#define CONFIG_INPUT_PULL_DOWN								0x00000020 //MODE: 00, CNF: 10, ODR: 0
#define CONFIG_INPUT_PULL_UP_PULL_DOWN						0x00000008 //MODE: 00, CNF: 10

#define CONFIG_OUTPUT_GENERAL_PUSH_PULL						0x00000000 //MODE: 00, CNF: 00
#define CONFIG_OUTPUT_GENERAL_OPEN_DRAIN 					0x00000004
#define CONFIG_OUTPUT_ALTERNAL_FUNCTION_OPEN_DRAIN 			0x0000000C
#define CONFIG_OUTPUT_ALTERNAL_FUNCTION_PUSH_PULL			0x00000008

#define MODE_INPUT											0x00000000 //MODE: 00
#define MODE_OUTPUT_SPEED_10								0x00000001 //MODE: 01
#define MODE_OUTPUT_SPEED_2									0x00000002 //MODE: 10
#define MODE_OUTPUT_SPEED_50								0x00000003 //MODE: 11

#define MODE_CONFIG_CLEAR 0x0000000F
#define MODE_CONFIG_SIZE 4

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


#endif
