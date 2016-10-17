/*
 * Struct_Task.hpp
 *
 *  Created on: 12 de cot. de 2016
 *      Author: LuisD
 *
 *
 *
 */
#ifndef STRUCT_SLOT_HPP_
#define STRUCT_SLOT_HPP_
#define __NOP __nop
#include "msp.h"

struct Message
{
	uint8_t source;
	uint8_t destination;
	bool messageType;
	uint8_t data;
};


#endif /* STRUCT_TASK_HPP_ */




