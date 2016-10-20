/*
 * Struct_Task.hpp
 *
 *  Created on: 28 de set. de 2016
 *      Author: JosePablo
 *
 *
 *
 */
#ifndef STRUCT_SLOT_HPP_
#define STRUCT_SLOT_HPP_
#define __NOP __nop
#include "msp.h"
#include "Task.hpp"

struct Slot
{
	Task * slotTask;
	uint64_t required_ticks;
	uint64_t current_ticks;
	uint64_t last_ticks;
	bool isOneShot;
	bool executed;
};


#endif /* STRUCT_TASK_HPP_ */




