/*
 * Transport.hpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */

#ifndef TRANSPORT_HPP_
#define TRANSPORT_HPP_

#include <stdint.h>
#include "uart_wrapper.hpp"

namespace SerLink
{

class TransportData
{
public:
	char data[UART_BUFF_LEN];
	uint8_t dataLen;
};

class Transport
{
private:
	uint16_t startTick;
};

}




#endif /* TRANSPORT_HPP_ */
