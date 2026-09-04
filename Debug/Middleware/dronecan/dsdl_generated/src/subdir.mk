################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.c \
../Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.c \
../Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.c 

OBJS += \
./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.o \
./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.o \
./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.o 

C_DEPS += \
./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.d \
./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.d \
./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.d 


# Each subdirectory must supply rules for building sources it contributes
Middleware/dronecan/dsdl_generated/src/%.o Middleware/dronecan/dsdl_generated/src/%.su Middleware/dronecan/dsdl_generated/src/%.cyclo: ../Middleware/dronecan/dsdl_generated/src/%.c Middleware/dronecan/dsdl_generated/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/libcanard" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Board" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/gripper" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/motor" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Application/actuator" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Config" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS/tasks" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan/dsdl_generated/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middleware-2f-dronecan-2f-dsdl_generated-2f-src

clean-Middleware-2f-dronecan-2f-dsdl_generated-2f-src:
	-$(RM) ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.cyclo ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.d ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.o ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.ArrayCommand.su ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.cyclo ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.d ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.o ./Middleware/dronecan/dsdl_generated/src/thaco.equipment.actuator.Command.su ./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.cyclo ./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.d ./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.o ./Middleware/dronecan/dsdl_generated/src/uavcan.protocol.NodeStatus.su

.PHONY: clean-Middleware-2f-dronecan-2f-dsdl_generated-2f-src

