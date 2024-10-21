/*
 * Event.hpp
 *
 *  Created on: 4 Jun 2024
 *      Author: rob
 */

#ifndef ARDMOD_EVENT_HPP_
#define ARDMOD_EVENT_HPP_

#include <stdint.h>


namespace ArdMod
{

class Event{

public:

  Event();
  virtual uint8_t serialise(char* str);
  static bool deSerialise(char* str, Event* outEvent);
  virtual void copy(Event* copyEvent);

  char action;

};

// Abstract base class for all EventProducer classes
class EventProducerBase{
public:
  // Used to poll EventProducer to see if it has an event to consume
  virtual bool hasEvent() = 0;
};

}



#endif /* ARDMOD_EVENT_HPP_ */
