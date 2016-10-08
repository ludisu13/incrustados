#include "LED.hpp"


LED::LED()
{
    //ctor

	P2->DIR |= BIT1;
}

uint8_t LED::run(void)
{

	P2->OUT ^= BIT1;
    //#########################
    // Blink code
    //#########################
    return(NO_ERR);
}
