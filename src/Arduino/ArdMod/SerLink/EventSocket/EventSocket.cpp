/*
 * EventSocket.cpp
 *
 *  Created on: 26 Oct 2024
 *      Author: rw123
 */

#include "EventSocket.hpp"

namespace ArdMod{

EventSocket::EventSocket(SerLink::Socket* sock): socket(sock){}

EventReadSocket::EventReadSocket(SerLink::Socket* sock, char* rxData, Event* event):
  EventSocket(sock), EventProducer(event), rxData(rxData)
{
  this->eventOutputFlag = false;
}

bool EventReadSocket::hasEvent()
{
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
