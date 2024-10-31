#ifndef INC_STM32F103_I2C_H_
#define INC_STM32F103_I2C_H_

#include "stm32f103.h"

typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint32_t I2C_DeviceAddress;
	uint32_t I2C_AckControl;
	uint32_t I2C_FMDutyCycle;
}I2C_Config_t;

typedef struct{
	I2C_Config_t I2C_Config;
	I2C_RegDef_t *pI2Cx;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t  TxRxState;
	uint8_t  DevAddr;
	uint32_t RxSize;
	uint8_t  Sr;
}I2C_Handle_t;


#endif /* INC_STM32F103_I2C_H_ */
