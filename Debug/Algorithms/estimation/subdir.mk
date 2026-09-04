################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Algorithms/estimation/estimator.c 

OBJS += \
./Algorithms/estimation/estimator.o 

C_DEPS += \
./Algorithms/estimation/estimator.d 


# Each subdirectory must supply rules for building sources it contributes
Algorithms/estimation/%.o Algorithms/estimation/%.su Algorithms/estimation/%.cyclo: ../Algorithms/estimation/%.c Algorithms/estimation/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/libcanard" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Board" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/gripper" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/motor" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Application/actuator" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Config" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS/tasks" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan/dsdl_generated/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Algorithms-2f-estimation

clean-Algorithms-2f-estimation:
	-$(RM) ./Algorithms/estimation/estimator.cyclo ./Algorithms/estimation/estimator.d ./Algorithms/estimation/estimator.o ./Algorithms/estimation/estimator.su

.PHONY: clean-Algorithms-2f-estimation

