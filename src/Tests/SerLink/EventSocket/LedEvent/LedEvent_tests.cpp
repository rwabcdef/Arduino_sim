/*
 * LedEvent_tests.cpp
 *
 *  Created on: 9 Dec 2024
 *      Author: rw123
 */

#include "LedEvent_tests.hpp"
#include "ArdMod_LedEvent.hpp"
#include <string.h>
#include <stdio.h>

static void debugPrint(ArdMod::LedEvent& ledEvent)
{
  printf("action: %c\n", ledEvent.action);
  printf("flashInitialOn: %d\n", ledEvent.flashInitialOn);
  printf("flashDelayPeriods: %d\n", ledEvent.flashDelayPeriods);
  printf("flashOnPeriods: %d\n", ledEvent.flashOnPeriods);
  printf("flashOffPeriods: %d\n", ledEvent.flashOffPeriods);
}

void LedEventTests::deSerialise()
{
  ArdMod::LedEvent ledEvent;
  char eventStr[ArdMod::LedEvent::FRAME_LEN + 1];
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN);
  uint8_t i = 0;

  //--------------------
  eventStr[0] = ArdMod::LedEvent::ACTION_OFF;
  ledEvent.deSerialise(eventStr);
  debugPrint(ledEvent);
  //--------------------
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN + 1);
  eventStr[0] = ArdMod::LedEvent::ACTION_ON;
  ledEvent.deSerialise(eventStr);
  debugPrint(ledEvent);
  //--------------------
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN + 1);
  i = 0;
  eventStr[i] = ArdMod::LedEvent::ACTION_FLASH;
  i++;
  strcpy(&eventStr[i], "1072376\n");
  printf("%s\n", eventStr);
  ledEvent.deSerialise(eventStr);
  debugPrint(ledEvent);
//--------------------
}


