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
    uint64_t ticks;
    uint8_t attach(Task * i_ToAttach);
    uint8_t attach(Task * i_ToAttach, uint64_t i_u64TickInterval);
    uint8_t attachMessage(uint8_t source,uint8_t destination,bool messageType,uint8_t data);
    uint8_t run(void);
    uint8_t CalculateSchedule(void);
    Message MessageQueue[NUMBER_OF_MESSAGES];
private:
    uint8_t mOpenSlots;
    uint8_t mNextSlot;
    uint8_t mOpenSlotsMessages;
    uint8_t mNextSlotMessages;
    void clean(uint8_t inputIndex);
    Slot  EventsSchedule[NUMBER_OF_SLOTS];
    Slot  CurrentSchedule[NUMBER_OF_SLOTS];
    uint8_t SortScheduleByPriority(Task * i_pSchedule);
};



#endif /* TASKS_SCHEDULER_HPP_ */
