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
	Task * m_pSlotTask;
	uint64_t m_i64RequiredTicks;
	uint64_t m_i64CurrentTicks;
	uint64_t m_i64LastTicks;
	bool m_BIsOneShot;
	bool m_BExecuted;
};


#endif /* STRUCT_TASK_HPP_ */




