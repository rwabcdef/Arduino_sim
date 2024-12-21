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

LedEvent::LedEvent()
{
  this->action = ACTION_OFF;
  this->flashInitialOn = false;
  this->flashDelayPeriods = 0;
  this->flashOnPeriods = 0;
  this->flashOffPeriods = 0;
}

uint8_t LedEvent::serialise(char* str)
{
  uint8_t i = 0;
  str[i++] = this->action;

  if(this->action == LedEvent::ACTION_FLASH)
  {
    if(this->flashInitialOn)
    {
      str[i++] = LedEvent::ACTION_ON;
    }
    else
    {
      str[i++] = LedEvent::ACTION_OFF;
    }

    SerLink::Utils::uint16ToStr(this->flashDelayPeriods, &str[i], LedEvent::FLASH_DELAY_LEN);
    i += 2;
    SerLink::Utils::uint16ToStr(this->flashOnPeriods, &str[i], LedEvent::FLASH_DELAY_LEN);
    i += 2;
    SerLink::Utils::uint16ToStr(this->flashOffPeriods, &str[i], LedEvent::FLASH_DELAY_LEN);
    i += 2;
  }
  str[i++] = '\n';  // endl terminate for transmission test
  str[i] = 0;   // null terminate (for debug print)
}

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
