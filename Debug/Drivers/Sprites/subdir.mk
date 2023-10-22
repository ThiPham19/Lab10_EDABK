################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Sprites/sprite.c 

OBJS += \
./Drivers/Sprites/sprite.o 

C_DEPS += \
./Drivers/Sprites/sprite.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Sprites/%.o Drivers/Sprites/%.su Drivers/Sprites/%.cyclo: ../Drivers/Sprites/%.c Drivers/Sprites/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L053xx -c -I../Core/Inc -I"C:/Users/Dell/STM32CubeIDE/workspace_1.12.1/testOLED_L053r8/Drivers/Sprites" -I"C:/Users/Dell/STM32CubeIDE/workspace_1.12.1/testOLED_L053r8/Drivers/OLED" -I../Drivers/STM32L0xx_HAL_Driver/Inc -I../Drivers/STM32L0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Sprites

clean-Drivers-2f-Sprites:
	-$(RM) ./Drivers/Sprites/sprite.cyclo ./Drivers/Sprites/sprite.d ./Drivers/Sprites/sprite.o ./Drivers/Sprites/sprite.su

.PHONY: clean-Drivers-2f-Sprites

