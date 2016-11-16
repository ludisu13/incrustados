#include "Servo.hpp"

Servo::Servo(uint8_t id, uint16_t newAngle)
{
	m_u8TaskID= id;
	Angle= newAngle;;
}


uint8_t Servo::run(void)
{
	return(NO_ERR);
}

uint8_t Servo::readMessage(uint8_t source, uint64_t data)
{
	if(source==0){
		TIMER_A0->CCR[2]=-0.85*data+62280;// actualizacion
	}
	return (NO_ERR);
}
