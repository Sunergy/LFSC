################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := c:/ti/ccsv6/tools/compiler/arm_5.1.8

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./Interface_Motores.obj" \
"./Motor.obj" \
"./Potenciometro.obj" \
"./RTC.obj" \
"./main.obj" \
"./spa.obj" \
"./spa_calculation.obj" \
"./tm4c1294ncpdt_startup_ccs.obj" \
"C:/ti/ccsv6/eclipse/TIVAWARE_INSTALL/driverlib/ccs/Debug/driverlib.lib" \
"../tm4c1294ncpdt.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Integracion_V6.out \

EXE_OUTPUTS__QUOTED += \
"Integracion_V6.out" \

BIN_OUTPUTS += \
Integracion_V6.hex \

BIN_OUTPUTS__QUOTED += \
"Integracion_V6.hex" \


# All Target
all: Integracion_V6.out

# Tool invocations
Integracion_V6.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.8/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --gcc --define=ccs="ccs" --define=PART_TM4C1294NCPDT --display_error_number --diag_warning=225 --diag_wrap=off -z -m"Integracion_V6.map" --heap_size=4096 --stack_size=4096 -i"c:/ti/ccsv6/tools/compiler/arm_5.1.8/lib" -i"c:/ti/ccsv6/tools/compiler/arm_5.1.8/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="Integracion_V6_linkInfo.xml" --rom_model -o "Integracion_V6.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

Integracion_V6.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.8/bin/armhex"  -o "Integracion_V6.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "Interface_Motores.pp" "Motor.pp" "Potenciometro.pp" "main.pp" "spa.pp" "spa_calculation.pp" "tm4c1294ncpdt_startup_ccs.pp" 
	-$(RM) "RTC.pp" 
	-$(RM) "Interface_Motores.obj" "Motor.obj" "Potenciometro.obj" "RTC.obj" "main.obj" "spa.obj" "spa_calculation.obj" "tm4c1294ncpdt_startup_ccs.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

