/*
 * EventSocket.cpp
 *
 *  Created on: 26 Oct 2024
 *      Author: rw123
 */

#include "EventSocket.hpp"

namespace ArdMod{

EventSocket::EventSocket(){}

void EventSocket::init(SerLink::Socket* sock)
{
  this->socket = sock;
}

//-------------------------------------------------------------------
EventReadSocket::EventReadSocket(){}

void EventReadSocket::init(SerLink::Socket* sock, char* rxData, Event* event)
{
  EventProducer();
  this->socket = sock;
  this->rxData = rxData;
  this->outpuEvent = event;
}

bool EventReadSocket::hasEvent()
{
  //return false;
  if(this->socket->getRxData(this->rxData, &this->rxDataLen))
  {
    // Convert the data from the socket to an event.
    this->outpuEvent->deSerialise(this->rxData);

    return true;
  }
  else
  {
    return false;
  }
}

} // end of namespace
