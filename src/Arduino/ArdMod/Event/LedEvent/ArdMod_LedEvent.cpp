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

bool LedEvent::deSerialise(char* str)
{
  uint8_t i = 0;
  //LedEvent* ledEvent = (LedEvent*) outEvent;

  this->action = str[i++];

  if((this->action == LedEvent::ACTION_OFF) ||
      (this->action == LedEvent::ACTION_ON))
  {
    return true;
  }

  else if(this->action == LedEvent::ACTION_FLASH)
  {
    if(str[i++] == LedEvent::ACTION_ON)
    {
      this->flashInitialOn = true;
    }
    else
    {
      this->flashInitialOn = false;
    }

    this->flashDelayPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_DELAY_LEN);
    i += LedEvent::FLASH_DELAY_LEN;

    this->flashOnPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_ON_LEN);
    i += LedEvent::FLASH_ON_LEN;

    this->flashOffPeriods = SerLink::Utils::strToUint8(&str[i], LedEvent::FLASH_OFF_LEN);
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

} // end of namespace
