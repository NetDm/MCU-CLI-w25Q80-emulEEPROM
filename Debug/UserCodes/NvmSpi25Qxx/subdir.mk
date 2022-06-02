################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UserCodes/NvmSpi25Qxx/al-spi.c \
../UserCodes/NvmSpi25Qxx/w25Qxx.c 

OBJS += \
./UserCodes/NvmSpi25Qxx/al-spi.o \
./UserCodes/NvmSpi25Qxx/w25Qxx.o 

C_DEPS += \
./UserCodes/NvmSpi25Qxx/al-spi.d \
./UserCodes/NvmSpi25Qxx/w25Qxx.d 


# Each subdirectory must supply rules for building sources it contributes
UserCodes/NvmSpi25Qxx/%.o UserCodes/NvmSpi25Qxx/%.su: ../UserCodes/NvmSpi25Qxx/%.c UserCodes/NvmSpi25Qxx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/NvmSpi25Qxx" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/microrl/src" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/microrl" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/inc" -I"/home/me/STM32CubeIDE/CLI25Q80/UserCodes" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -includestdint.h -include"/home/me/STM32CubeIDE/CLI25Q80/UserCodes/hwConfig.h" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserCodes-2f-NvmSpi25Qxx

clean-UserCodes-2f-NvmSpi25Qxx:
	-$(RM) ./UserCodes/NvmSpi25Qxx/al-spi.d ./UserCodes/NvmSpi25Qxx/al-spi.o ./UserCodes/NvmSpi25Qxx/al-spi.su ./UserCodes/NvmSpi25Qxx/w25Qxx.d ./UserCodes/NvmSpi25Qxx/w25Qxx.o ./UserCodes/NvmSpi25Qxx/w25Qxx.su

.PHONY: clean-UserCodes-2f-NvmSpi25Qxx

