/*
 * Reader_config.hpp
 *
 *  Created on: 23 Apr 2023
 *      Author: rw123
 */

#ifndef READER_CONFIG_HPP_
#define READER_CONFIG_HPP_

#include "env.h"
//#include "Reader.hpp"

#define READER_CONFIG__READER0

#define READER_CONFIG__MAX_NUM_INSTANT_HANDLERS 5

//----------------------------------------------------------
// READER0
#ifdef READER_CONFIG__READER0

#include "uart_wrapper.hpp"

#define READER_CONFIG__READER0_ID 1 // READER0 instance id

//#define READER_CONFIG__READER0_UART_INIT uart_init

//#define READER_CONFIG__READER0_UART_CHECKFRAMERX uart_checkFrameRx

//#define READER_CONFIG__READER0_UART_GETRXLENANDRESET uart_getRxLenAndReset

#endif
//----------------------------------------------------------

#endif /* READER_CONFIG_HPP_ */
