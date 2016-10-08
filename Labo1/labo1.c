/******************************************************************************
*
* MSP432 blink.c template - P1.0 port toggle
*
* CMSIS compliant coding
*
******************************************************************************/

#include "msp.h"
#include "labo1.h"
#include <driverlib.h>
#include <stdio.h>
#include "../../../workspace_v6_1_3/empty_project/HAL_I2C.h"
#include "../../../workspace_v6_1_3/empty_project/HAL_OPT3001.h"

float lux;

void main(void)
{
    setup();



    while(1)
    {
    lux = OPT3001_getLux();
    if (lux>64000)
    {P2->OUT |=BIT0 ;}
    else
    {P2->OUT |=~BIT0;}
    //_wfe();                ; /* Delay */
    }
}

void setup(void)
{

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  /* Stop watchdog timer */
	 /* Initialize I2C communication */
	    Init_I2C_GPIO();
	    I2C_init();

	    /* Initialize OPT3001 digital ambient light sensor */
	    OPT3001_init();

	    __delay_cycles(100000);
	P2->DIR |= BIT2; //BLUE LED
	    // The following code toggles P1.0 port
	P1->DIR |= BIT0;                             /* Configure P1.0 as output */
	__disable_irq();
	NVIC_SetPriority(PORT1_IRQn,1); //Enabling PORT1 interrupt
	NVIC_EnableIRQ(PORT1_IRQn);
	// Configuring P1.0 as output and P1.1 (switch) as input with pull-up
	    // resistor. Rest of pins are configured as output low.
	    // Notice intentional '=' assignment since all P1 pins are being
	    // deliberately configured
	    P1->DIR = ~ (BIT1);
	    P1->OUT = BIT1;
	    P1->REN = BIT1;                         // Enable pull-up resistor (P1.1 output high)
	    P1->SEL0 = 0;
	    P1->SEL1 = 0;
	    P1->IFG = 0;                            // Clear all P1 interrupt flags
	    P1->IES = BIT1;                         // Interrupt on high-to-low transition
	    P1->IE = BIT1;                          // Enable interrupt for P1.1

	    // Enable Port 1 interrupt on the NVIC
	   //NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

	    // Enable global interrupt
	        __enable_irq();

}

/* Port1 ISR */
void PORT1_ISR(void)
{
    volatile uint32_t i;

    // Toggling the output on the LED
    if(P1->IFG & BIT1)
        P1->OUT ^= BIT0;

    // Delay for switch debounce
    for(i = 0; i < 10000; i++)

    P1->IFG &= ~BIT1;
}
