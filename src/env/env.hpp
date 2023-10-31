/*
 * env.hpp
 *
 *  Created on: 25 Jun 2023
 *      Author: rw123
 */

#ifndef ENV_HPP_
#define ENV_HPP_

#include "env_config.h"

#define ENV_DEBUG_STR_LEN 256

//-----------------------------------
// Debug levels
#define ENV_CONFIG__DEBUG_LEVELS_OVERRIDE
namespace DebugPrint_defs{
static const char* const LevelLabels[] = {"ZERO ", "ONE  ", "MAIN ",
		"TMR0 ", "DIGIN", "EV0  ", "URX  ", "UTX  ", "BUTT0", "UTST0"};

enum Levels
{
	Null = -1,
	Zero = 0,
	One,
	Main,
	Timer0,
	DigIn,
	Event0,
	UartRx,
	UartTx,
	Butt0,
	UnitTest0
};

}
//-----------------------------------

#endif /* ENV_HPP_ */
