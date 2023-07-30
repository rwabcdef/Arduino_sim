/*
 * Button.cpp
 *
 *  Created on: 15 Jul 2023
 *      Author: rw123
 */

#include "Button.hpp"
#include "swTimer.h"

#define STABLE 0
#define UNSTABLE 1

#define _TICK_CHECK_PERIOD_mS 1
#define _STABLE_THRESH_SAMPLES 3

Button::Button(uint8_t id, bool defaultPinState, Button_defs::GetPinState getPinState)
: id(id), defaultPinState(defaultPinState), getPinState(getPinState)
{
  this->currentState = STABLE;
  this->debugOn = true;
  this->previousPinState = this->defaultPinState;
  this->currentPinState = this->previousPinState;
  this->releaseEvent = false;
  this->pressEvent = false;
  this->unStableCount = 0;
  swTimer_tickReset(&this->checkTick);
}

//----------------------------------------------------------------
//----------------------------------------------------------------
// Start of state methods

uint8_t Button::stable()
{
  if(swTimer_tickCheckTimeout(&this->checkTick, _TICK_CHECK_PERIOD_mS))
  {
    this->currentPinState = this->getPinState();
    if(this->currentPinState != this->previousPinState)
    {
      this->stableCount = 0;
      this->previousPinState = this->currentPinState;
      this->debugWrite("b0 -> unst\n");

      return UNSTABLE;
    }
    else{}
  }
  this->previousPinState = this->currentPinState;
  return STABLE;
}
uint8_t Button::unstable()
{
  if(swTimer_tickCheckTimeout(&this->checkTick, _TICK_CHECK_PERIOD_mS))
  {
    this->currentPinState = this->getPinState();
    if(this->currentPinState == this->previousPinState)
    {
      this->stableCount++;
      if(this->stableCount >= _STABLE_THRESH_SAMPLES)
      {
        if(this->currentPinState == this->defaultPinState)
        {
          this->releaseEvent = true;
          this->debugWrite("rel\n");
          this->pressDuration = swTimer_tickElapsed(this->pressStartTick);
        }
        else
        {
          swTimer_tickReset(&this->pressStartTick);
          this->debugWrite("press\n");
          this->pressEvent = true;
        }

        this->previousPinState = this->currentPinState;
        return STABLE;
      }
    }
    else
    {
      this->stableCount = 0;
    }
  }

  this->previousPinState = this->currentPinState;
  return UNSTABLE;
}
// End of state methods
//----------------------------------------------------------------
//----------------------------------------------------------------
// Start of interface methods

void Button::run()
{
  switch(this->currentState)
  {
    case STABLE: { this->currentState = this->stable(); break;}
    case UNSTABLE: { this->currentState = this->unstable(); break;}
  }
}

bool Button::checkPress()
{
  bool b = this->pressEvent;
  this->pressEvent = false;
  return b;
}
bool Button::checkRelease(uint16_t& pressDuration)
{
  pressDuration = this->pressDuration;
  bool b = this->releaseEvent;
  this->releaseEvent = false;
  return b;
}
// End of interface methods
//----------------------------------------------------------------
//----------------------------------------------------------------
