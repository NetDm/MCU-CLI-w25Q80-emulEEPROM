################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserCodes/microrl/src/microrl.c 

OBJS += \
./UserCodes/microrl/src/microrl.o 

C_DEPS += \
./UserCodes/microrl/src/microrl.d 


# Each subdirectory must supply rules for building sources it contributes
UserCodes/microrl/src/%.o UserCodes/microrl/src/%.su: ../UserCodes/microrl/src/%.c UserCodes/microrl/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/ApL" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/NvmSpi25Qxx" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/microrl/src" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/microrl" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/inc" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/SysUtils" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -includestdint.h -includestddef.h -include"/home/me/STM32CubeIDE/CLI25Q80/Core/Inc/main.h" -include"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/hwConfig.h" -include"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/myTypes.h" -include"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/SysUtils/dbug.h" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserCodes-2f-microrl-2f-src

clean-UserCodes-2f-microrl-2f-src:
	-$(RM) ./UserCodes/microrl/src/microrl.d ./UserCodes/microrl/src/microrl.o ./UserCodes/microrl/src/microrl.su

.PHONY: clean-UserCodes-2f-microrl-2f-src

