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

LedModule::LedModule(uint8_t port, uint8_t pin, Event* event)
: port(port), pin(pin), EventConsumer(event)
{
  //this->inputEvent = false;
  this->currentState = OFF;
  this->periodCount = 0;
}

/*
void LedModule::setEvent(LedEvent& event)
{
  this->inputEvent = true;
  event.copy(&this->event);
}*/

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
  LedEvent* event = (LedEvent*) this->inputEvent;

  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }
  else{ /* No new state from input event - so do nothing */ }

  if(event->action == LedEvent::ACTION_FLASH)
  {
    // Handle flash mode - is it time to change state ?

    if(this->periodCount >= event->flashOnPeriods)
    {
      // Yes - go to off state
      gpio_setPinLow(this->port, this->pin);
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
  LedEvent* event = (LedEvent*) this->inputEvent;

  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }
  else{ /* No new state from input event - so do nothing */ }

  if(event->action == LedEvent::ACTION_FLASH)
  {
    // Handle flash mode - is it time to change state ?

    if(this->periodCount >= event->flashOffPeriods)
    {
      // Yes - go to on state
      gpio_setPinHigh(this->port, this->pin);
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
  LedEvent* event = (LedEvent*) this->inputEvent;

  uint8_t newState = this->common();
  if(newState > 0)
  {
    return newState;
  }

  // Is it time to start standard flashing cycles ?
  if(this->periodCount >= event->flashDelayPeriods)
  {
    // Yes
    this->periodCount = 0;

    if(event->flashInitialOn == true)
    {
      // Initial state was on - so go to off
      gpio_setPinLow(this->port, this->pin);
      return OFF;
    }
    else
    {
      // Initial state was off - so go to on
      gpio_setPinHigh(this->port, this->pin);
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
  LedEvent* event = (LedEvent*) this->inputEvent;

  if(false == this->eventInputFlag)
  {
    // No new input event
    this->eventInputFlag = false;  // clear flag
    return 0;
  }

  this->eventInputFlag = false;  // clear flag

  if(event->action == LedEvent::ACTION_ON)
  {
    gpio_setPinHigh(this->port, this->pin);
    return ON;
  }
  else if(event->action == LedEvent::ACTION_OFF)
  {
    gpio_setPinLow(this->port, this->pin);
    return OFF;
  }
  else if(event->action == LedEvent::ACTION_FLASH)
  {
    if(event->flashDelayPeriods > 0)
    {
      // Delay period before standard flashing cycles

      if(event->flashInitialOn == true)
      {
        gpio_setPinHigh(this->port, this->pin);
      }
      else
      {
        gpio_setPinLow(this->port, this->pin);
      }

      this->periodCount = 0;
      return DELAY;
    }
    else
    {
      // NO delay - so go straight to the initial state

      if(event->flashInitialOn == true)
      {
        gpio_setPinHigh(this->port, this->pin);
        this->periodCount = 0;
        return ON;
      }
      else
      {
        gpio_setPinLow(this->port, this->pin);
        this->periodCount = 0;
        return OFF;
      }
    }
  }
  else
  {
    // Invalid event action - so do nothing
    return 0;
  }
}
