/*
 * Scheduler.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */
#include "Struct_Task.hpp"

#ifndef TASKS_SCHEDULER_HPP_
#define TASKS_SCHEDULER_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"
#include "Message.hpp"
#include <stdlib.h>
#define NUMBER_OF_SLOTS 255
#define NUMBER_OF_MESSAGES 20
#define NULL            0

class Scheduler
{
public:
    Scheduler();
    uint64_t m_i64Ticks;
    uint8_t attach(Task * i_pToAttach,bool i_bShot);
    uint8_t attach(Task * i_pToAttach, uint64_t i_u64TickInterval,bool i_bShot);
    uint8_t attachMessage(uint8_t i_i8Source,uint8_t i_i8Destination,bool i_bMessageType,uint64_t i_i64Data);
    uint8_t run(void);
    uint8_t CalculateSchedule(void);
    uint8_t mailMan(void);
    Message m_mMessageQueue[NUMBER_OF_MESSAGES];
private:
    uint8_t m_i8OpenSlots;
    uint8_t m_i8NextSlot;
    uint8_t m_i8OpenSlotsMessages;
    uint8_t m_i8NextSlotMessages;
    void clean(uint8_t inputIndex);
    void cleanMail(void);
    void runMessage(uint8_t i_i8TaskIndex,uint8_t i_i8MsgIndex);
    Slot  m_sEventsSchedule[NUMBER_OF_SLOTS];
    Slot  m_sCurrentSchedule[NUMBER_OF_SLOTS];

    uint8_t SortScheduleByPriority(Task * i_pSchedule);
};



#endif /* TASKS_SCHEDULER_HPP_ */
