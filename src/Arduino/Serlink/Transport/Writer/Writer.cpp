/*
 * Writer.cpp
 *
 *  Created on: 31 Oct 2023
 *      Author: rw123
 */

#include "Writer.hpp"
#if defined(ENV_CONFIG__SYSTEM_PC)
#include "ATmega328.hpp"
#endif
#include "swTimer.h"
#include <string.h>
#include <stdio.h>

#define IDLE 0
#define TXWAIT 1
#define RXACKWAIT 2

using namespace SerLink;

Writer::Writer(uint8_t id, char* txBuffer, uint8_t bufferLen): id(id), DebugUser()
{
  this->txFlag = false;
  this->ackRxFlag = false;
  this->txBuffer = txBuffer;
  this->bufferLen = bufferLen;
  this->currentState = IDLE;
  #if defined (ENV_CONFIG__SYSTEM_PC)
  this->debugLevel = DebugPrint_defs::Writer0;
  #endif
  this->status = Writer::STATUS_IDLE;
  this->flag = 0;
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
uint8_t Writer::sendFrame(Frame& frame)
{
  this->txFlag = true;
  frame.copy(&this->txFrame);
  return 0;
}

// Used to check tx status (called from app layer above)
uint8_t Writer::getStatus()
{
  uint8_t status = this->status;
  this->status = Writer::STATUS_IDLE; // clear status
  return status;
}

void Writer::getStatusStr(char* str)
{
  switch(this->status)
  {
    case STATUS_IDLE: sprintf(str, "idle\0", 0); break;
    case STATUS_BUSY: sprintf(str, "busy\0", 0); break;
    case STATUS_OK: sprintf(str, "ok\0", 0); break;
    case STATUS_TIMEOUT: sprintf(str, "timeout\0", 0); break;
    case STATUS_PROTOCOL_ERROR: sprintf(str, "protocol error\0", 0); break;
  }
}

  // Called by a Reader to pass an ack frame to the Writer.
void Writer::setAckFrame(Frame& frame)
{
  frame.copy(&this->ackRxFrame);
  this->ackRxFlag = true;
  //this->debugWrite("writer ack");
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
    //return IDLE;

    if(this->txFrame.type == Frame::TYPE_UNIDIRECTION)
    {
      this->status = Writer::STATUS_IDLE;
      return IDLE;
    }
    else
    {
      //sprintf(this->s, "writer ack wait", 0);
      //this->debugWrite(this->s);

      swTimer_tickReset(&this->startTick);
      return RXACKWAIT;
    }
  }
}
uint8_t Writer::rxAckWait()
{
  if(true == this->ackRxFlag)
  {
    this->ackRxFlag = false;

    sprintf(this->s, "writer rx ack", 0);
    this->debugWrite(this->s);

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

  if(swTimer_tickCheckTimeout(&this->startTick, 2000))
  {
    this->status = Writer::STATUS_TIMEOUT;
    //printf("timeout\n");
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
