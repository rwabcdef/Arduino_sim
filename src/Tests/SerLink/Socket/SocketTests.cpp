/*
 * SocketTests.cpp
 *
 *  Created on: 19 Dec 2023
 *      Author: rw123
 */


#include "SocketTests.hpp"
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
#include "Reader.hpp"
#include "Writer.hpp"
#include "DebugUser.hpp"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <thread>

//------------------------------------------------
//#define DEBUG_STR_LEN 256
static char debugStr[ENV_DEBUG_STR_LEN] = {0};

static char readerRxBuffer[UART_BUFF_LEN];
static char readerAckBuffer[UART_BUFF_LEN];
static char writerTxBuffer[UART_BUFF_LEN];

static char uartTxFrame[128] = {0};
static uint8_t uartTxCharIndex = 0;
static void Set_UDRIE0_CallBack();
static void Clr_UDRIE0_CallBack();
static void uartTxRun();

// The uart hardware bit: UDRIE0, is set in uart_write().
// This function is then called, which sets up a single interrupt,
// which corresponds to the uart data register becoming empty.
// The ISR then loads the uart data register with the first character.
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
    //debugPrint->writeLine(debugStr, DebugPrint_defs::UartTx);
    memset(debugStr, 0, ENV_DEBUG_STR_LEN);
    uartTxCharIndex = 0;
  }
  else
  {
    sprintf(s, "uartTx: %c", c);
  }
  debugPrint->writeLine(s, DebugPrint_defs::UartTx);

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
//---------------------------------------------------------------
static void uartRxHandler(void* pData)
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
    sprintf(s, "rx: endl", 0);
  }
  else
  {
    sprintf(s, "rx: %c", c);
  }

  debugPrint->writeLine(s, DebugPrint_defs::UartRx);
  sei();
}

//---------------------------------------------------------------
//---------------------------------------------------------------
// These objects MUST be global to avoid a stack overflow.
static SerLink::Writer writer0(WRITER_CONFIG__WRITER0_ID, writerTxBuffer, UART_BUFF_LEN);
static SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer, UART_BUFF_LEN, &writer0);
static InterruptSchedule* pUartRxInterrupt;

void SocketTests::TxThenAck1()
{
  char s[256] = {0};
  char uartRxDebugStr[128];
  volatile char rxBuffer[UART_BUFF_LEN];
  Event uartRx(uartRxHandler);
  uint16_t startTick;
  swTimer_tickReset(&startTick);
  char ackBuffer[32];
  uint64_t current = 0;
  bool txFrameSent = false;

  //SerLink::Frame* txFrame = new SerLink::Frame("TST04", SerLink::Frame::TYPE_UNIDIRECTION, 615, 6, "hello\n");

  // Frame that is initially sent
  static SerLink::Frame txFrame("TST04", SerLink::Frame::TYPE_TRANSMISSION, 615, 6, "hello1");

  // Ack frame that is received (i.e. from simulated remote device).
  static SerLink::Frame ackFrame("TST04", SerLink::Frame::TYPE_ACK, 615, SerLink::Frame::ACK_OK, "");

  //uint8_t retCode;
  //txFrame.toString(ackBuffer, &retCode);

  writer0.debugOn = true;
  reader0.init();  // Initialises uart hardware & buffers

  initDebug();
  initRun(50000);

  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);


  //ackFrame.toString(ackBuffer, &retCode);
  //pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, ackBuffer, 20000, 530);
  //pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, "TST04A615900\n", 20000, 530);
  //interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
//  SerLink::Reader reader0(READER_CONFIG__READER0_ID);

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);
  //return 0;

  //uart_init((char*) rxBuffer, (uint8_t) UART_BUFF_LEN);

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

      //sprintf(uartTxFrame, "hello\n", 0);
      //uart_write(uartTxFrame);
      writer0.sendFrame(txFrame);                // start send of tx frame
      txFrameSent = true;

      // Set up ack frame to be received
      uint8_t retCode;
      ackFrame.toString(ackBuffer, &retCode);
      pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, ackBuffer,
      simClk->getCurrent() + 20000, 530);
      interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
    }

    writer0.run();
    reader0.run();

    if(txFrameSent)
    {
      uint8_t status = writer0.getStatus();
      if(status == SerLink::Writer::STATUS_OK){
        char s[16];
        writer0.getStatusStr(s);
        sprintf(debugStr, "writer0 %s\n", s);
        //printf("%s\n", debugStr);
        debugPrint->writeLine(debugStr, DebugPrint_defs::UnitTest0);
        txFrameSent = false;
      }
    }

    if(30000 == current)
    {
      //printf("at 30000\n");
      debugPrint->writeLine("at 30000", DebugPrint_defs::Zero);
    }

  }while(!simClk->getDone());

  //simClk->join(); // Wait for simClk thread to completely terminate
  //endSys();
  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    ASSERT(current == 20000);
    ASSERT_EQUAL(20000, current);
    ASSERT_EQUAL(20, swTimer_tickElapsed(startTick));
  }
}
