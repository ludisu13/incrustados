#include "Scheduler.hpp"

Scheduler::Scheduler()
{
	ticks=0;
    mOpenSlots = static_cast<uint8_t>(NUMBER_OF_SLOTS);
    mNextSlot = 0;
    mOpenSlotsMessages=static_cast<uint8_t>(NUMBER_OF_MESSAGES);
    mNextSlotMessages=0;
    for(uint8_t index = 0; index < (NUMBER_OF_SLOTS); index++)
    {
        EventsSchedule[index].slotTask = (uintptr_t) 0;
        CurrentSchedule[index].slotTask= (uintptr_t) 0;
        EventsSchedule[index].isOneShot=false;
        EventsSchedule[index].executed=false;

    }
    for(uint8_t index=0;index <=NUMBER_OF_MESSAGES;index++)
    {
    	MessageQueue[index].messageWritten=false;
    }
    return;
}

uint8_t Scheduler::attach(Task * i_ToAttach,bool shot)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        EventsSchedule[mNextSlot].slotTask = i_ToAttach;
        EventsSchedule[mNextSlot].required_ticks=1U;
        EventsSchedule[mNextSlot].isOneShot=shot;
        mOpenSlots--;
        mNextSlot++;
    }
    else
    {
        l_ErrorCode = RET_ERR;
    }
    return l_ErrorCode;
}

uint8_t Scheduler::attach(Task * i_ToAttach, uint64_t i_u64TickInterval,bool shot)
{
    uint8_t l_ErrorCode = NO_ERR;
    if((mOpenSlots>0) && (mNextSlot < NUMBER_OF_SLOTS))
    {
        EventsSchedule[mNextSlot].slotTask = i_ToAttach;
        EventsSchedule[mNextSlot].required_ticks=i_u64TickInterval;
        EventsSchedule[mNextSlot].last_ticks=0U;
        EventsSchedule[mNextSlot].isOneShot=shot;
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
				if(EventsSchedule[NextTaskSlot].isOneShot==false)
					{CurrentSchedule[NextTaskSlot]=EventsSchedule[NextTaskSlot];}
				else
					{

					if(EventsSchedule[NextTaskSlot].executed==false)
					{
						CurrentSchedule[NextTaskSlot]=EventsSchedule[NextTaskSlot];
						EventsSchedule[NextTaskSlot].executed=true;
					}
					}
			}
		else
			{	EventsSchedule[NextTaskSlot].current_ticks=ticks;
				if(abs(EventsSchedule[NextTaskSlot].current_ticks-EventsSchedule[NextTaskSlot].last_ticks)==EventsSchedule[NextTaskSlot].required_ticks)
				{

					EventsSchedule[NextTaskSlot].last_ticks=ticks;

					if(EventsSchedule[NextTaskSlot].isOneShot==false)
										{CurrentSchedule[NextTaskSlot]=static_cast<Slot>(EventsSchedule[NextTaskSlot]);}
									else
										{

										if(EventsSchedule[NextTaskSlot].executed==false)
										{
											CurrentSchedule[NextTaskSlot]=EventsSchedule[NextTaskSlot];
											EventsSchedule[NextTaskSlot].executed=true;
										}
										}
				}

			}
		NextTaskSlot++;
	}
	return l_u8ReturnCode;
}
uint8_t Scheduler::attachMessage(uint8_t source,uint8_t destination,bool messageType,uint64_t data)
{
	uint8_t l_u8ReturnCode=NO_ERR;
	if((mOpenSlotsMessages>0) && (mNextSlotMessages < NUMBER_OF_SLOTS))
	    {
	        MessageQueue[mNextSlotMessages].source=source;
	        MessageQueue[mNextSlotMessages].destination=destination;
	        MessageQueue[mNextSlotMessages].data=data;
	        MessageQueue[mNextSlotMessages].messageType=messageType;
	        MessageQueue[mNextSlotMessages].messageWritten=true;
	        mOpenSlotsMessages--;
	        mNextSlotMessages++;
	    }
	    else
	    {
	    	l_u8ReturnCode = RET_ERR;
	    }

	return l_u8ReturnCode;


}

uint8_t Scheduler::mailMan(void)
{	uint8_t lastMessageProcessed=0U;
	uint8_t l_u8ReturnCode=NO_ERR;
	for(int8_t index=0U;index<=NUMBER_OF_SLOTS;index++)
	{
		if(EventsSchedule[index].slotTask==NULL)
			{
				break;
			}
		else
			{
				for(int8_t indexM=lastMessageProcessed;index<=NUMBER_OF_MESSAGES;indexM++)
					{
						if(MessageQueue[indexM].messageWritten==true)
							{
							lastMessageProcessed++;
							if(MessageQueue[indexM].messageType==false)
								{
									if(MessageQueue[indexM].destination==EventsSchedule[index].slotTask->m_u8TaskID)
										{runMessage(index,indexM);}
									//else{break;}
								}
							else
								{
									if(MessageQueue[indexM].destination==EventsSchedule[index].slotTask->m_u8TaskID)
									{EventsSchedule[index].slotTask->readMessage(MessageQueue[indexM].source,MessageQueue[indexM].data);}
									//else{break;}
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


void Scheduler::cleanMail(void)
{
	mOpenSlotsMessages=static_cast<uint8_t>(NUMBER_OF_MESSAGES);
	mNextSlotMessages=0;
	for(int8_t index=0;index<=NUMBER_OF_MESSAGES;index++)
		{
		MessageQueue[index].messageWritten=false;
		}



}

void Scheduler::runMessage(uint8_t taskIndex,uint8_t msgIndex )
{
//switch entre cada caso
	switch(MessageQueue[msgIndex].destination)
	{
		case 0: //LED0
					switch(MessageQueue[msgIndex].source)
					{
					case 0:
							 EventsSchedule[taskIndex].required_ticks=MessageQueue[msgIndex].data; //BOTON 1
							 EventsSchedule[taskIndex].last_ticks=EventsSchedule[taskIndex].current_ticks;
							 break;

					default:
							break;
					}
					break;

		case 1: //LED1
							switch(MessageQueue[msgIndex].source)
							{
							case 0:
									EventsSchedule[taskIndex].required_ticks=MessageQueue[msgIndex].data; //BOTON 1
									EventsSchedule[taskIndex].last_ticks=EventsSchedule[taskIndex].current_ticks;
									break;
							default:
								break;
							}
							break;

		default:
			break;



}
}

uint8_t Scheduler::SortScheduleByPriority(Task * i_pSchedule)
{
    return(NO_ERR);
}
