/*
 * Servo.hpp
 *
 *  Created on: Oct 5, 2016
 *      Author: P.Av.Lo
 */

#ifndef Servo_HPP_
#define Servo_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
<<<<<<< HEAD
extern "C"
{
#include <driverlib.h>
}
class Servo : public Task
{
    public:
        Servo(uint8_t i_i8ID, uint16_t i_i16NewAngle);
        uint16_t m_i16Angle; // Set 60000 to start at center
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t i_i8Source,uint64_t i_i64Data);
=======


class Servo : public Task
{
    public:
        Servo(uint8_t id, uint16_t newAngle);
        uint16_t Angle; // Set 60000 to start at center
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t source,uint64_t data);
>>>>>>> refs/remotes/origin/master
    protected:
    private:
};

#endif /* Servo_HPP_ */
