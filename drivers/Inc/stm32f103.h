#ifndef INC_STM32F103XX_H_
#define INC_STM32F103XX_H_

#include <stdint.h>

#define __vo volatile

#define FLASH_BASEADDR 0x08000000U
#define ROM_BASEADDR   0x1FFFF000U
#define SRAM_BASEADDR  0x20000000U

#define PERI_BASEADDR  0x40000000U
#define APB1_BASEADDR  PERI_BASEADDR
#define APB2_BASEADDR  0x40010000U
#define AHB_BASEADDR   0x40018000U
#define AFIO_BASEADDR  APB2_BASEADDR
#define RCC_BASEADDR   (AHB_BASEADDR + 0x1000)

#define EXTI_BASEADDR  (APB2_BASEADDR + 0x0400)

#define GPIOA_BASEADDR (APB2_BASEADDR + 0x0800)
#define GPIOB_BASEADDR (APB2_BASEADDR + 0x0C00)
#define GPIOC_BASEADDR (APB2_BASEADDR + 0x1000)
#define GPIOD_BASEADDR (APB2_BASEADDR + 0x1400)
#define GPIOE_BASEADDR (APB2_BASEADDR + 0x1800)
#define GPIOF_BASEADDR (APB2_BASEADDR + 0x1C00)
#define GPIOG_BASEADDR (APB2_BASEADDR + 0x2000)

typedef struct{
	__vo uint32_t CRL;
	__vo uint32_t CRH;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t BRR;
	__vo uint32_t LCKR;
}GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF ((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG ((GPIO_RegDef_t *)GPIOG_BASEADDR)

typedef struct{
	__vo uint32_t EVCR;
	__vo uint32_t MAPR;
	__vo uint32_t EXTICR[4];
	__vo uint32_t MAPR2;
}AFIO_RegDef_t;

#define AFIO ((AFIO_RegDef_t *)AFIO_BASEADDR)

typedef struct{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;

#define EXTI ((EXTI_RegDef_t *)EXTI_BASEADDR)

typedef struct{
	__vo uint32_t CR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t APB1RSTR;
	__vo uint32_t AHBENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t APB1ENR;
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	__vo uint32_t AHBSTR;
	__vo uint32_t CFGR2;
}RCC_RegDef_t;

#define RCC ((RCC_RegDef_t *)RCC_BASEADDR)


#define GPIOA_PCLK_EN() (RCC->APB2ENR |= (1<<2))
#define GPIOB_PCLK_EN() (RCC->APB2ENR |= (1<<3))
#define GPIOC_PCLK_EN() (RCC->APB2ENR |= (1<<4))
#define GPIOD_PCLK_EN() (RCC->APB2ENR |= (1<<5))
#define GPIOE_PCLK_EN() (RCC->APB2ENR |= (1<<6))

#define AFIO_PCLK_EN() (RCC->APB2ENR |= (1<<0))

#define GPIOA_PCLK_DI() (RCC->APB2RSTR &=~ (1<<2))
#define GPIOB_PCLK_DI() (RCC->APB2RSTR &=~ (1<<3))
#define GPIOC_PCLK_DI() (RCC->APB2RSTR &=~ (1<<4))
#define GPIOD_PCLK_DI() (RCC->APB2RSTR &=~ (1<<5))
#define GPIOE_PCLK_DI() (RCC->APB2RSTR &=~ (1<<6))

#define GPIOA_REG_RESET()		do{	(RCC->APB2RSTR |= (1<<2)); (RCC->APB2RSTR &= ~(1<<2)); }while(0)
#define GPIOB_REG_RESET()		do{	(RCC->APB2RSTR |= (1<<3)); (RCC->APB2RSTR &= ~(1<<3)); }while(0)
#define GPIOC_REG_RESET()		do{	(RCC->APB2RSTR |= (1<<4)); (RCC->APB2RSTR &= ~(1<<4)); }while(0)
#define GPIOD_REG_RESET()		do{	(RCC->APB2RSTR |= (1<<5)); (RCC->APB2RSTR &= ~(1<<5)); }while(0)
#define GPIOE_REG_RESET()		do{	(RCC->APB2RSTR |= (1<<6)); (RCC->APB2RSTR &= ~(1<<6)); }while(0)

#define GPIO_BASSADDR_TO_CODE(x)		((x == GPIOA) ? 0 : (x == GPIOB) ? 1 : (x == GPIOC) ? 2 : (x == GPIOD) ? 3 : (x == GPIOE) ? 4 : (x == GPIOF) ? 5 : (x == GPIOG) ? 6 : 0 )
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET SET
#define GPIO_PIN_RESET RESET
#define FLAG_RESET RESET
#define FLAG_SET SET

#endif
