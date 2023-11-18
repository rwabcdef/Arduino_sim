/*
 * Socket.hpp
 *
 *  Created on: 18 Nov 2023
 *      Author: rw123
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include "Socket_config.hpp"
#include "Writer.hpp"
#include "Reader.hpp"
#include "StateMachine.hpp"
#include "Frame.hpp"
#include "DebugUser.hpp"

namespace SerLink
{

class Socket : public StateMachine, public DebugUser
{
private:
  Writer* writer;
  Reader* reader;
  readHandler instantReadHandler;
  Frame rxFrame, txFrame;
  char protocol[Frame::LEN_PROTOCOL];
  uint16_t txRollCode;

public:
  Socket(Writer* writer, Reader* reader);
  void init(char* protocol, readHandler instantReadHandler = nullptr, uint16_t startRollCode = 0);

  //-------------------------------------------
  // Upper (i.e. application) Interface

  bool getRxData(char* data, uint8_t* dataLen);
  uint8_t sendData(char* data, uint16_t dataLen, bool ack);
  uint8_t getSendStatus();
  //-------------------------------------------
};

} // namespace SerLink



#endif /* SOCKET_HPP_ */
