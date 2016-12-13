//****************************************************************************

#include "msp.h"
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <arm_math.h>
#include "arm_const_structs.h"

/* ------------------------------------------------------------------
* Global variables for FFT Bin Example
* ------------------------------------------------------------------- */


	int frec=20000;
void main(void)
{

	//int fmin=1500;
	//int fmax=10000;

	/* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();
	__disable_irq();

	// ****************************
	//       TIMER CONFIG
	// ****************************
	// - Disable all interrupts
	// - Configure Timer A0 with SMCLK, Division by 8, Enable the interrupt
	// - Enable the interrupt in the NVIC
	// - Start the timer in UP mode.
	// - Re-enable interrupts

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK; // | TIMER_A_CTL_ID__2; // | TIMER_A_CTL_IE;
	//TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_1;
	//NVIC_SetPriority(TA0_N_IRQn,1);
	//NVIC_EnableIRQ(TA0_N_IRQn);
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);

	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	TIMER_A0->CCR[0] = frec; // 1500 -> 2 kHz a 3 MHz el system clock
	TIMER_A0->CCR[4] = frec/2; // Periodo simétrico
	TIMER_A0->CCTL[4] |= TIMER_A_CCTLN_OUTMOD_3; //Set/Reset (cuando CCR[0] se pone en 0, cuando CCR[1] se pone en 1)


	P2->DIR |= BIT7; // La salida para el servo es el bit 7 del puerto 2
	//P2->IE |= BIT7;
	P2->SEL0 =BIT7;
	P2->SEL1 =0;
	//P2->IFG = 0;

	// Mapeo del Timer_A0.4 al pin 2.7 (PWM) output
	PMAP->KEYID = PMAP_KEYID_VAL;
	P2MAP ->PMAP_REGISTER7 |= PMAP_TA0CCR4A;

	//BOTON CONFIG
	P1->DIR &= ~(BIT1) | ~(BIT4);
	P1->OUT |= BIT1 | BIT4;
	P1->REN |= BIT1 | BIT4;                 // Enable pull-up resistor (P1.1 and P1.4 output high)
	P1->SEL0 = 0;
	P1->SEL1 = 0;
	P1->IFG = 0;                            // Clear all P1 interrupt flags
	P1->IES |= BIT1 | BIT4;                 // Interrupt on high-to-low transition
	P1->IE |= BIT1 | BIT4;                  // Enable interrupt for P1.1 and P1.4


	__enable_irq();

//*******************************************************************************************************************************

	while(1){
	   __wfe();
	}

}

//*********************************************************************************************************************************************


void PORT1_IRQHandler(void)
{
    volatile uint32_t i;

    // Toggling the output on the LED
    if(P1->IFG & BIT1){
    	frec  += 1000; // 2.6ms -> 90

    	// Delay for switch debounce
        for(i = 0; i < 80000; i++){
   	    	P1->IFG &= ~BIT1;
    	}
    }
    else if(P1->IFG & BIT4){
    	frec -= 1000; // 0.6 ms -> -90

    	// Delay for switch debounce
    	 for(i = 0; i < 80000; i++){
    	   	P1->IFG &= ~BIT4;
         }
    }
	TIMER_A0->CCR[0] = frec; // 1500 -> 2 kHz a 3 MHz el system clock
	TIMER_A0->CCR[4] = frec/2; // Periodo simétrico
    return;
}
