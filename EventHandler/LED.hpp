/*
 * LED.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef LED_HPP_
#define LED_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"


class LED : public Task
{
    public:
        LED(uint8_t ledMask,uint8_t id);
        uint8_t ledMask;
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t source,uint8_t data);
    protected:
    private:
};

#endif /* LED_HPP_ */
