################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/stm32f103_gpio.c \
../drivers/Src/stm32f103_rcc.c \
../drivers/Src/stm32f103_spi.c \
../drivers/Src/stm32f103_uart.c 

OBJS += \
./drivers/Src/stm32f103_gpio.o \
./drivers/Src/stm32f103_rcc.o \
./drivers/Src/stm32f103_spi.o \
./drivers/Src/stm32f103_uart.o 

C_DEPS += \
./drivers/Src/stm32f103_gpio.d \
./drivers/Src/stm32f103_rcc.d \
./drivers/Src/stm32f103_spi.d \
./drivers/Src/stm32f103_uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"E:/Embedded-interview/driverSTM32F103/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

