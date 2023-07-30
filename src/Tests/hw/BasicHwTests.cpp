/*
 * BasicHwTests.cpp
 *
 *  Created on: 26 Jun 2023
 *      Author: rw123
 */

#include "BasicHwTests.hpp"
#include "env.hpp"
#include "Global.hpp"
#include "testSys.hpp"
#include "swTimer.h"
#include "Event.hpp"
#include "cute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

BasicHwTests::BasicHwTests(){}

void BasicHwTests::operator()(){}

void event0Handler(void* pData);

void BasicHwTests::test1()
{
  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;

	InitTimestamp();
  //initSys(5000);
	initRun(5000);
	debugPrint->writeLine("pA", DebugPrint_defs::Zero);
	//return 0;
	sei();

  simClk->start();

	do
	{
		current = simClk->getCurrent();
		if(4000 == current)
		//if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
		{
			debugPrint->writeLine("pA", DebugPrint_defs::Zero);
			//printf("pB");

			//printf("mainThreadId: 0x%x\n", std::hash<std::thread::id>{}(mainThreadId));
			//printf("intThreadId:  0x%x\n", std::hash<std::thread::id>{}(intThreadId));
			//x = true;
		}


	}while(!simClk->getDone());

	//simClk->join(); // Wait for simClk thread to completely terminate
	endRun();


	//std::this_thread::sleep_for (std::chrono::seconds(1));

	//ASSERT_EQUALM("x should be 5", 5, current);

	if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
	{
	  ASSERT(current == 5000);
	  ASSERT_EQUAL(5000, current);
	  ASSERT_EQUAL(5, swTimer_tickElapsed(startTick));
	}


	//endRun();
	//delete simClk;
	//interruptRunner->clear();
	//delete interruptRunner;
	//endSys();
}

void BasicHwTests::test2()
{
  Event event0(event0Handler);
  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;

  InitTimestamp();
  //initSys(5000);

  initRun(5000);

//  std::function<void(uint64_t)> tickBind = [](uint64_t tick) { interruptRunner->tickRun(tick); };
//  simClk = new SimClk(5000, tickBind, 7854983);



  InterruptSchedule getIntThreadIdInterrupt2(&getIntThreadId_ISR, new std::list<uint32_t>{1});
  //interruptRunner->RegisterInterruptSchedule(&getIntThreadIdInterrupt2);

  InterruptSchedule* pEvent0Interrupt = InterruptSchedule::buildStringEvent(&event0, "hello\n", 3300, 10);
  interruptRunner->RegisterInterruptSchedule(pEvent0Interrupt);




  debugPrint->writeLine("pA", DebugPrint_defs::Zero);
  //return 0;

  //sei();
  simClk->start();

  do
  {
    current = simClk->getCurrent();
    if(4000 == current)
    //if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
    {
      debugPrint->writeLine("pA", DebugPrint_defs::Zero);
      //printf("pB");

      //printf("mainThreadId: 0x%x\n", std::hash<std::thread::id>{}(mainThreadId));
      //printf("intThreadId:  0x%x\n", std::hash<std::thread::id>{}(intThreadId));
      //x = true;
    }


  }while(!simClk->getDone());

  //simClk->join(); // Wait for simClk thread to completely terminate
  //endSys();
  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    ASSERT(current == 5000);
    ASSERT_EQUAL(5000, current);
    ASSERT_EQUAL(5, swTimer_tickElapsed(startTick));
  }

}

void event0Handler(void* pData)
{
  static char s[32];

  uint64_t n = (uint64_t)pData;
  char c = (char)n;
  memset(s, 0, 32);
  if(c == '\n')
  {
    strcpy(s, "event 0: endl");
  }
  else
  {
    sprintf(s, "event 0: %c", c);
  }

  debugPrint->writeLine(s, DebugPrint_defs::Event0);
}
