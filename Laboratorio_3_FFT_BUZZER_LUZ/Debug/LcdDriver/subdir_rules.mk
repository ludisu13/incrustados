################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
LcdDriver/Crystalfontz128x128_ST7735.obj: ../LcdDriver/Crystalfontz128x128_ST7735.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/GrLib/grlib" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/GrLib/fonts" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/LcdDriver" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="LcdDriver/Crystalfontz128x128_ST7735.d" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.obj: ../LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/GrLib/grlib" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/GrLib/fonts" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/LcdDriver" --include_path="C:/Users/Luis/workspace_v6_1_3/Laboratorio_3_FFT_BUZZER_LUZ/CMSIS/Include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=ARM_MATH_CM4 --define=__FPU_PRESENT=1 --define=_LINKAGE --define=_CODE_ACCESS="" --define=_DATA_ACCESS="" --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --gen_func_subsections=on --abi=eabi --ual --preproc_with_compile --preproc_dependency="LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.d" --obj_directory="LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


