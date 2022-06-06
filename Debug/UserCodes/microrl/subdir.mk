################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserCodes/microrl/stm32CLImisc.c 

OBJS += \
./UserCodes/microrl/stm32CLImisc.o 

C_DEPS += \
./UserCodes/microrl/stm32CLImisc.d 


# Each subdirectory must supply rules for building sources it contributes
UserCodes/microrl/%.o UserCodes/microrl/%.su: ../UserCodes/microrl/%.c UserCodes/microrl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../UserCodes/ApL -I../UserCodes/NvmSpi25Qxx -I../UserCodes/microrl/src -I../UserCodes/microrl -I../UserCodes/inc -I../UserCodes -I../UserCodes/SysUtils -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -includestdint.h -includestddef.h -include../Core/Inc/main.h -include../UserCodes/hwConfig.h -include../UserCodes/myTypes.h -include../UserCodes/SysUtils/dbug.h -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserCodes-2f-microrl

clean-UserCodes-2f-microrl:
	-$(RM) ./UserCodes/microrl/stm32CLImisc.d ./UserCodes/microrl/stm32CLImisc.o ./UserCodes/microrl/stm32CLImisc.su

.PHONY: clean-UserCodes-2f-microrl

