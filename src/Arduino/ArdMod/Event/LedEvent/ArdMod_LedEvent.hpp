/*
 * ArdMod_LedEvent.hpp
 *
 *  Created on: 10 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_LEDEVENT_HPP_
#define ARDMOD_LEDEVENT_HPP_

#include <stdint.h>
#include "ArdMod_Event.hpp"

// ArdMod_LedEvent.hpp

namespace ArdMod
{

class LedEvent : public Event
{
  public:
    static const char ACTION_ON = '1';
    static const char ACTION_OFF = '0';
    static const char ACTION_FLASH = 'F';

    static const char FLASH_DELAY_LEN = 3;
    static const char FLASH_ON_LEN = 3;
    static const char FLASH_OFF_LEN = 3;

    LedEvent(){};
    static void deSerialise(char* str, Event* outEvent);

    uint8_t flashDelayPeriods;
    uint8_t flashOnPeriods;
    uint8_t flashOffPeriods;
};

}

#endif /* ARDMOD_LEDEVENT_HPP_ */