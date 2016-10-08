#include "Servo.hpp"

Servo::Servo()
{

	//ctor
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;


	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts

	__disable_irq();
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__2 | TIMER_A_CTL_IE;
	NVIC_SetPriority(TA0_N_IRQn,1);
	NVIC_EnableIRQ(TA0_N_IRQn);
	TIMER_A0->CCR[0] = 10000;
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	P2->DIR |= BIT5;
	P2->OUT |= BIT5;
	__enable_irq();



}

uint8_t Servo::run(void)
{

	P2->OUT |= BIT5;
	TIMER_A0->CCR[1] = 9000; // CCR[0] - CCR[1] = 1000 Hz = 1ms


	return(NO_ERR);
}
