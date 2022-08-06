################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OLED_ConfigFile/oled.c 

OBJS += \
./OLED_ConfigFile/oled.o 

C_DEPS += \
./OLED_ConfigFile/oled.d 


# Each subdirectory must supply rules for building sources it contributes
OLED_ConfigFile/%.o OLED_ConfigFile/%.su: ../OLED_ConfigFile/%.c OLED_ConfigFile/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../OLED_ConfigFile -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-OLED_ConfigFile

clean-OLED_ConfigFile:
	-$(RM) ./OLED_ConfigFile/oled.d ./OLED_ConfigFile/oled.o ./OLED_ConfigFile/oled.su

.PHONY: clean-OLED_ConfigFile

