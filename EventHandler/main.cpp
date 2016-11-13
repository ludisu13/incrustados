#define __NOP __nop
#include "msp.h"
#include "main.hpp"
#include <stdlib.h>
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
}
#include <stdio.h>
#define dCount 40
#include "Struct_Task.hpp"
volatile static uint64_t SystemTicks = 0;
int dummycount=0;
Scheduler MainScheduler;
bool debounce=false;
uint8_t debounceCount=0U;
/* Graphic library context */
Graphics_Context g_sContext;
Graphics_Rectangle g_sNew;
Graphics_Rectangle g_sOld;

void main(void)
{

    LED BlinkLED(0x01,0U);//red
 LED BlinkLED2(0x02,1U);//green
    Setup();
  MainScheduler.attach(&BlinkLED,1000,false);
  MainScheduler.attach(&BlinkLED2,2000,false);
    while(1){
    	__wfe();
        if(SystemTicks != MainScheduler.ticks)
        {
        	MainScheduler.ticks = SystemTicks;
        	MainScheduler.mailMan();
        	MainScheduler.CalculateSchedule();
        	MainScheduler.run();
        }
    };
}

// **********************************
// Setup function for the application
// @input - none
// @output - none
// **********************************
void Setup(void)
{
	// ****************************
	//         DEVICE CONFIG
	// ****************************
	/* Halting WDT and disabling master interrupts */
	MAP_WDT_A_holdTimer();
	MAP_Interrupt_disableMaster();


	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P1.0 is connected to the Red LED
	P1->DIR |= BIT0;
	P2->DIR |= BIT2;
	P2->OUT=0;
	// Configuring P1.1 (switch) as input with pull-up
	// resistor. Rest of pins are configured as output low.
	// Notice intentional '=' assignment since all P1 pins are being
	// deliberately configured
	P1->DIR &= ~ (BIT1);
	P1->OUT = BIT1;
	P1->REN = BIT1;                         // Enable pull-up resistor (P1.1 output high)
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	P1->IFG = 0;                            // Clear all P1 interrupt flags
	P1->IES = BIT1;                         // Interrupt on high-to-low transition
	P1->IE = BIT1;                          // Enable interrupt for P1.1
	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer32_1  with MCLK (3Mhz), Division by 1, Enable the interrupt, Periodic Mode
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts
	__disable_irq();
	TIMER32_1->LOAD = 0x0000BB80; //~1ms ---> a 48Mhz
	//TIMER32_1->LOAD = 0x00000BB8; //~1ms ---> a 3Mhz
	TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0 | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE | TIMER32_CONTROL_ENABLE;
	NVIC_SetPriority(T32_INT1_IRQn,1);
	NVIC_SetPriority(PORT1_IRQn,1); //Enabling PORT1 interrupt
	NVIC_EnableIRQ(T32_INT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);

	/* Set the core voltage level to VCORE1 */
	    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

	    /* Set 2 flash wait states for Flash bank 0 and 1*/
	    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

	    /* Initializes Clock System */
	    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
	    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
	    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
	    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1 );
	    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

	    /* Initializes display */
	    Crystalfontz128x128_Init();

	    /* Set default screen orientation */
	    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

	    /* Initializes graphics context */
	    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128);
	    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);

	    g_sNew.xMax = 128;
		g_sNew.xMin =  0;
		g_sNew.yMin = 0;

		g_sOld.xMin = 0;
		g_sOld.xMax = 128;
		g_sOld.yMax = 128;
		int interrupt_count=0;

	    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
	    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2, GPIO_TERTIARY_MODULE_FUNCTION);
	    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);

	    /* Initializing ADC (ADCOSC/64/8) */
	    MAP_ADC14_enableModule();
	    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8,
	            0);

	    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM2 (A11, A13, A14)  with no repeat)
	         * with internal 2.5v reference */
	    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
	    MAP_ADC14_configureConversionMemory(ADC_MEM0,
	            ADC_VREFPOS_AVCC_VREFNEG_VSS,
	            ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

	    MAP_ADC14_configureConversionMemory(ADC_MEM1,
	            ADC_VREFPOS_AVCC_VREFNEG_VSS,
	            ADC_INPUT_A13, ADC_NONDIFFERENTIAL_INPUTS);

	    MAP_ADC14_configureConversionMemory(ADC_MEM2,
	            ADC_VREFPOS_AVCC_VREFNEG_VSS,
	            ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

	    /* Enabling the interrupt when a conversion on channel 2 (end of sequence)
	     *  is complete and enabling conversions */
	    MAP_ADC14_enableInterrupt(ADC_INT2);
	    /* Enabling Interrupts */
	        MAP_Interrupt_enableInterrupt(INT_ADC14);
	        MAP_Interrupt_enableMaster();

	        /* Setting up the sample timer to automatically step through the sequence
	         * convert.
	         */
	        MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

	        /* Triggering the start of the sample */
	        MAP_ADC14_enableConversion();
	        MAP_ADC14_toggleConversionTrigger();
	        Graphics_clearDisplay(&g_sContext);
	return;
}

extern "C"
{
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0;
		SystemTicks++;

		if(debounce==true)
			{debounceCount++;}
		if(debounceCount==dCount)
			{debounceCount=0;
			debounce=false;
			P1->IE=BIT1;}
		return;
	}

	void PORT1_IRQHandler(void)
	{	P1->IFG &= ~BIT1;
	   P1->IE &= ~BIT1;
	   debounce=true;
	    MainScheduler.attachMessage(0,0,false,10000);
	   MainScheduler.attachMessage(0,1,false,1500);
		//P2->OUT ^= BIT2;

	   dummycount++;
	   return;
	}

	void ADC14_IRQHandler(void)
	{
	    uint64_t status;

	    status = MAP_ADC14_getEnabledInterruptStatus();
	    MAP_ADC14_clearInterruptFlag(status);

	/*  interrupt_count++;ADC_MEM2 conversion completed */
	    if((status & ADC_INT2))
	    {
	        /* Store ADC14 conversion results */
	        g_sNew.yMax= 0.0194*ADC14_getResult(ADC_MEM2)-93.12;
	        g_sOld.yMin = g_sNew.yMax;
	        if(ADC14_getResult(ADC_MEM1)< 8000)
	        {Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_DOWN);}
	        else
	        {
	        	Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
	        }
	       Graphics_fillRectangleOnDisplay(g_sContext.display,
	      		&g_sNew, 0xa145);
	       Graphics_fillRectangleOnDisplay(g_sContext.display,
	       		&g_sOld,GRAPHICS_COLOR_BLUE);
	}

	}
}
