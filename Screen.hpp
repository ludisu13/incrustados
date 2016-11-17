/*
 * Screen.hpp
 *
 *  Created on: Nov 13, 2016
 *      Author: ldiego
 */

#ifndef SCREEN_HPP_
#define SCREEN_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include <stdlib.h>
#include "Scheduler.hpp"
extern "C"
{
#include <driverlib.h>
#include <grlib.h>
#include "Crystalfontz128x128_ST7735.h"
}

class screen  : public Task
{
    public:
        screen(uint8_t id, Graphics_Context *i_pContext,Graphics_Rectangle *i_pLand,Graphics_Rectangle *i_pSky);
        virtual uint8_t run(void);
        virtual uint8_t readMessage(uint8_t i_i8Source,uint64_t i_i64Data);
    protected:
    private:
        Graphics_Context *m_pMyContext;
        Graphics_Rectangle *m_pLand;
        Graphics_Rectangle *m_pSky;
        bool m_bNewOrientation;
        bool m_bCurOrientation;
        int16_t m_i16CurrentLevel;
        int16_t m_i16NewLevel;
};

#endif /* LED_HPP_ */
