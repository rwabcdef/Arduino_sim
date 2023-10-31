/*
 * Writer.cpp
 *
 *  Created on: 31 Oct 2023
 *      Author: rw123
 */

#include "Reader.hpp"
#include "ATmega328.hpp"
#include "swTimer.h"
#include <string.h>
#include <stdio.h>

#define IDLE 0
#define TXWAIT 1
#define RXACKWAIT 2

using namespace SerLink;

Writer::Writer(uint8_t id): id(id)
{
  this->txFlag = false;
  this->ackRxFlag = false;
  this->txAck = false;
  this->bufferLen = UART_BUFF_LEN;
  this->currentState = IDLE;
  this->debugLevel = DebugPrint_defs::UartRx;
  this->debugOn = true;
}

void Writer::run()
{
  switch(this->currentState)
    {
      case IDLE: { this->currentState = this->idle(); break;}
      case TXWAIT: { this->currentState = this->txWait(); break;}
      case RXACKWAIT: { this->currentState = this->rxAckWait(); break;}
    }
}

  // Used to send frame
uint8_t Writer::sendFrame(Frame& frame, bool ack)
{
  this->txFlag = true;
  this->txAck = ack;
  frame.copy(&this->txFrame);
}

  // Called by a Reader to pass an ack frame to the Writer.
void Writer::setAckFrame(Frame& frame)
{
  this->ackRxFlag = true;
  frame.copy(&this->ackRxFrame);
}


uint8_t Writer::idle()
{

}
uint8_t Writer::txWait()
{

}
uint8_t Writer::rxAckWait()
{

}
