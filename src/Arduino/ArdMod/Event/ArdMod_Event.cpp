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

}

uint8_t Event::serialise(char* str)
{
  uint8_t i = 0;

  str[i++] = this->action;
  str[i] = 0;

  return i;
}

bool Event::deSerialise(char* str, Event* outEvent)
{
  uint8_t i = 0;

  outEvent->action = str[i];
}

void Event::copy(Event* copyEvent)
{
  copyEvent->action = this->action;
}

};
