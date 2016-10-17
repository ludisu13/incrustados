/******************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

#include "msp.h"

void main(void)
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
	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	TIMER_A0->CCR[0] = 30000; // 30 000 -> 20ms Period
	TIMER_A0->CCR[2] = 27750; // 3000 -> 2ms, 1500 -> 1ms, 2250 -> 1.5ms
	TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_3; //Set/Reset (cuando CCR[0] se pone en 0, cuando CCR[1] se pone en 1)


	P2->DIR |= BIT5; // La salida para el servo es el bit 5 del puerto 2
	P2->IE |= BIT5;
	P2->SEL0 =BIT5;
	P2->SEL1 =0;
	P2->IFG = 0;

	PMAP->KEYID = PMAP_KEYID_VAL;
	P2MAP ->PMAP_REGISTER5 |= PMAP_TA0CCR2A;

	//Acelerometro
	//	P4->DIR |= BIT0;
	//	ADC14-> MCTL[0] |= ADC14_MCTLN_INCH_13;

	//BOTON CONFIG
	P1->DIR &= ~(BIT1) | ~(BIT4);
    P1->OUT |= BIT1 | BIT4;
    P1->REN |= BIT1 | BIT4;                         // Enable pull-up resistor (P1.1 and P1.4 output high)
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IES |= BIT1 | BIT4;                         // Interrupt on high-to-low transition
    P1->IE |= BIT1 | BIT4;                          // Enable interrupt for P1.1 and P1.4

	__enable_irq();

    while(1)
    {
       __wfe();
    }

}
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    // Toggling the output on the LED
    if(P1->IFG & BIT1){
    	TIMER_A0->CCR[2] = 27005;

    	// Delay for switch debounce
        for(i = 0; i < 10000; i++){
   	    	P1->IFG &= ~BIT1;
    	}
    }
    else if(P1->IFG & BIT4){
    	TIMER_A0->CCR[2] = 28495;

    	// Delay for switch debounce
    	 for(i = 0; i < 10000; i++){
    	   	P1->IFG &= ~BIT4;
         }
    }
}
