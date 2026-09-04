################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Board/board.c \
../Board/board_can.c \
../Board/board_gpio.c \
../Board/board_pwm.c \
../Board/board_time.c \
../Board/board_uart.c 

OBJS += \
./Board/board.o \
./Board/board_can.o \
./Board/board_gpio.o \
./Board/board_pwm.o \
./Board/board_time.o \
./Board/board_uart.o 

C_DEPS += \
./Board/board.d \
./Board/board_can.d \
./Board/board_gpio.d \
./Board/board_pwm.d \
./Board/board_time.d \
./Board/board_uart.d 


# Each subdirectory must supply rules for building sources it contributes
Board/%.o Board/%.su Board/%.cyclo: ../Board/%.c Board/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/libcanard" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Board" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/gripper" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Devices/motor" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Application/actuator" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Config" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS" -I"C:/Users/truonghquangthong/Documents/drone_actuator/RTOS/tasks" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan" -I"C:/Users/truonghquangthong/Documents/drone_actuator/Middleware/dronecan/dsdl_generated/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/include" -I"C:/Users/truonghquangthong/Documents/drone_actuator/ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Board

clean-Board:
	-$(RM) ./Board/board.cyclo ./Board/board.d ./Board/board.o ./Board/board.su ./Board/board_can.cyclo ./Board/board_can.d ./Board/board_can.o ./Board/board_can.su ./Board/board_gpio.cyclo ./Board/board_gpio.d ./Board/board_gpio.o ./Board/board_gpio.su ./Board/board_pwm.cyclo ./Board/board_pwm.d ./Board/board_pwm.o ./Board/board_pwm.su ./Board/board_time.cyclo ./Board/board_time.d ./Board/board_time.o ./Board/board_time.su ./Board/board_uart.cyclo ./Board/board_uart.d ./Board/board_uart.o ./Board/board_uart.su

.PHONY: clean-Board

