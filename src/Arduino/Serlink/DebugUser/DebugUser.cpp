/*
 * DebugUser.cpp
 *
 *  Created on: 30 Apr 2023
 *      Author: rw123
 */

//#include "DebugPrint.hpp"
#include "DebugUser.hpp"
#include "Global.hpp"

void DebugUser::debugWrite(char* str)
{
	if(this->debugOn)
	{
		debugPrint->writeLine(str, this->debugLevel);
	}
}

