/*
 * Hutton.hpp
 *
 *  Created on: 15 Jul 2023
 *      Author: rw123
 */

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "StateMachine.hpp"
#include "DebugUser.hpp"
#include<stdint.h>

namespace Button_defs{

typedef bool (*GetPinState)();
}

class Button : public StateMachine, public DebugUser
{
public:
  Button(uint8_t id, bool defaultPinState, Button_defs::GetPinState getPinState);
  void run();
  bool checkPress();
  bool checkRelease(uint16_t& pressDuration);
private:
  const uint8_t id;
  const bool defaultPinState;
  bool currentPinState;
  bool previousPinState;
  bool pressEvent;
  bool releaseEvent;
  Button_defs::GetPinState getPinState;
  uint8_t stableCount;
  uint8_t unStableCount;
  uint16_t checkTick;
  uint16_t pressStartTick;
  uint16_t pressDuration;
  uint8_t stable();
  uint8_t unstable();
};


#endif /* BUTTON_HPP_ */
