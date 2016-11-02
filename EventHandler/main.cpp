#define __NOP __nop
#include "msp.h"
#include "main.hpp"
#include <stdlib.h>
#include "Scheduler.hpp"
#include "Task.hpp"
#include "LED.hpp"
#define dCount 40
#include "Struct_Task.hpp"
uint8_t Task::m_u8NextTaskID = 0;
volatile static uint64_t SystemTicks = 0;
int dummycount=0;
Scheduler MainScheduler;
bool debounce=false;
uint8_t debounceCount=0U;
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
	// - Disable WDT
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


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
	__enable_irq();
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
}
