################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/LcdDriver" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup_msp432p401r_ccs.obj: ../startup_msp432p401r_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/LcdDriver" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="startup_msp432p401r_ccs.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/LcdDriver" --include_path="C:/Users/Luis/Documents/incrustados/ProyectoFinal/ProyectoFinal/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="system_msp432p401r.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


