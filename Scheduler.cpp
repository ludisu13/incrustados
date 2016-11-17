#include "Scheduler.hpp"

Scheduler::Scheduler()
{
	m_i64Ticks=0;
    m_i8OpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    m_i8NextSlot = 0;
    m_i8OpenSlotsMessages=static_cast<uint8_t>(NUMBER_OF_MESSAGES);
    m_i8NextSlotMessages=0;
    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS); index++)
    {
        m_sEventsSchedule[index].m_pSlotTask = (uintptr_t) 0;
        m_sCurrentSchedule[index].m_pSlotTask= (uintptr_t) 0;
        m_sEventsSchedule[index].m_BIsOneShot=false;
        m_sEventsSchedule[index].m_BExecuted=false;

    }
    for(uint8_t index=0;index <=NUMBER_OF_MESSAGES;index++)
    {
    	m_mMessageQueue[index].m_bMessageWritten=false;
    }
    return;
}
	// **********************************
	// scheduler attach function
	// attaches a new task to the scheduler
	// **********************************
uint8_t Scheduler::attach(Task * i_pToAttach,bool i_bShot)
{
    uint8_t l_i8ErrorCode = NO_ERR;
    if((m_i8OpenSlots>0) && (m_i8NextSlot < NUMBER_OF_SLOTS))
    {
        m_sEventsSchedule[m_i8NextSlot].m_pSlotTask = i_pToAttach;
        m_sEventsSchedule[m_i8NextSlot].m_i64RequiredTicks=1U;
        m_sEventsSchedule[m_i8NextSlot].m_BIsOneShot=i_bShot;
        m_i8OpenSlots--;
        m_i8NextSlot++;
    }
    else
    {
        l_i8ErrorCode = RET_ERR;
    }
    return l_i8ErrorCode;
}
	// **********************************
	// scheduler attach function
	// attaches a new task to the scheduler
	// ************************************
uint8_t Scheduler::attach(Task * i_pToAttach, uint64_t i_u64TickInterval,bool i_bShot)
{
    uint8_t l_i8ErrorCode = NO_ERR;
    if((m_i8OpenSlots>0) && (m_i8NextSlot < NUMBER_OF_SLOTS))
    {
        m_sEventsSchedule[m_i8NextSlot].m_pSlotTask = i_pToAttach;
        m_sEventsSchedule[m_i8NextSlot].m_i64RequiredTicks=i_u64TickInterval;
        m_sEventsSchedule[m_i8NextSlot].m_i64LastTicks=0U;
        m_sEventsSchedule[m_i8NextSlot].m_BIsOneShot=i_bShot;
        m_i8OpenSlots--;
        m_i8NextSlot++;
    }
    else
    {
        l_i8ErrorCode = RET_ERR;
    }
    return l_i8ErrorCode;
}
// **********************************
	// scheduler run function
	// runs the schedule
	// **********************************
uint8_t Scheduler::run(void)
{
    uint8_t l_i8NextTaskSlot = 0;
    Task * l_pNextTask = (uintptr_t) 0;
    uint8_t l_u8ReturnCode = NO_ERR;

    while(l_i8NextTaskSlot<NUMBER_OF_SLOTS)
    {
        l_pNextTask = static_cast<Task *> (m_sCurrentSchedule[l_i8NextTaskSlot].m_pSlotTask);
        if(l_pNextTask != ((uintptr_t) 0))
        {
        	l_pNextTask->run();
            l_i8NextTaskSlot++;
        }
        else
        {
           break;
        }
    }
    clean(0);
    return l_u8ReturnCode;
}
// **********************************
	// scheduler clean function
	// 	cleans the current schedule
	// **********************************
void Scheduler::clean(uint8_t i_i8InputIndex)
{
	for(uint8_t index = i_i8InputIndex; index <= (NUMBER_OF_SLOTS); index++)
	    {
	       if(m_sCurrentSchedule[i_i8InputIndex].m_pSlotTask==NULL)
	       	   {
	    	   break;
	       	   }
	       else
	       	   {
	    	   m_sCurrentSchedule[i_i8InputIndex].m_pSlotTask= (uintptr_t) 0;
	       	   }
	       }
	return;
}

// **********************************
	// scheduler calculate schedule function
	// Calculates the scheduler based on task properties
	// Supported types:
	// 	- One shot task
	//  - Timed one shot task
	// - Ever present task
	// - Periodic task
	// **********************************
uint8_t Scheduler::CalculateSchedule(void)
{
	uint8_t l_i8NextTaskSlot = 0U;
    uint8_t l_u8ReturnCode = NO_ERR;
	while(l_i8NextTaskSlot <= (NUMBER_OF_SLOTS))
	{
		if(m_sEventsSchedule[l_i8NextTaskSlot].m_pSlotTask==NULL)
		{
		    clean(l_i8NextTaskSlot);
			break ;
		}
		if(m_sEventsSchedule[l_i8NextTaskSlot].m_i64RequiredTicks==0U)
			{
				if(m_sEventsSchedule[l_i8NextTaskSlot].m_BIsOneShot==false)
					{m_sCurrentSchedule[l_i8NextTaskSlot]=m_sEventsSchedule[l_i8NextTaskSlot];} //- Ever present task
				else
					{

					if(m_sEventsSchedule[l_i8NextTaskSlot].m_BExecuted==false)
					{
						m_sCurrentSchedule[l_i8NextTaskSlot]=m_sEventsSchedule[l_i8NextTaskSlot];// One shot task
						m_sEventsSchedule[l_i8NextTaskSlot].m_BExecuted=true;
					}
					}
			}
		else
			{	m_sEventsSchedule[l_i8NextTaskSlot].m_i64CurrentTicks=m_i64Ticks;
				if(abs(m_sEventsSchedule[l_i8NextTaskSlot].m_i64CurrentTicks-m_sEventsSchedule[l_i8NextTaskSlot].m_i64LastTicks)==m_sEventsSchedule[l_i8NextTaskSlot].m_i64RequiredTicks)
				{

					m_sEventsSchedule[l_i8NextTaskSlot].m_i64LastTicks=m_i64Ticks;

					if(m_sEventsSchedule[l_i8NextTaskSlot].m_BIsOneShot==false)
										{m_sCurrentSchedule[l_i8NextTaskSlot]=static_cast<Slot>(m_sEventsSchedule[l_i8NextTaskSlot]);}// - Periodic task (assigned based on difference between two counters)
									else																							  // Done this way to prevent overflow related issues
										{

										if(m_sEventsSchedule[l_i8NextTaskSlot].m_BExecuted==false)
										{
											m_sCurrentSchedule[l_i8NextTaskSlot]=m_sEventsSchedule[l_i8NextTaskSlot];//- Timed one shot task
											m_sEventsSchedule[l_i8NextTaskSlot].m_BExecuted=true;
										}
										}
				}

			}
		l_i8NextTaskSlot++;
	}
	return l_u8ReturnCode;
}
// **********************************
	// scheduler attach Message function
	// attaches messages to the scheduler's message queue
	// **********************************
uint8_t Scheduler::attachMessage(uint8_t i_i8Source,uint8_t i_i8Destination,bool i_bMessageType,uint64_t i_i64Data)
{
	uint8_t l_u8ReturnCode=NO_ERR;
	if((m_i8OpenSlotsMessages>0) && (m_i8NextSlotMessages < NUMBER_OF_SLOTS))
	    {
	        m_mMessageQueue[m_i8NextSlotMessages].m_i8Source= i_i8Source;
	        m_mMessageQueue[m_i8NextSlotMessages].m_i8Destination=i_i8Destination;
	        m_mMessageQueue[m_i8NextSlotMessages].m_i64Data=i_i64Data;
	        m_mMessageQueue[m_i8NextSlotMessages].m_bMessageType=i_bMessageType;
	        m_mMessageQueue[m_i8NextSlotMessages].m_bMessageWritten=true;
	        m_i8OpenSlotsMessages--;
	        m_i8NextSlotMessages++;
	    }
	    else
	    {
	    	l_u8ReturnCode = RET_ERR;
	    }

	return l_u8ReturnCode;


}
// **********************************
	// scheduler mailman function
	// handles the message queue:
	// two types of messages supported:
	//	-Scheduler bound message
	//  -Task bound message
	// **********************************
uint8_t Scheduler::mailMan(void)
{
	uint8_t l_u8ReturnCode=NO_ERR;
	for(int8_t index=0U;index<=NUMBER_OF_SLOTS;index++)
	{
		if(m_sEventsSchedule[index].m_pSlotTask==NULL)
			{
				break;
			}
		else
			{
				for(int8_t indexM=0U;index<=NUMBER_OF_MESSAGES;indexM++)
					{
						if(m_mMessageQueue[indexM].m_bMessageWritten==true)
							{

							if(m_mMessageQueue[indexM].m_bMessageType==false)
								{
									if(m_mMessageQueue[indexM].m_i8Destination==m_sEventsSchedule[index].m_pSlotTask->m_u8TaskID)
										{runMessage(index,indexM);} //	-Scheduler bound message

								}
							else
								{
									if(m_mMessageQueue[indexM].m_i8Destination==m_sEventsSchedule[index].m_pSlotTask->m_u8TaskID)
									{m_sEventsSchedule[index].m_pSlotTask->readMessage(m_mMessageQueue[indexM].m_i8Source,m_mMessageQueue[indexM].m_i64Data);} //  -Task bound message

								}
							}
						else
							{
								break;
							}
					}
				}
	}
	cleanMail();
	return l_u8ReturnCode;
}

// **********************************
	// scheduler clean mail function
	// cleans the message queue
	// **********************************
void Scheduler::cleanMail(void)
{
	m_i8OpenSlotsMessages=static_cast<uint8_t>(NUMBER_OF_MESSAGES);
	m_i8NextSlotMessages=0;
	for(int8_t index=0;index<=NUMBER_OF_MESSAGES;index++)
		{
		m_mMessageQueue[index].m_bMessageWritten=false;
		}



}
	// **********************************
	// scheduler runMessage function
	// Reads messages send to the schedulere
	// Two nested switches for messages' destination and source, respectively
	// **********************************
void Scheduler::runMessage(uint8_t i_i8TaskIndex,uint8_t i_i8MsgIndex )
{
//switch between each source
	switch(m_mMessageQueue[i_i8MsgIndex].m_i8Destination)
	{
		case 0: //LED0
					switch(m_mMessageQueue[i_i8MsgIndex].m_i8Source)//switch between each source
					{
					case 0:
							 m_sEventsSchedule[i_i8TaskIndex].m_i64RequiredTicks=m_mMessageQueue[i_i8MsgIndex].m_i64Data; //Button 1
							 m_sEventsSchedule[i_i8TaskIndex].m_i64LastTicks=m_sEventsSchedule[i_i8TaskIndex].m_i64CurrentTicks;
							 break;

					default:
							break;
					}
					break;

		case 1: //LED1
							switch(m_mMessageQueue[i_i8MsgIndex].m_i8Source)
							{
							case 0:
									m_sEventsSchedule[i_i8TaskIndex].m_i64RequiredTicks=m_mMessageQueue[i_i8MsgIndex].m_i64Data; //Button 1
									m_sEventsSchedule[i_i8TaskIndex].m_i64LastTicks=m_sEventsSchedule[i_i8TaskIndex].m_i64CurrentTicks;
									break;
							default:
								break;
							}
							break;

		default:
			break;



}
}
	// **********************************
	// scheduler sort by priority function
	// Coming soon
	// **********************************
uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
