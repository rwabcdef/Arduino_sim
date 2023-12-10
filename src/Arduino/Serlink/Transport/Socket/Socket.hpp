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
  //Writer* writer;
  //Reader* reader;
  bool active;
  bool txFlag;
  //bool txBusy;
  bool rxFlag;
  uint8_t txStatus;
  readHandler instantReadHandler;
  Frame rxFrame, txFrame;
  char protocol[Frame::LEN_PROTOCOL];
  uint16_t txRollCode;

public:
  const uint8_t TX_STATUS_IDLE = 5;
  const uint8_t TX_STATUS_BUSY = 6;

  //Socket(Writer* writer, Reader* reader);
  Socket();
  void init(char* protocol, readHandler instantReadHandler = nullptr, uint16_t startRollCode = 0);

  //-------------------------------------------
  // Upper (i.e. application) Interface

  bool getRxData(char* data, uint8_t* dataLen);

  bool sendData(char* data, uint16_t dataLen, bool ack);

  uint8_t getAndClearSendStatus();

  //-------------------------------------------
  // Lower (i.e. transport) Interface

  // Called by transport layer, to see if there is a frame to be sent from this Socket.
  bool getTxFrame(Frame& txFrame);

  // Called by transport layer when writer has finished sending frame
  void setWriterDoneStatus(uint8_t status);

  // Called by transport layer when reader has a frame for this Socket.
  void setRxFrame(Frame& rxFrame);
  //-------------------------------------------
};

} // namespace SerLink



#endif /* SOCKET_HPP_ */
