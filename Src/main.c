/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "my_stm32f103.h"
#include "my_gpio_stm32f103.h"

void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

int a = 0;

int main(void)
{
	GPIO_Handle_t GPIOLed, GPIOBtn;
	GPIOLed.pGPIOx = GPIOC;
	GPIOLed.GPIO_PinConfig.PinNumber = PIN13;
	GPIOLed.GPIO_PinConfig.PinConfig = CONFIG_OUTPUT_GENERAL_PUSH_PULL;
	GPIOLed.GPIO_PinConfig.PinMode = MODE_OUTPUT_SPEED_50;

	GPIO_Init(&GPIOLed);

	GPIOBtn.pGPIOx = GPIOB;
	GPIOBtn.pAFIO = AFIO;
	GPIOBtn.GPIO_PinConfig.PinNumber = PIN11;
	GPIOBtn.GPIO_PinConfig.PinConfig = CONFIG_INPUT_PULL_UP;
	GPIOBtn.GPIO_PinConfig.PinMode = MODE_INPUT;
	GPIOBtn.GPIO_PinConfig.Interrupt = IT_FALLING_EDGE;

	GPIO_Init(&GPIOBtn);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 3);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

	while(1)
	{
		//GPIO_ToggleOutputPin(GPIOC, PIN13);
		//delay();
	}

	return 0;
}


void EXTI15_10_IRQHandler()
{
	delay();
	GPIO_IRQHandling(PIN11);
	GPIO_ToggleOutputPin(GPIOC, PIN13);
	a++;
}

