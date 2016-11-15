/*
 * Screen.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
}

class screen  : public Task
{
    public:
        screen(uint8_t id, Graphics_Context *context,Graphics_Rectangle *New,Graphics_Rectangle *Old);
        Graphics_Context *mycontext;
        Graphics_Rectangle *newRec;
        Graphics_Rectangle *oldRec;
        bool newOrientation;
        bool curOrientation;
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t source,uint64_t data);
    protected:
    private:
};

#endif /* LED_HPP_ */