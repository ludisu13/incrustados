/*
 * Struct_Task.hpp
 *
 *  Created on: 12 de cot. de 2016
 *      Author: LuisD
 *
 *
 *
 */
#ifndef STRUCT_MESSAGE_HPP_
#define STRUCT_MESSAGE_HPP_
#define __NOP __nop
#include "msp.h"

struct Message
{
	uint8_t m_i8Source;
	uint8_t m_i8Destination;
	bool m_bMessageType;
	uint64_t m_i64Data;
	bool m_bMessageWritten;
};


#endif /* STRUCT_TASK_HPP_ */




