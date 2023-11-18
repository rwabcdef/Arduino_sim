/*
 * Socket.cpp
 *
 *  Created on: 18 Nov 2023
 *      Author: rw123
 */

#include "Socket.hpp"
#include "string.h"

SerLink::Socket::Socket(Writer* writer, Reader* reader)
: writer(writer), reader(reader)
{

}

void SerLink::Socket::init(char* protocol, readHandler instantReadHandler, uint16_t startRollCode)
{
  strncpy(this->protocol, protocol, Frame::LEN_PROTOCOL);
  this->instantReadHandler = instantReadHandler;
  this->txRollCode = startRollCode;
}

bool SerLink::Socket::getRxData(char* data, uint8_t* dataLen)
{
  if(this->reader->getRxFrame(this->rxFrame))
  {
    *dataLen = this->rxFrame.dataLen;
    strncpy(data, this->rxFrame.data, this->rxFrame.dataLen);
    return true;
  }
  return false;
}

uint8_t SerLink::Socket::sendData(char* data, uint16_t dataLen, bool ack)
{
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
  this->txFrame.dataLen = dataLen;
  strncpy(this->txFrame.data, data, dataLen);

  uint8_t status = this->writer->sendFrame(this->txFrame);
  return status;
}

uint8_t SerLink::Socket::getSendStatus()
{
  uint8_t status = this->writer->getStatus();
  return status;
}
