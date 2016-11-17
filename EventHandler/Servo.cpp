#include "Servo.hpp"

Servo::Servo(uint8_t i_i8ID, uint16_t i_i16NewAngle)
{
	m_u8TaskID= i_i8ID;
	m_i16Angle= i_i16NewAngle;
}

	// **********************************
	// servo run function
	// changes duty cycle
	// **********************************
uint8_t Servo::run(void)
{
	TIMER_A0->CCR[2]=m_i16Angle;
	return(NO_ERR);
}

	// **********************************
	// servo read message function
	// Reads message to change duty cycle according to the reading from the accelerometer
 	// *********************
uint8_t Servo::readMessage(uint8_t i_i8Source, uint64_t i_i64Data)
{
	if(i_i8Source==0){
		m_i16Angle=-0.85*i_i64Data+62280;// change duty cycle
	}
	return (NO_ERR);
}
