#define __NOP __nop
#include "msp.h"
#include "main.hpp"
#include <stdlib.h>
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#include "Servo.hpp"
extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
}
#include "Screen.hpp"
#include <stdio.h>
#include "Struct_Task.hpp"
volatile static uint64_t g_i64SystemTicks = 0;
Scheduler g_sMainScheduler;

/* Graphic library context */
Graphics_Context g_sContext;
Graphics_Rectangle g_sSky;
Graphics_Rectangle g_sLand;

void main(void)
{

  Servo l_sMyServo(0x04,0U);
  LED l_lBlinkLED(0x02,1U); //Led for signaling purposes
  screen l_sMyScreen(0x03,&g_sContext,&g_sLand,&g_sSky);
  g_sMainScheduler.attach(&l_sMyScreen,260,false);
  g_sMainScheduler.attach(&l_sMyServo,200,false);
  g_sMainScheduler.attach(&l_lBlinkLED,2000,false);
  Setup();
    while(1){
    	__wfe();
        if( g_i64SystemTicks != g_sMainScheduler.m_i64Ticks)
        {
        	g_sMainScheduler.m_i64Ticks =  g_i64SystemTicks;
        	g_sMainScheduler.mailMan();
        	g_sMainScheduler.CalculateSchedule();
        	g_sMainScheduler.run();
        }
    };
}

// **********************************
// Setup function for the application
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
	P2->DIR |= BIT2; // configuration for leds
	P2->OUT=0;
	P2->DIR |= BIT5; //Servo output
	P2->SEL0 =BIT5;
	P2->SEL1 =0;

	// ****************************
	//       TIMER32 CONFIG
	// ****************************
	// Tick generator
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
	NVIC_EnableIRQ(T32_INT1_IRQn);
	// ****************************
	//       Screen Config
	// ***************************

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

	 /*Initializes rectangles*/
	 g_sLand.xMax = 128;
	 g_sLand.xMin =  0;
	 g_sLand.yMax = 128;
	 g_sLand.yMin = 64;

	 g_sSky.xMin = 0;
	 g_sSky.xMax = 128;
	 g_sSky.yMax = 63;
     g_sSky.yMin =0 ;
     // ****************************
     //      ADC Config
     // ***************************
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
	 MAP_Interrupt_enableInterrupt(INT_ADC14);
	 // ****************************
	 //      Timer A0 Config
	 // ************
	 TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8;
	 TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_1;
	 TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	 TIMER_A0->CCR[0] = 60000; // 30 000 -> 20ms Period
	 TIMER_A0->CCR[2] = 55500; // 3000 -> 2ms, 1500 -> 1ms, 2250 -> 1.5ms
	 TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_3; //Set/Reset (cuando CCR[0] se pone en 0, cuando CCR[1] se pone en 1)
	 // Mapping timer output to servo
	 PMAP->KEYID = PMAP_KEYID_VAL;
	 P2MAP ->PMAP_REGISTER5 |= PMAP_TA0CCR2A;
	 // ****************************
	 //      Start up sequence
	 // ************
	 /* Setting up the sample timer to automatically step through the sequence
	  * convert.
	*/
	 MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
     /* Triggering the start of the sample */
	 MAP_ADC14_enableConversion();
	 MAP_ADC14_toggleConversionTrigger();
	 Graphics_clearDisplay(&g_sContext);//cleaning display
	 Graphics_fillRectangleOnDisplay(g_sContext.display,
	            		&g_sLand, 0xa347);// display land and sky in one haf of the screen each
	 Graphics_fillRectangleOnDisplay(g_sContext.display,
	             		&g_sSky,GRAPHICS_COLOR_BLUE);
	 MAP_Interrupt_enableMaster();// enable interrupts
	return;
}

extern "C"
{
	// **********************************
	// T32 Interrupt subroutine
	// Updates ticks
	// **********************************
	void T32_INT1_IRQHandler(void)
	{
		TIMER32_1->INTCLR = 0U;
		P1->OUT ^= BIT0; //tick update
		 g_i64SystemTicks++;
		return;
	}


	// **********************************
	// ADC14 Interrupt subroutine
	// Sends three messages:
	//	-One to change the horizon
	//  -One to change the orientation
	//	-One to rotate the servo
	// **********************************
	void ADC14_IRQHandler(void)
	{
	    uint64_t l_i64Status;

	    l_i64Status = MAP_ADC14_getEnabledInterruptStatus();
	    MAP_ADC14_clearInterruptFlag(l_i64Status);
	    if((l_i64Status & ADC_INT2))
	    {
	    	g_sMainScheduler.attachMessage(0,3,true,ADC14_getResult(ADC_MEM2));
	    	g_sMainScheduler.attachMessage(1,3,true,ADC14_getResult(ADC_MEM1)); //send message to screen and servo tasks
	    	g_sMainScheduler.attachMessage(0,4,true,ADC14_getResult(ADC_MEM2));

	}
	    MAP_Interrupt_disableInterrupt(INT_ADC14);
	}
}
