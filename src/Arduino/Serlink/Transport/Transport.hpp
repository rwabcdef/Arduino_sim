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
#include "Transport_config.hpp"
#include "StateMachine.hpp"
#if defined(TRANSPORT_CONFIG__SOCKETS_ENABLED)
#include "Socket.hpp"
#endif

namespace SerLink
{

class TransportData  // probably not needed
{
public:
	char data[UART_BUFF_LEN];
	uint8_t dataLen;
};

class Transport : public StateMachine, public DebugUser
{
private:
#if defined(TRANSPORT_CONFIG__NUM_SOCKETS_MAX)
  Socket socket[TRANSPORT_CONFIG__NUM_SOCKETS_MAX];
  uint8_t currentTxSocketIndex;
  uint8_t socketCount;
#endif
  Reader* reader;
  Writer* writer;
	uint16_t startTick;
	//uint16_t txDataLen;
	//char txData[UART_BUFF_LEN];
	Frame rxFrame, txFrame;

	uint8_t idle();
	uint8_t txWait();
	uint8_t postTxWait();

public:
	Transport(Reader* reader, Writer* writer);
	void run();
	void commonBehaviour();

#if defined(TRANSPORT_CONFIG__SOCKETS_ENABLED)

  bool acquireSocket(Socket* socket, char* protocol,
      readHandler instantReadHandler = nullptr, uint16_t startRollCode = 0);

  int8_t findSocketIndex(char* protocol);

#endif
};

}




#endif /* TRANSPORT_HPP_ */
