/*
 * LedEvent_tests.cpp
 *
 *  Created on: 9 Dec 2024
 *      Author: rw123
 */

#include "LedEvent_tests.hpp"
#include "ArdMod_LedEvent.hpp"
#include "env.hpp"
#include "DebugPrint.hpp"
#include "Global.hpp"
#include "testSys.hpp"
#include "swTimer.h"
#include "Event.hpp"
#include "cute.h"
#include "System0.hpp"
#include "testComponents.hpp"
#include <string.h>
#include <stdio.h>

static void dPrint(ArdMod::LedEvent& ledEvent)
{
  printf("action: %c\n", ledEvent.action);
  printf("flashInitialOn: %d\n", ledEvent.flashInitialOn);
  printf("flashDelayPeriods: %d\n", ledEvent.flashDelayPeriods);
  printf("flashOnPeriods: %d\n", ledEvent.flashOnPeriods);
  printf("flashOffPeriods: %d\n", ledEvent.flashOffPeriods);
}
//-------------------------------------------------------------------
// Test of receive of a single LedEvent frame
void LedEventTests::rxOn()
{
  uint64_t current = 0;

  // ******    TO DO
}
//-------------------------------------------------------------------
void LedEventTests::deSerialise()
{
  ArdMod::LedEvent ledEvent;
  char eventStr[ArdMod::LedEvent::FRAME_LEN + 1];
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN);
  uint8_t i = 0;

  //--------------------
  eventStr[0] = ArdMod::LedEvent::ACTION_OFF;
  ledEvent.deSerialise(eventStr);
  dPrint(ledEvent);
  //--------------------
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN + 1);
  eventStr[0] = ArdMod::LedEvent::ACTION_ON;
  ledEvent.deSerialise(eventStr);
  dPrint(ledEvent);
  //--------------------
  memset(eventStr, 0, ArdMod::LedEvent::FRAME_LEN + 1);
  i = 0;
  eventStr[i] = ArdMod::LedEvent::ACTION_FLASH;
  i++;
  strcpy(&eventStr[i], "1072376\n");
  printf("%s\n", eventStr);
  ledEvent.deSerialise(eventStr);
  dPrint(ledEvent);
//--------------------
}


