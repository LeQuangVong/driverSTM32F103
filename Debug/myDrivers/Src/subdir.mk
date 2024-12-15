################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myDrivers/Src/my_gpio_stm32f103.c \
../myDrivers/Src/my_rcc_stm32f103.c \
../myDrivers/Src/my_spi_stm32f103.c \
../myDrivers/Src/my_uart_stm32f103.c 

OBJS += \
./myDrivers/Src/my_gpio_stm32f103.o \
./myDrivers/Src/my_rcc_stm32f103.o \
./myDrivers/Src/my_spi_stm32f103.o \
./myDrivers/Src/my_uart_stm32f103.o 

C_DEPS += \
./myDrivers/Src/my_gpio_stm32f103.d \
./myDrivers/Src/my_rcc_stm32f103.d \
./myDrivers/Src/my_spi_stm32f103.d \
./myDrivers/Src/my_uart_stm32f103.d 


# Each subdirectory must supply rules for building sources it contributes
myDrivers/Src/%.o: ../myDrivers/Src/%.c myDrivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"E:/Embedded-interview/driverSTM32F103C8_2/myDrivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

