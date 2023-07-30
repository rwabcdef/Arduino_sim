/*
 * SerLinkTxTests.cpp
 *
 *  Created on: 3 Jul 2023
 *      Author: rw123
 */

#include "SerLinkTxTests.hpp"
#include "env.hpp"
#include "DebugPrint.hpp"
#include "Global.hpp"
#include "testSys.hpp"
#include "swTimer.h"
#include "Event.hpp"
#include "cute.h"
#include "uart_wrapper.hpp"
#include "Frame.hpp"
#include "Reader_config.hpp"
#include "DebugUser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

//------------------------------------------------
//#define DEBUG_STR_LEN 256
static char debugStr[ENV_DEBUG_STR_LEN] = {0};

static char uartTxFrame[128] = {0};
static uint8_t uartTxCharIndex = 0;
static void Set_UDRIE0_CallBack();
static void Clr_UDRIE0_CallBack();
static void uartTxRun();

static void Set_UDRIE0_CallBack()
{
  debugPrint->writeLine("Set_UDRIE0_CallBack", DebugPrint_defs::UartTx);

  g_enable_USART_UDRE_ISR = true;

  std::list<uint32_t>* lst = new std::list<uint32_t>();
  lst->push_back((uint32_t) (simClk->getCurrent() + 200));

  InterruptSchedule* nextCharRun = new InterruptSchedule((InterruptRoutine)&uartTxRun, lst);
  interruptRunner->RegisterInterruptSchedule(nextCharRun);
}

static void uartTxRun()
{
  cli();
  RUN_ISR(UART_ISR_TX); // Run the Uart tx interrupt routine

  char c = UDR0;

  uartTxFrame[uartTxCharIndex++] = c;

  static char s[32];
  memset(s, 0, 32);

  if(c == '\n')
  {
    strcpy(s, "uartTx: endl");
    sprintf(debugStr, "uart Tx frame: %s", uartTxFrame);
    debugPrint->writeLine(debugStr, DebugPrint_defs::UartTx);
    memset(debugStr, 0, ENV_DEBUG_STR_LEN);
    uartTxCharIndex = 0;
  }
  else
  {
    sprintf(s, "uartTx: %c", c);
  }
  //debugPrint->writeLine(s, DebugPrint_defs::UartTx);

  if(g_enable_USART_UDRE_ISR)
  {
    // Initialise another ISR trigger
    std::list<uint32_t>* lst = new std::list<uint32_t>();
    lst->push_back((uint32_t) (simClk->getCurrent() + 500));

    InterruptSchedule* nextCharRun = new InterruptSchedule((InterruptRoutine)&uartTxRun, lst);
    interruptRunner->RegisterInterruptSchedule(nextCharRun);
  }
  sei();
}
static void Clr_UDRIE0_CallBack()
{
  debugPrint->writeLine("Clr_UDRIE0_CallBack", DebugPrint_defs::UartTx);
  g_enable_USART_UDRE_ISR = false;
}
//------------------------------------------------

void SerLinkTxTests::test1()
{
  char s[256] = {0};
  char uartRxDebugStr[128];
  volatile char rxBuffer[UART_BUFF_LEN];

  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);

  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;

  InitTimestamp();
  //initSys(5000);

  initRun(20000);
//  InterruptSchedule* pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, "TST08T0750076ABC07C\n", 5000, 530);
//  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
//  SerLink::Reader reader0(READER_CONFIG__READER0_ID);

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);
  //return 0;

  uart_init((char*) rxBuffer, (uint8_t) UART_BUFF_LEN);

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

      sprintf(uartTxFrame, "hello\n", 0);
      uart_write(uartTxFrame);
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
