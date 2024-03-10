/*
 * Socket.cpp
 *
 *  Created on: 18 Nov 2023
 *      Author: rw123
 */

#include "Socket.hpp"
#include "swTimer.h"
#include "string.h"

//SerLink::Socket::Socket(Writer* writer, Reader* reader): writer(writer), reader(reader)
SerLink::Socket::Socket()
{
  this->active = false;
}

void SerLink::Socket::init(char* protocol, readHandler instantReadHandler, uint16_t startRollCode)
{
  this->active = true;
  this->txFlag = false;
  this->rxFlag = false;
  //this->txBusy = false;
  this->txStatus = TX_STATUS_IDLE;
  strncpy(this->protocol, protocol, Frame::LEN_PROTOCOL);
  //this->instantReadHandler = instantReadHandler;
  this->txRollCode = startRollCode;
}

//------------------------------------------------------------------------------
// Upper (i.e. application) Interface

bool SerLink::Socket::getRxData(char* data, uint8_t* dataLen)
{
  if(this->rxFlag)
  {
    this->rxFlag = false;
    *dataLen = this->rxFrame.dataLen;
    strncpy(data, this->rxFrame.data, this->rxFrame.dataLen);
    return true;
  }
  return false;
}

bool SerLink::Socket::sendData(char* data, uint16_t dataLen, bool ack)
{
  if(!this->active)
  {
    return false;
  }

  if(this->txStatus != TX_STATUS_IDLE)
  {
    // Tx is already in progress
    return false;
  }
  else
  {
    // No Tx is currently in progress.
    this->txFlag = true;
    this->txStatus = TX_STATUS_BUSY;

    this->txFrame.setProtocol(this->protocol);
    if(ack)
    {
      this->txFrame.type = Frame::TYPE_TRANSMISSION;
    }
    else
    {
      this->txFrame.type = Frame::TYPE_UNIDIRECTION;
    }
    this->txFrame.rollCode = this->txRollCode;
    Frame::incRollCode(&this->txRollCode);
    this->txFrame.dataLen = dataLen;
    strncpy(this->txFrame.data, data, dataLen);

    return true;
  }
}

uint8_t SerLink::Socket::getAndClearSendStatus()
{
  if(this->txStatus == TX_STATUS_BUSY)
  {
    return this->txStatus;
  }
  else
  {
    uint8_t status = this->txStatus;
    this->txStatus = TX_STATUS_IDLE; // clear status
    return status;
  }
}

//------------------------------------------------------------------------------
// Lower (i.e. transport) Interface

bool SerLink::Socket::getTxFrame(Frame& txFrame)
{
  if(this->txFlag)
  {
    this->txFrame.copy(&txFrame);
    this->txFlag = false;
    //this->txBusy = true;
    return true;
  }
  else
  {
    return false;
  }
}

void SerLink::Socket::setWriterDoneStatus(uint8_t status)
{
  if(status == Writer::STATUS_IDLE)
  {
    this->txStatus = TX_STATUS_IDLE;
  }
  else if(status >= Writer::STATUS_OK)
  {
    // Writer has finished sending Frame
    this->txStatus = status;
  }
}

void SerLink::Socket::setRxFrame(Frame& rxFrame)
{
  if(!this->active)
  {
    return;
  }

  this->rxFlag = true;
  rxFrame.copy(&this->rxFrame);
}
//------------------------------------------------------------------------------
