/*
 * Writer.hpp
 *
 *  Created on: 12 Jul 2023
 *      Author: rw123
 */

#ifndef WRITER_WRITER_HPP_
#define WRITER_WRITER_HPP_

#include <stdint.h>
#include "Writer_config.hpp"
#include "StateMachine.hpp"
#include "Frame.hpp"
#include "Transport.hpp"
#include "DebugPrint.hpp"
#include "DebugUser.hpp"

namespace SerLink
{
class Writer : public StateMachine, public DebugUser
{
private:
  uint8_t id;
  uint16_t startTick;
  bool txFlag;
  bool ackRxFlag;
  bool txAck;
  char txBuffer[UART_BUFF_LEN];
  char s[64];
  uint8_t bufferLen;
  Frame txFrame;
  Frame ackRxFrame;

  uint8_t idle();
  uint8_t txWait();
  uint8_t rxAckWait();

public:
  Writer(uint8_t id);
  void run();

  // Used to send frame
  uint8_t sendFrame(Frame& frame, bool ack = true);

  // Called by a Reader to pass an ack frame to the Writer.
  void setAckFrame(Frame& frame);
};

} // end namespace SerLink

#endif /* WRITER_WRITER_HPP_ */
