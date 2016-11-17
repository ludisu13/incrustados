#include "LED.hpp"


LED::LED(uint8_t i_i8LedMask,uint8_t i_i8ID)
{
    //ctor
	m_i8LedMask=i_i8LedMask;
	P2->DIR |= m_i8LedMask;
	m_u8TaskID=i_i8ID;
}
	// **********************************
	// led run Message function
	// Blinks led
	// **********************************
uint8_t LED::run(void)
{

	P2->OUT ^= m_i8LedMask;
    //#########################
    // Blink code
    //#########################
    return(NO_ERR);
}
	// **********************************
	// led read Message function
	// Reads one message to change toggling led
	// **********************************
uint8_t LED::readMessage(uint8_t i_i8Source, uint8_t i_i8Data)
{
	switch(i_i8Source)
	{
			case 0: m_i8LedMask= i_i8Data; //BOTON 1

			default: break;

	}
	return (NO_ERR);
}
