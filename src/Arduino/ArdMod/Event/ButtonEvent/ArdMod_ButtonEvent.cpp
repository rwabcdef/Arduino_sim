/*
 * ArdMod_ButtonEvent.cpp
 *
 *  Created on: 9 Jun 2024
 *      Author: rw123
 */

#include "SerLink_Utils.hpp"
#include "ArdMod_ButtonEvent.hpp"

namespace ArdMod
{
ButtonEvent::ButtonEvent(){}

uint8_t ButtonEvent::serialise(char* str)
{
  uint8_t i = 0;

  str[i++] = this->action;

  if(this->action == ButtonEvent::ACTION_RELEASE)
  {
    SerLink::Utils::uint16ToStr(this->pressDuration_mS, &str[i]);
    i += 5;
  }
  else{ /* do nothing */ }

  str[i] = 0; // null terminate string

  return i;

  return i;
}

void ButtonEvent::deSerialise(char* str, Event* outEvent)
{
  uint8_t i = 0;

  outEvent->action = str[i];
}

}



