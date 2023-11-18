/*
 * env_config.h
 *
 *  Created on: 25 Jun 2023
 *      Author: rw123
 */

#ifndef ENV_CONFIG_H_
#define ENV_CONFIG_H_

//----------------------------------------------------
// System (i.e. run time) type - ONLY ONE should be defined

#define ENV_CONFIG__SYSTEM_PC 0               // Code is run in eclipse on a PC
//#define ENV_CONFIG__SYSTEM_ARDUINO_UNO 1    // Code is run on an Arduino Uno
//----------------------------------------------------

#define ENV_CONFIG__SYSTEM ENV_CONFIG__SYSTEM_ARDUINO_UNO

#if defined(ENV_CONFIG__SYSTEM_ARDUINO_UNO)
#include <avr/io.h>
#include "wiring_private.h"
#endif
#if defined(ENV_CONFIG__SYSTEM_PC)
#include "ATmega328.hpp"
#endif

#endif /* ENV_CONFIG_H_ */
