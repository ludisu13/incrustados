################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ServoPrueba/ServoPrueba/Servo.obj: ../ServoPrueba/ServoPrueba/Servo.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Luis/Documents/incrustados/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/LcdDriver" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="ServoPrueba/ServoPrueba/Servo.d" --obj_directory="ServoPrueba/ServoPrueba" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ServoPrueba/ServoPrueba/startup_msp432p401r_ccs.obj: ../ServoPrueba/ServoPrueba/startup_msp432p401r_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Luis/Documents/incrustados/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/LcdDriver" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="ServoPrueba/ServoPrueba/startup_msp432p401r_ccs.d" --obj_directory="ServoPrueba/ServoPrueba" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

ServoPrueba/ServoPrueba/system_msp432p401r.obj: ../ServoPrueba/ServoPrueba/system_msp432p401r.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Luis/Documents/incrustados/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/LcdDriver" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="ServoPrueba/ServoPrueba/system_msp432p401r.d" --obj_directory="ServoPrueba/ServoPrueba" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


