#include "LED.hpp"


LED::LED(uint8_t iledMask,uint8_t id)
{
    //ctor
	ledMask=iledMask;
	P2->DIR |= iledMask;
	m_u8TaskID=id;
}

uint8_t LED::run(void)
{

	P2->OUT ^= ledMask;
    //#########################
    // Blink code
    //#########################
    return(NO_ERR);
}

uint8_t LED::readMessage(uint8_t source, uint8_t data)
{
	switch(source)
	{
			case 0: ledMask=data; //BOTON 1

			default: break;

	}
	return (NO_ERR);
}
