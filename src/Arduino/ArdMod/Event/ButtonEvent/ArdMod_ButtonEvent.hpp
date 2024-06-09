/*
 * ArdMod_ButtonEvent.hpp
 *
 *  Created on: 9 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_BUTTONEVENT_HPP_
#define ARDMOD_BUTTONEVENT_HPP_

#include "ArdMod_Event.hpp"

namespace ArdMod
{


class ButtonEvent : public Event
{
public:
  static const char ACTION_PRESS = 'P';
  static const char ACTION_RELEASE = 'R';

  ButtonEvent();
  uint8_t serialise(char* str);
  static void deSerialise(char* str, Event* outEvent);

  uint16_t pressDuration_mS;
};

}


#endif /* ARDMOD_BUTTONEVENT_HPP_ */
