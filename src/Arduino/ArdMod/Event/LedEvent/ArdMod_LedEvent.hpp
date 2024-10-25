/*
 * ArdMod_LedEvent.hpp
 *
 *  Created on: 10 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_LEDEVENT_HPP_
#define ARDMOD_LEDEVENT_HPP_

#include <stdint.h>
#include <stdbool.h>
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

    // LedEvent frame string length (includes null termination char)
    static const uint8_t FRAME_LEN = 1 + FLASH_DELAY_LEN + FLASH_ON_LEN + FLASH_OFF_LEN + 1;

    LedEvent(){};
    //uint8_t serialise(char* str){};
    bool deSerialise(char* str);
    void copy(Event* copyEvent);

    bool flashInitialOn;
    uint8_t flashDelayPeriods;
    uint8_t flashOnPeriods;
    uint8_t flashOffPeriods;
};

}

#endif /* ARDMOD_LEDEVENT_HPP_ */
