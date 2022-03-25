################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
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

BUILD_ARTIFACT_NAME := Hyperlink_PROP_Library
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Hyperlink_PROP_Library.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Hyperlink_PROP_Library.list \

OBJCOPY_BIN += \
Hyperlink_PROP_Library.bin \


# All Target
all: main-build

# Main-build Target
main-build: Hyperlink_PROP_Library.elf secondary-outputs

# Tool invocations
Hyperlink_PROP_Library.elf: $(OBJS) $(USER_OBJS) C:\Users\hp\STM32CubeIDE\workspace_1.3.0\Hyperlink_PROP_Library\STM32F303R6TX_FLASH.ld
	arm-none-eabi-gcc -o "Hyperlink_PROP_Library.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\hp\STM32CubeIDE\workspace_1.3.0\Hyperlink_PROP_Library\STM32F303R6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Hyperlink_PROP_Library.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Hyperlink_PROP_Library.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Hyperlink_PROP_Library.list"
	@echo 'Finished building: $@'
	@echo ' '

Hyperlink_PROP_Library.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Hyperlink_PROP_Library.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
