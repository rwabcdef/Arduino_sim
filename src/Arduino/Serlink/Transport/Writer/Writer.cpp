/*
 * Writer.cpp
 *
 *  Created on: 31 Oct 2023
 *      Author: rw123
 */

#include "Writer.hpp"
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
  this->status = Writer::STATUS_IDLE;
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

// Used to send frame (called from app layer above)
uint8_t Writer::sendFrame(Frame& frame, bool ack)
{
  this->txFlag = true;
  this->txAck = ack;
  frame.copy(&this->txFrame);
}

// Used to check tx status (called from app layer above)
uint8_t Writer::getStatus()
{
  return this->status;
}
  // Called by a Reader to pass an ack frame to the Writer.
void Writer::setAckFrame(Frame& frame)
{
  this->ackRxFlag = true;
  frame.copy(&this->ackRxFrame);
}

//----------------------------------------------------------------
// start of state methods
uint8_t Writer::idle()
{
  if(this->txFlag)
  {
    this->txFlag = false;

    uint8_t ret;
    this->txFrame.toString((char*)this->txBuffer, &ret);

    this->status = Writer::STATUS_BUSY;

    this->uartWrite((char*)this->txBuffer);

    return TXWAIT;
  }

  return IDLE;
}
uint8_t Writer::txWait()
{
  if(this->getUartTxBusy())
  {
    return TXWAIT;
  }
  else
  {
    if(this->txFrame.type == Frame::TYPE_UNIDIRECTION)
    {
      this->status = Writer::STATUS_IDLE;
      return IDLE;
    }
    else
    {
      swTimer_tickReset(&this->startTick);
      return RXACKWAIT;
    }
  }
}
uint8_t Writer::rxAckWait()
{
  if(this->ackRxFlag)
  {
    this->ackRxFlag = false;

    if(0 == strncmp(this->ackRxFrame.protocol, this->txFrame.protocol, Frame::LEN_PROTOCOL))
    {
      this->status = Writer::STATUS_OK;
      return IDLE;
    }
    else
    {
      this->status = Writer::STATUS_PROTOCOL_ERROR;
      return IDLE;
    }
  }

  if(swTimer_tickCheckTimeout(&this->startTick, 1000))
  {
    this->status = Writer::STATUS_TIMEOUT;
    return IDLE;
  }

  return RXACKWAIT;
}
// end of state methods
//----------------------------------------------------------------

uint8_t Writer::uartWrite(char* buffer)
{
#ifdef WRITER_CONFIG__WRITER0

  if(this->id == WRITER_CONFIG__WRITER0_ID)
  {
    return uart_write(buffer);
  }

#endif
}
bool Writer::getUartTxBusy()
{
#ifdef WRITER_CONFIG__WRITER0

  if(this->id == WRITER_CONFIG__WRITER0_ID)
  {
    return uart_getTxBusy();
  }

#endif
}
