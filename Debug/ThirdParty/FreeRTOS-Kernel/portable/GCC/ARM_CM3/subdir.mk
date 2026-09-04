################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.c 

OBJS += \
./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/%.o ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/%.su ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/%.cyclo: ../ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/%.c ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/libcanard" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Board" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/gripper" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/motor" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Application/actuator" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Config" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS/tasks" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan/dsdl_generated/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ThirdParty-2f-FreeRTOS-2d-Kernel-2f-portable-2f-GCC-2f-ARM_CM3

clean-ThirdParty-2f-FreeRTOS-2d-Kernel-2f-portable-2f-GCC-2f-ARM_CM3:
	-$(RM) ./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.cyclo ./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.d ./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.o ./ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/port.su

.PHONY: clean-ThirdParty-2f-FreeRTOS-2d-Kernel-2f-portable-2f-GCC-2f-ARM_CM3

