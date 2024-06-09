/*
 * ArdMod_ButtonEvent.cpp
 *
 *  Created on: 9 Jun 2024
 *      Author: rw123
 */

#include "ArdMod_ButtonEvent.hpp"

namespace ArdMod
{
ButtonEvent::ButtonEvent(){}

uint8_t ButtonEvent::serialise(char* str)
{
  uint8_t i = 0;

  str[i++] = this->action;
  str[i] = 0;

  return i;
}

void ButtonEvent::deSerialise(char* str, Event* outEvent)
{
  uint8_t i = 0;

  outEvent->action = str[i];
}
}



