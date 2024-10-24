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

  // Convert this event to a string. Returns the string length.
  virtual uint8_t serialise(char* str);

  // Parses the input string and sets the variables of this event accordingly.
  virtual bool deSerialise(char* str);

  // Copies the variables of this event into the other event (copyEvent).
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
