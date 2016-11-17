/*
 * LED.hpp
 *
 *  Created on: Oct 20, 2016
 *      Author: ldiego,pavila
 */

#ifndef LED_HPP_
#define LED_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


class LED : public Task
{
    public:
        LED(uint8_t i_i8ledMask,uint8_t i_i8ID);
        uint8_t m_i8LedMask;
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t i_i8Source,uint8_t i_i8Data);
    protected:
    private:
};

#endif /* LED_HPP_ */
