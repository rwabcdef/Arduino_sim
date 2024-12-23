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

class EventProducer{
protected:
  bool eventOutputFlag;
  Event* outpuEvent;
public:
  EventProducer();
  void init(Event* event);
  virtual Event* getEventPtr();
  virtual bool hasEvent();
};

class EventConsumer{
protected:
  bool eventInputFlag;
  Event* inputEvent;
public:
  EventConsumer(Event* event);
  virtual void setEvent(Event* event);
};

}



#endif /* ARDMOD_EVENT_HPP_ */
