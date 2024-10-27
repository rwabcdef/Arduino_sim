/*
 * EventSocket.hpp
 *
 *  Created on: 26 Oct 2024
 *      Author: rw123
 */

#ifndef EVENTSOCKET_HPP_
#define EVENTSOCKET_HPP_

#include "Transport.hpp"
#include "ArdMod_Event.hpp"

namespace ArdMod{

class EventSocket{
protected:
  SerLink::Socket* socket;

public:
  EventSocket(SerLink::Socket* sock);
};

class EventReadSocket : public EventSocket, public EventProducer{

protected:
    char* rxData;
    uint8_t rxDataLen;
public:
  EventReadSocket(SerLink::Socket* sock, char* rxData, Event* event);

  // over-ridden EventProducer method
  bool hasEvent();

};

class EventWriteSocket : public EventSocket, public EventConsumer{

};

} // end of namespace

#endif /* EVENTSOCKET_HPP_ */
