/*
 * ArdMod_LedEvent.cpp
 *
 *  Created on: 10 Jun 2024
 *      Author: rw123
 */

#include "ArdMod_LedEvent.hpp"
#include <SerLink_Utils.hpp>

namespace ArdMod
{

bool LedEvent::deSerialise(char* str, Event* outEvent)
{
  uint8_t i = 0;
  LedEvent* ledEvent = (LedEvent*) outEvent;

  ledEvent->action = str[i++];

  if((ledEvent->action == LedEvent::ACTION_OFF) ||
      (ledEvent->action == LedEvent::ACTION_ON))
  {
    return true;
  }

  else if(ledEvent->action == LedEvent::ACTION_FLASH)
  {
    if(str[i++] == LedEvent::ACTION_ON)
    {
      ledEvent->flashInitialOn = true;
    }
    else
    {
      ledEvent->flashInitialOn = false;
    }

    ledEvent->flashDelayPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_DELAY_LEN);
    i += LedEvent::FLASH_DELAY_LEN;

    ledEvent->flashOnPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_ON_LEN);
    i += LedEvent::FLASH_ON_LEN;

    ledEvent->flashOffPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_OFF_LEN);
    i += LedEvent::FLASH_OFF_LEN;

    return true;
  }
  else
  {
    // invalid action code
    return false;
  }
}

void LedEvent::copy(Event* copyEvent)
{
  LedEvent* copy = (LedEvent*) copyEvent;

  copyEvent->action = this->action;

  if(this->action == LedEvent::ACTION_FLASH)
  {
    copy->flashDelayPeriods = this->flashDelayPeriods;
    copy->flashOnPeriods = this->flashOnPeriods;
    copy->flashOffPeriods = this->flashOffPeriods;
  }
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
LedEventProducer::LedEventProducer()
{
  this->eventReadyFlag = false;
}

LedEvent* LedEventProducer::getEventPtr()
{
  return &this->event;
}

bool LedEventProducer::hasEvent()
{
  bool ret = this->eventReadyFlag;
  this->eventReadyFlag = false;
  return ret;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
LedEventConsumer::LedEventConsumer()
{
  this->eventInputFlag = false;
}

void LedEventConsumer::setEvent(LedEvent* inputEvent)
{
  this->eventInputFlag = true;
  inputEvent->copy(&this->event);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
} // end of namespace
