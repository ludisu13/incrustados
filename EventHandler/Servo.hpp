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


class Servo : public Task
{
    public:
        Servo();
        virtual uint8_t run(void);
    protected:
    private:
};

#endif /* Servo_HPP_ */
