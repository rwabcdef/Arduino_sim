/*
 * ButtonTests.cpp
 *
 *  Created on: 15 Jul 2023
 *      Author: rw123
 */

#include "ButtonTests.hpp"
#include "Button.hpp"
#include "env.hpp"
#include "Global.hpp"
#include "ATmega328.hpp"
#include "testSys.hpp"
#include "swTimer.h"
#include "Event.hpp"
#include "cute.h"
#include "EmbUtils.hpp"
//#include "uart_wrapper.hpp"
//#include "Frame.hpp"
//#include "Reader_config.hpp"
#include "DebugUser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

ButtonTests::ButtonTests(){}

char debugStr[64];
//------------------------------------------------
static volatile bool button0State = true;
static void button0InputInterruptHandler()
{
  static char s[32];

  //button0State = (!button0State)
//  if(CHECK_BIT(PIND, 2)){
//      CLR_BIT(PIND, 2);
//    }else{
//      SET_BIT(PIND, 2);
  TOGGLE_BIT(PIND, 2);

  //char pinState = button0State == true ? '1' : '0';
  char pinState = CHECK_BIT(PIND, 2) ? '1' : '0';
  memset(s, 0, 32);
  sprintf(s, "button0 pin: %c", pinState);
  debugPrint->writeLine(s, DebugPrint_defs::Butt0);
}

bool getButton0State()
{
  return CHECK_BIT(PIND, 2) ? true : false;
  //return button0State;
}
//------------------------------------------------

void ButtonTests::test1()
{
  uint16_t startTick;
  uint16_t pressDuration;
  swTimer_tickReset(&startTick);
  SET_BIT(PIND, 2);

  g_simClk_uS = 0;

  initDebug();
  initRun(500000);

  Button button0(0, true, getButton0State);

  InterruptSchedule button0InputInterrupt(&button0InputInterruptHandler,
      new std::list<uint32_t>{2010, 3237, 4568, 5978, 6478, 7580, 8894, 9596, 10308,
    401259, 402678, 403045, 404185, 405356
  });
  interruptRunner->RegisterInterruptSchedule(&button0InputInterrupt);



  debugPrint->writeLine("pA", DebugPrint_defs::Zero);


  sei();
  simClk->start();

  do
  {
    g_simClk_uS = simClk->getCurrent();
    if(4000 == g_simClk_uS)
    //if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
    {
      debugPrint->writeLine("pB", DebugPrint_defs::Zero);
      //printf("pB");

      //printf("mainThreadId: 0x%x\n", std::hash<std::thread::id>{}(mainThreadId));
      //printf("intThreadId:  0x%x\n", std::hash<std::thread::id>{}(intThreadId));
      //x = true;
    }
    button0.run();
    if(button0.checkPress())
    {
      debugPrint->writeLine("button0 press\n\0", DebugPrint_defs::Butt0);
    }
    if(button0.checkRelease(pressDuration))
    {
      memset(debugStr, 0, 64);
      sprintf(debugStr, "button0 release: %d\n\0", pressDuration);
      debugPrint->writeLine(debugStr, DebugPrint_defs::Butt0);
    }

//    cli();
//    if(uart_checkFrameRx())
//    {
//      uint8_t rxLen = uart_getRxLenAndReset();
//
//      sprintf(s, "uart rx: %s", rxBuffer);
//
//      debugPrint->writeLine(s, DebugPrint_defs::UartRx);
//    }
//    sei();


  }while(!simClk->getDone());

  endRun();
}
