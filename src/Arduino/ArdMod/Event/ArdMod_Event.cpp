/*act:
 * Event.cpp
 *
 *  Created on: 7 Jun 2024
 *      Author: rw123
 */

#include <ArdMod_Event.hpp>


namespace ArdMod{

Event::Event()
{
  this->action = 0;
}

uint8_t Event::serialise(char* str)
{
  uint8_t i = 0;

  str[i++] = this->action;
  str[i] = 0;

  return i;
}

bool Event::deSerialise(char* str)
{
  uint8_t i = 0;

  this->action = str[i];

  return true;
}

void Event::copy(Event* copyEvent)
{
  copyEvent->action = this->action;
}



//----------------------------------------------------------------------
//----------------------------------------------------------------------
EventProducer::EventProducer(Event* event): outpuEvent(event)
{
  this->eventOutputFlag = false;
}

Event* EventProducer::getEventPtr()
{
  return this->outpuEvent;
}

bool EventProducer::hasEvent()
{
  bool ret = this->eventOutputFlag;
  this->eventOutputFlag = false;
  return ret;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
EventConsumer::EventConsumer(Event* event): inputEvent(event)
{
  this->eventInputFlag = false;
}

void EventConsumer::setEvent(Event* event)
{
  this->eventInputFlag = true;
  event->copy(this->inputEvent);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
}; // end of namespace
