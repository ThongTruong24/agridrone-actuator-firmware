################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utils/subdir.mk
-include ThirdParty/libcanard/subdir.mk
-include ThirdParty/FreeRTOS-Kernel/portable/MemMang/subdir.mk
-include ThirdParty/FreeRTOS-Kernel/portable/GCC/ARM_CM3/subdir.mk
-include ThirdParty/FreeRTOS-Kernel/subdir.mk
-include Services/parameter/subdir.mk
-include Services/health/subdir.mk
-include Services/command/subdir.mk
-include RTOS/tasks/subdir.mk
-include RTOS/subdir.mk
-include Middleware/dronecan/dsdl_generated/src/subdir.mk
-include Middleware/dronecan/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Devices/motor/subdir.mk
-include Devices/gripper/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Board/subdir.mk
-include Application/debug_console/subdir.mk
-include Application/actuator/subdir.mk
-include Algorithms/math/subdir.mk
-include Algorithms/filters/subdir.mk
-include Algorithms/estimation/subdir.mk
-include Algorithms/control/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := drone_actuator
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
drone_actuator.elf \

MAP_FILES += \
drone_actuator.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
drone_actuator.list \


# All Target
all: main-build

# Main-build Target
main-build: drone_actuator.elf secondary-outputs

# Tool invocations
drone_actuator.elf drone_actuator.map: $(OBJS) $(USER_OBJS) C:\Users\truonghquangthong\Documents\drone_actuator\STM32F103C8TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "drone_actuator.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\truonghquangthong\Documents\drone_actuator\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="drone_actuator.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

drone_actuator.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "drone_actuator.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout drone_actuator.elf drone_actuator.list drone_actuator.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
