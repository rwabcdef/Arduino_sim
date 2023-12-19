/*
 * Transport.cpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */

#include<string.h>
#include "Transport.hpp"
#include "swTimer.h"

#define IDLE 0
#define TXWAIT 1
#define POST_TXWAIT 2

using namespace SerLink;

Transport::Transport(Reader* reader, Writer* writer):
reader(reader), writer(writer)
{
  this->socketCount = 0;
  this->currentState = IDLE;
}
bool Transport::acquireSocket(Socket* socket)
{
  if(this->socketCount < (TRANSPORT_CONFIG__NUM_SOCKETS_MAX - 1))
  {
    // One or more sockets are free.
    socket = &this->socket[this->socketCount];
    this->socketCount++;
    return true;
  }
  else
  {
    // All sockets are already in use
    return false;
  }
}

int8_t Transport::findSocketIndex(char* protocol)
{
  uint8_t i;

  for(i=0; i<this->socketCount; i++)
  {
    if(this->socket[i].getActive())
    {
      if(0 == strncmp(this->socket[i].getProtocol(), protocol, Frame::LEN_PROTOCOL))
      {
        // socket found.
        return i;
      }
    }
  }
  // socket not found.
  return -1;
}

// Common behaviour for all states.
void Transport::commonBehaviour()
{
  if(this->reader->getRxFrame(this->rxFrame))
  {
    // A frame has been received
    int8_t socketIndex = this->findSocketIndex(this->rxFrame.protocol);

    if(socketIndex >= 0)
    {
      // socket found - pass it up to the relevant socket
      this->socket[socketIndex].setRxFrame(this->rxFrame);
    }
  }
}

void Transport::run()
{
  switch(this->currentState)
  {
    case IDLE: { this->currentState = this->idle(); break;}
    case TXWAIT: { this->currentState = this->txWait(); break;}
    case POST_TXWAIT: { this->currentState = this->postTxWait(); break;}
  }
}
//----------------------------------------------------------------
// start of state methods

uint8_t Transport::idle()
{
  uint8_t i;

  for(i=0; i<this->socketCount; i++)
  {
    if(this->socket[i].getActive())
    {
      if(this->socket[i].getTxFrame(this->txFrame))
      {
        this->writer->sendFrame(this->txFrame);
        return TXWAIT;
      }
    }
  }
  return IDLE;
}

uint8_t Transport::txWait()
{
  uint8_t status = this->writer->getStatus();
  if(status == SerLink::Writer::STATUS_BUSY)
  {
    return TXWAIT;
  }
  else
  {
    swTimer_tickReset(&this->startTick);
    return POST_TXWAIT;
  }
}

uint8_t Transport::postTxWait()
{
  if(swTimer_tickCheckTimeout(&this->startTick, 100))
  {
    return IDLE;
  }
  return POST_TXWAIT;
}

