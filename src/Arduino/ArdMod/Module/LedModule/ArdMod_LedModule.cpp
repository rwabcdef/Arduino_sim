/*
 * ArdMod_LedModule.cpp
 *
 *  Created on: 29 Jun 2024
 *      Author: rw123
 */

/*
 * LedModule.cpp
 *
 *  Created on: 29 Jun 2024
 *      Author: rw123
 */

#include "ArdMod_LedModule.hpp"

// State codes (do not use 0)
#define ON 1
#define OFF 2
#define DELAY 3

using namespace ArdMod;

LedModule::LedModule(uint8_t port, uint8_t pin, LedModule_defs::ledOn on, LedModule_defs::ledOff off)
: port(port), pin(pin), ledOn(ledOn), ledOff(ledOff)
{
  this->inputEvent = false;
  this->periodCount = 0;
}

void LedModule::setEvent(LedEvent& event)
{
  this->inputEvent = true;
  event.copy(&this->event);
}

void LedModule::run()
{
  switch(this->currentState)
  {
    case ON: { this->currentState = this->on(); break;}
    case OFF: { this->currentState = this->off(); break;}
    case DELAY: { this->currentState = this->delay(); break;}
  }
}

//-----------------------------------------------------------------
// Start of state methods

// On state
uint8_t LedModule::on()
{
  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }
  else{ /* No new state from input event - so do nothing */ }

  if(this->event.action == LedEvent::ACTION_FLASH)
  {
    // Handle flash mode - is it time to change state ?

    if(this->periodCount >= this->event.flashOnPeriods)
    {
      // Yes - go to off state
      this->ledOff(this->port, this->pin);
      this->periodCount = 0;
      return OFF;
    }
    else
    {
      // No - stay in this state
    }
    this->periodCount++;
  }

  return ON;
}

// Off state
uint8_t LedModule::off()
{
  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }
  else{ /* No new state from input event - so do nothing */ }

  if(this->event.action == LedEvent::ACTION_FLASH)
  {
    // Handle flash mode - is it time to change state ?

    if(this->periodCount >= this->event.flashOffPeriods)
    {
      // Yes - go to on state
      this->ledOn(this->port, this->pin);
      this->periodCount = 0;
      return ON;
    }
    else
    {
      // No - stay in this state
    }
    this->periodCount++;
  }

  return OFF;
}

// Delay state
uint8_t LedModule::delay()
{
  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }

  // Is it time to start standard flashing cycles ?
  if(this->periodCount >= this->event.flashDelayPeriods)
  {
    // Yes
    this->periodCount = 0;

    if(this->event.flashInitialOn == true)
    {
      // Initial state was on - so go to off
      this->ledOff(this->port, this->pin);
      return OFF;
    }
    else
    {
      // Initial state was off - so go to on
      this->ledOn(this->port, this->pin);
      return ON;
    }
  }
  else{ /* No - so do nothing */ }

  this->periodCount++;
  return DELAY;
}

// End of state methods
//-----------------------------------------------------------------
// All states common behaviour
uint8_t LedModule::common()
{
  this->inputEvent = false;  // clear flag

  if(this->event.action == LedEvent::ACTION_ON)
  {
    this->ledOn(this->port, this->pin);
    return ON;
  }
  else if(this->event.action == LedEvent::ACTION_OFF)
  {
    this->ledOff(this->port, this->pin);
    return OFF;
  }
  else if(this->event.action == LedEvent::ACTION_FLASH)
  {
    if(this->event.flashDelayPeriods > 0)
    {
      // Delay period before standard flashing cycles

      if(this->event.flashInitialOn == true)
      {
        this->ledOn(this->port, this->pin);
      }
      else
      {
        this->ledOff(this->port, this->pin);
      }

      this->periodCount = 0;
      return DELAY;
    }
    else
    {
      // NO delay - so go straight to the initial state

      if(this->event.flashInitialOn == true)
      {
        this->ledOn(this->port, this->pin);
        this->periodCount = 0;
        return ON;
      }
      else
      {
        this->ledOff(this->port, this->pin);
        this->periodCount = 0;
        return OFF;
      }
    }
  }
  else
  {
    // No input event - so do nothing
    return 0;
  }
}
