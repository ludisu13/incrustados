################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
EventHandler/LcdDriver/Crystalfontz128x128_ST7735.obj: ../EventHandler/LcdDriver/Crystalfontz128x128_ST7735.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Luis/Documents/incrustados/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/LcdDriver" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="EventHandler/LcdDriver/Crystalfontz128x128_ST7735.d" --obj_directory="EventHandler/LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

EventHandler/LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.obj: ../EventHandler/LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Luis/Documents/incrustados/driverlib/MSP432P4xx" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/fonts" --include_path="C:/Users/Luis/Documents/incrustados/GrLib/grlib" --include_path="C:/Users/Luis/Documents/incrustados/LcdDriver" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.12.1.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="EventHandler/LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.d" --obj_directory="EventHandler/LcdDriver" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


