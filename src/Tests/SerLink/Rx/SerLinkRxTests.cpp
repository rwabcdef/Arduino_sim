/*
 * SerLinkRxTests.cpp
 *
 *  Created on: 30 Jun 2023
 *      Author: rw123
 */

#include <stdarg.h>
#include "SerLinkRxTests.hpp"
#include "env.hpp"
#include "Global.hpp"
#include "testSys.hpp"
#include "swTimer.h"
#include "Event.hpp"
#include "cute.h"
#include "uart_wrapper.hpp"
#include "Frame.hpp"
//#include "Reader_config.hpp"
#include "Reader.hpp"
#include "DebugUser.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

char rxBuffer[UART_BUFF_LEN];
char readerRxBuffer[UART_BUFF_LEN];
char readerAckBuffer[UART_BUFF_LEN];
static char writerTxBuffer[UART_BUFF_LEN];

static char writerTxFrameBuffer[UART_BUFF_LEN];
static char writerAckFrameBuffer[UART_BUFF_LEN];
static SerLink::Frame writerTxFrame(writerTxFrameBuffer);
static SerLink::Frame writerAckFrame(writerAckFrameBuffer);

static char readerRxFrameBuffer[UART_BUFF_LEN];
static char readerAckFrameBuffer[UART_BUFF_LEN];
static SerLink::Frame readerRxFrame(readerRxFrameBuffer);
static SerLink::Frame readerAckFrame(readerAckFrameBuffer);

static SerLink::Writer writer0(WRITER_CONFIG__WRITER0_ID, writerTxBuffer,
    UART_BUFF_LEN, &writerTxFrame, &writerAckFrame);

static SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer,
    UART_BUFF_LEN, &readerRxFrame, &readerAckFrame, &writer0);

void uartRxHandler(void* pData);

SerLinkRxTests::SerLinkRxTests(){}

//------------------------------------------------
//#define DEBUG_STR_LEN 256
static char debugStr[ENV_DEBUG_STR_LEN] = {0};

static char uartTxFrame[128] = {0};
static uint8_t uartTxCharIndex = 0;
static void Set_UDRIE0_CallBack();
static void Clr_UDRIE0_CallBack();
static void uartTxRun();

// Called when the line: sbi(UCSR0B, UDRIE0);
// is called from: uart_write(char* buffer)   in uart.c
// In the Arduino it sets the UDRIE0 bit in the UCSR0B register.
// On the PC, it sets g_enable_USART_UDRE_ISR = true;
// and registers the uartTxRun interrupt to be fired in 200 uS time.
static void Set_UDRIE0_CallBack()
{
  debugPrint->writeLine("Set_UDRIE0_CallBack", DebugPrint_defs::UartTx);

  g_enable_USART_UDRE_ISR = true;

  std::list<uint32_t>* lst = new std::list<uint32_t>();
  lst->push_back((uint32_t) (simClk->getCurrent() + 200));

  InterruptSchedule* nextCharRun = new InterruptSchedule((InterruptRoutine)&uartTxRun, lst);
  interruptRunner->RegisterInterruptSchedule(nextCharRun);
}

// Run by the interrupt thread.
// Calls the ISR: UART_ISR_TX
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
  debugPrint->writeLine(s, DebugPrint_defs::UartTx);

  // If the UART_ISR_TX interrupt is still enabled
  //(it is disabled in the UART_ISR_TX when the last character has been sent)
  // then trigger another interrupt to fire after the current character has been sent.
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

// Instant rx handler (i.e. payload goes into ack frame of reader)
static bool testReadHandler(SerLink::Frame &rxFrame, uint16_t* dataLen, char* data)
{
  uint8_t pRetCode;
  rxFrame.toString(debugStr, &pRetCode);
  debugPrint->writeLine(debugStr, DebugPrint_defs::UnitTest0);

  sprintf(data, "Echo: ");
  strncpy(&data[6], rxFrame.buffer, (long) rxFrame.dataLen);
  *dataLen = rxFrame.dataLen + 6;

  return true;
}
//------------------------------------------------

void SerLinkRxTests::test1()
{
  char s[256] = {0};
  Event uartRx(uartRxHandler);
  char uartRxDebugStr[128];

  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;

  InitTimestamp();
  //initSys(5000);

  initRun(20000);
  InterruptSchedule* pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, "TST08T0750076ABC07C\n", 5000, 530);
  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
  //SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer, UART_BUFF_LEN);

  debugPrint->writeLine("pA", DebugPrint_defs::Zero);
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
    }

    cli();
    if(uart_checkFrameRx())
    {
      uint8_t rxLen = uart_getRxLenAndReset();

      sprintf(s, "uart rx: %s", rxBuffer);

      debugPrint->writeLine(s, DebugPrint_defs::UartRx);
    }
    sei();


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
//------------------------------------------------
void SerLinkRxTests::instantHandler1()
{
  char s[256] = {0};
  Event uartRx(uartRxHandler);
  char uartRxDebugStr[128];

  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;



  //InitTimestamp();
  initDebug();
  //initSys(5000);
  //SerLink::Reader *reader0 = new SerLink::Reader(READER_CONFIG__READER0_ID);
  //SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer, UART_BUFF_LEN);

  reader0.registerInstantCallback("TST08", testReadHandler);

  initRun(50000);
  InterruptSchedule* pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, "TST08T0750076ABC07C\n", 5000, 530);
  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
//
  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);

  debugPrint->writeLine("pA", DebugPrint_defs::Zero);

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

    //reader0->run();
    reader0.run();

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

  //simClk->join(); // Wait for simClk thread to completely terminate
  //endSys();
  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    //ASSERT(current == 50000);
    //ASSERT_EQUAL(50000, current);
    ASSERT_GREATER_EQUAL(50000, current);
    ASSERT_LESS_EQUAL(current, 50010);
    ASSERT_EQUAL(50, swTimer_tickElapsed(startTick));
  }

}

//------------------------------------------------
void SerLinkRxTests::stdRx1()
{
  char s[256] = {0};
  Event uartRx(uartRxHandler);
  char uartRxDebugStr[128];
  char rxFrameBuffer[UART_BUFF_LEN];
  SerLink::Frame rxFrame(rxFrameBuffer);
  uint16_t startTick;
  swTimer_tickReset(&startTick);

  uint64_t current = 0;



  //InitTimestamp();
  initDebug();
  //initSys(5000);
  //SerLink::Reader *reader0 = new SerLink::Reader(READER_CONFIG__READER0_ID);
  //SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer, UART_BUFF_LEN);

  initRun(50000);
  InterruptSchedule* pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, "TST08T0750076ABC07C\n", 5000, 530);
  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
//
  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);

  debugPrint->writeLine("pA", DebugPrint_defs::Zero);

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

    //reader0->run();
    reader0.run();

    // poll reader0 to check if a frame has been received.
    if(reader0.getRxFrame(&rxFrame))
    {
      // Frame received

      uint8_t pRetCode;
      sprintf(debugStr, "Rx Frame: ", 0);
      rxFrame.toString(&debugStr[10], &pRetCode);
      debugPrint->writeLine(debugStr, DebugPrint_defs::UnitTest0);
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

  //simClk->join(); // Wait for simClk thread to completely terminate
  //endSys();
  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    //ASSERT(current == 50000);
    //ASSERT_EQUAL(50000, current);
    ASSERT_GREATER_EQUAL(50000, current);
    ASSERT_LESS_EQUAL(current, 50010);
    ASSERT_EQUAL(50, swTimer_tickElapsed(startTick));
  }

}
//---------------------------------------------------------------
void uartRxHandler(void* pData)
{
  uint64_t n = (uint64_t)pData;
  char c = (char)n;

  UDR0 = c;    // load ATmega328 Uart data register with the received char


  cli();
  RUN_ISR(UART_ISR_RX); // Run the Uart rx interrupt routine
  sei();

  static char s[32];
  memset(s, 0, 32);
  if(c == '\n')
  {
    sprintf(s, "uart: endl", 0);
  }
  else
  {
    sprintf(s, "uart: %c", c);
  }

  debugPrint->writeLine(s, DebugPrint_defs::UartRx);
  sei();
}
