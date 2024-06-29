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

void LedEvent::deSerialise(char* str, Event* outEvent)
{
  uint8_t i = 0;
  LedEvent* ledEvent = (LedEvent*) outEvent;

  ledEvent->action = str[i++];

  if(ledEvent->action == LedEvent::ACTION_FLASH)
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
  }
  else
  {

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

}
