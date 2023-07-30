/*
 * Global.hpp
 *
 *  Created on: 25 Jun 2023
 *      Author: rw123
 */

#ifndef GLOBAL_HPP_
#define GLOBAL_HPP_


#include "DebugPrint.hpp"
#include "SimClk.hpp"
#include "InterruptRunner.hpp"

extern DebugPrint *debugPrint;
extern SimClk *simClk;
extern InterruptRunner *interruptRunner;
extern volatile uint32_t g_simClk_uS;


#endif /* GLOBAL_HPP_ */
