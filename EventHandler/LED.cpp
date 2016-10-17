#include "LED.hpp"


LED::LED(uint8_t iledMask)
{
    //ctor
	ledMask=iledMask;
	P2->DIR |= iledMask;
}

uint8_t LED::run(void)
{

	P2->OUT ^= ledMask;
    //#########################
    // Blink code
    //#########################
    return(NO_ERR);
}
