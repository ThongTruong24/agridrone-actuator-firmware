################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Devices/motor/motor.c 

OBJS += \
./Devices/motor/motor.o 

C_DEPS += \
./Devices/motor/motor.d 


# Each subdirectory must supply rules for building sources it contributes
Devices/motor/%.o Devices/motor/%.su Devices/motor/%.cyclo: ../Devices/motor/%.c Devices/motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/libcanard" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Board" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/gripper" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/motor" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Application/actuator" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Config" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS/tasks" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan/dsdl_generated/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Devices-2f-motor

clean-Devices-2f-motor:
	-$(RM) ./Devices/motor/motor.cyclo ./Devices/motor/motor.d ./Devices/motor/motor.o ./Devices/motor/motor.su

.PHONY: clean-Devices-2f-motor

