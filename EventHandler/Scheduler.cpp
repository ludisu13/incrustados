#include "Scheduler.hpp"

Scheduler::Scheduler()
{
	ticks=0;
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    mNextSlot = 0;
    mOpenSlotsMessages=0;
    mNextSlotMessages=0;
    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS); index++)
    {
        EventsSchedule[index].slotTask = (uintptr_t) 0;
        CurrentSchedule[index].slotTask= (uintptr_t) 0;

    }
    return;
}

uint8_t Scheduler::attach(Task * i_ToAttach)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        EventsSchedule[mNextSlot].slotTask = i_ToAttach;
        EventsSchedule[mNextSlot].required_ticks=1U;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

uint8_t Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        EventsSchedule[mNextSlot].slotTask = i_ToAttach;
        EventsSchedule[mNextSlot].required_ticks=i_u64TickInterval;
        EventsSchedule[mNextSlot].last_ticks=0U;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

uint8_t Scheduler::run(void)
{
    uint8_t NextTaskSlot = 0;
    Task * NextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(NextTaskSlot<NUMBER_OF_SLOTS)
    {
        NextTask = static_cast<Task *> (CurrentSchedule[NextTaskSlot].slotTask);
        if(NextTask != ((uintptr_t) 0))
        {
            NextTask->run();
            NextTaskSlot++;
        }
        else
        {
           break;
        }
    }
    clean(0);
    return l_u8ReturnCode;
}

void Scheduler::clean(uint8_t inputIndex)
{
	for(uint8_t index = inputIndex; index <= (NUMBER_OF_SLOTS); index++)
	    {
	       if(CurrentSchedule[index].slotTask==NULL)
	       	   {
	    	   break;
	       	   }
	       else
	       	   {
	    	   CurrentSchedule[index].slotTask= (uintptr_t) 0;
	       	   }
	       }
	return;
}


uint8_t Scheduler::CalculateSchedule(void)
{
	uint8_t NextTaskSlot = 0U;
    uint8_t l_u8ReturnCode = NO_ERR;
	while(NextTaskSlot <= (NUMBER_OF_SLOTS))
	{
		if(EventsSchedule[NextTaskSlot].slotTask==NULL)
		{
		    clean(NextTaskSlot);
			break ;
		}
		if(EventsSchedule[NextTaskSlot].required_ticks==1U)
			{
			CurrentSchedule[NextTaskSlot]=EventsSchedule[NextTaskSlot];
			}
		else
			{	EventsSchedule[NextTaskSlot].current_ticks=ticks;
				if(abs(EventsSchedule[NextTaskSlot].current_ticks-EventsSchedule[NextTaskSlot].last_ticks)==EventsSchedule[NextTaskSlot].required_ticks)
				{
					CurrentSchedule[NextTaskSlot]=EventsSchedule[NextTaskSlot];
					EventsSchedule[NextTaskSlot].last_ticks=ticks;
				}

			}
		NextTaskSlot++;
	}
	return l_u8ReturnCode;
}
uint8_t Scheduler::attachMessage(uint8_t source,uint8_t destination,bool messageType,uint8_t data)
{
	uint8_t l_u8ReturnCode=NO_ERR;
	if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
	    {
	        EventsSchedule[mNextSlot].slotTask = i_ToAttach;
	        EventsSchedule[mNextSlot].required_ticks=1U;
	        mOpenSlots--;
	        mNextSlot++;
	    }
	    else
	    {
	        l_ErrorCode = RET_ERR;
	    }




}
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
