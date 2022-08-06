################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KEY_LED/keyled.c 

OBJS += \
./KEY_LED/keyled.o 

C_DEPS += \
./KEY_LED/keyled.d 


# Each subdirectory must supply rules for building sources it contributes
KEY_LED/%.o KEY_LED/%.su: ../KEY_LED/%.c KEY_LED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../KEY_LED -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-KEY_LED

clean-KEY_LED:
	-$(RM) ./KEY_LED/keyled.d ./KEY_LED/keyled.o ./KEY_LED/keyled.su

.PHONY: clean-KEY_LED

