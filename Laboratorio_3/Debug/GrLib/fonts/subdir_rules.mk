################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
GrLib/fonts/fontfixed6x8.obj: ../GrLib/fonts/fontfixed6x8.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3/LcdDriver" --include_path="C:/Users/Luis/Documents/incrustados/Laboratorio_3/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="GrLib/fonts/fontfixed6x8.d" --obj_directory="GrLib/fonts" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


