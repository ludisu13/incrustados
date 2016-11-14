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
	TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_ID__8; // | TIMER_A_CTL_IE;
	TIMER_A0->EX0 = TIMER_A_EX0_TAIDEX_1;
	//NVIC_SetPriority(TA0_N_IRQn,1);
	//NVIC_EnableIRQ(TA0_N_IRQn);
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_EnableIRQ(PORT1_IRQn);

	TIMER_A0->CTL |=  TIMER_A_CTL_MC__UP;
	TIMER_A0->CCR[0] = 60000; // 30 000 -> 20ms Period
	TIMER_A0->CCR[2] = 55500; // 3000 -> 2ms, 1500 -> 1ms, 2250 -> 1.5ms
	TIMER_A0->CCTL[2] |= TIMER_A_CCTLN_OUTMOD_3; //Set/Reset (cuando CCR[0] se pone en 0, cuando CCR[1] se pone en 1)


	P2->DIR |= BIT5; // La salida para el servo es el bit 5 del puerto 2
	//P2->IE |= BIT5;
	P2->SEL0 =BIT5;
	P2->SEL1 =0;
	//P2->IFG = 0;

	// Mapeo del Timer_A0.2 al pin 2.5 (PWM) output
	PMAP->KEYID = PMAP_KEYID_VAL;
	P2MAP ->PMAP_REGISTER5 |= PMAP_TA0CCR2A;


	//BOTON CONFIG
	P1->DIR &= ~(BIT1) | ~(BIT4);
    P1->OUT |= BIT1 | BIT4;
    P1->REN |= BIT1 | BIT4;                 // Enable pull-up resistor (P1.1 and P1.4 output high)
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1->IFG = 0;                            // Clear all P1 interrupt flags
    P1->IES |= BIT1 | BIT4;                 // Interrupt on high-to-low transition
    P1->IE |= BIT1 | BIT4;                  // Enable interrupt for P1.1 and P1.4

	//Acelerometro
    // Puerto 4.0  con el ADC14 A13 analog IN
//	P4MAP ->PMAP_REGISTER0;
//	P4->DIR &= ~(BIT0);


	// Clock using ACLK as VLOCLK 128/(9.4kHz)* 16 ciclos => 0.22s
//	CS->CTL1 |= CS_CTL1_SELA__VLOCLK | CS_CTL1_DIVA_7 ;
//	CS->CLKEN |= CS_CLKEN_ACLK_EN;



//	ADC14 ->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_ON | ADC14_CTL0_SSEL__ACLK |ADC14_CTL0_CONSEQ_2  | ADC14_CTL0_SHP |ADC14_CTL0_SHS_3 |ADC14_CTL0_SHT0_2|ADC14_CTL0_SHT1_2 ; // ADC14_CTL0_MSC muestrea varias veces con una Interr.



//	ADC14 ->MCTL[13] &= ~(ADC14_MCTLN_DIF);
//	ADC14-> MCTL[13] |= ADC14_MCTLN_INCH_13 ;
	 //ADC14CONSEQx =10 mode single chanel is converted repeatdly

			// The ADC results are written to the ADC14MEMx defined by the CSTARTADDx bits
			// ADC14RES = 03h for 14-bit mode. ADC14ENC low pulse duration must be at least three ADC14CLK cycle
			// to stop Reset ADC14ENC



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
    	TIMER_A0->CCR[2] = 52600; // 2.6ms -> 90


    	// Delay for switch debounce
        for(i = 0; i < 80000; i++){
   	    	P1->IFG &= ~BIT1;
    	}
    }
    else if(P1->IFG & BIT4){
    	TIMER_A0->CCR[2] = 58200; // 0.6 ms -> -90

    	// Delay for switch debounce
    	 for(i = 0; i < 80000; i++){
    	   	P1->IFG &= ~BIT4;
         }
    }
}


void ADC14_IRQHandler(void){
	__nop;

}
