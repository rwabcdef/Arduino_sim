/*
 * TransportTests.cpp
 *
 *  Created on: 16 Mar 2024
 *      Author: rob
 */


/*
 * TransportTests.cpp
 *
 *  Created on: 15 Mar 2024
 *      Author: rob
 */

#include "TransportTests.hpp"
#include "Transport.hpp"
#include <ATmega328.hpp>
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

static char rxFrameBuffer[UART_BUFF_LEN];

static char uartTxFrame[128] = {0};
static uint8_t uartTxCharIndex = 0;

//-----------------------------------------------------------------------
// These objects MUST be global to avoid a stack overflow.

//-----------------------
// reader0 & writer0
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

static InterruptSchedule* pUartRxInterrupt;
//static SerLink::Frame rxFrameFromReader0;  // received frame from reader0

//-----------------------
// socket0
static char socket0RxFrameBuffer[UART_BUFF_LEN];
static char socket0TxFrameBuffer[UART_BUFF_LEN];

static SerLink::Frame socket0RxFrame(socket0RxFrameBuffer);
static SerLink::Frame socket0TxFrame(socket0TxFrameBuffer);
//-----------------------
// transport0

static char transport0RxFrameBuffer[UART_BUFF_LEN];
static char transport0TxFrameBuffer[UART_BUFF_LEN];

static SerLink::Frame transport0RxFrame(transport0RxFrameBuffer);
static SerLink::Frame transport0TxFrame(transport0TxFrameBuffer);

static SerLink::Transport transport0(&reader0, &writer0, &transport0RxFrame, &transport0TxFrame);
//-----------------------

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

// Transmit a frame and then receive the ack
void TransportTests::TxThenAck1()
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
  SerLink::Socket* socket0;
  char ackFrameBuffer[UART_BUFF_LEN];

  // Ack frame that is received (i.e. from simulated remote device).
  static SerLink::Frame ackFrame("TST04", SerLink::Frame::TYPE_ACK, 615, ackFrameBuffer, SerLink::Frame::ACK_OK, "");

  writer0.debugOn = true;
  reader0.init();  // Initialises uart hardware & buffers

  initDebug();
  initRun(50000);

  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);

  socket0 = transport0.acquireSocket("TST04", &socket0RxFrame, &socket0TxFrame, 615);
  if(nullptr == socket0)
  {
	  debugPrint->writeLine("socket NOT acquired", DebugPrint_defs::Zero);
	  return;
  }

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);

  sei();
  simClk->start();

  do
  {
    current = simClk->getCurrent();
    if(4000 == current)
    //if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
    {
      debugPrint->writeLine("pA", DebugPrint_defs::Zero);

      if(SerLink::Socket::TX_STATUS_IDLE == socket0->getAndClearSendStatus())
      {
    	  socket0->sendData("hello Dave", 10, true);   // use socket to send frame
		  txFrameSent = true;

		  // Set up ack frame to be received
		  uint8_t retCode;
		  ackFrame.toString(ackBuffer, &retCode);
		  pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, ackBuffer,
		  simClk->getCurrent() + 20000, 530);
		  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);
      }
    }

      transport0.run();

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

  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    ASSERT(current == 20000);
    ASSERT_EQUAL(20000, current);
    ASSERT_EQUAL(20, swTimer_tickElapsed(startTick));
  }
}

//---------------------------------------------------------------
// Rx of frame then std ack
void TransportTests::RxThenStdAck()
{
  const uint16_t sLen = 64;
  char s[sLen] = {0};
  //char uartRxDebugStr[128];
  volatile char rxBuffer[UART_BUFF_LEN];
  Event uartRx(uartRxHandler);
  uint16_t startTick;
  swTimer_tickReset(&startTick);
  //char ackBuffer[32];
  uint64_t current = 0;
  //bool txFrameSent = false;
  SerLink::Socket* socket0;
  uint8_t retCode;
  const uint8_t SOCK_DATA_MAX_LEN = 32;
  uint8_t sockRxDataLen;
  char sockRxData[SOCK_DATA_MAX_LEN];


  // Frame that is initially received
  static SerLink::Frame rxFrame("TST05", SerLink::Frame::TYPE_TRANSMISSION, 615, rxFrameBuffer, 5, "hello");

  writer0.debugOn = false; //true;
  reader0.init();  // Initialises uart hardware & buffers
  reader0.debugOn = true;

  initDebug();
  initRun(70000);

  // Setup initial receive frame.
  rxFrame.toString((char*) rxBuffer, &retCode);
  pUartRxInterrupt = InterruptSchedule::buildStringEvent(&uartRx, (const char*)rxBuffer, 6000, 530);
  interruptRunner->RegisterInterruptSchedule(pUartRxInterrupt);

  setSet_UDRIE0_CallBack(&Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&Clr_UDRIE0_CallBack);

  // Acquire a socket from the transport layer
  socket0 = transport0.acquireSocket("TST05", &socket0RxFrame, &socket0TxFrame, 245);
  if(nullptr == socket0)
  {
	  debugPrint->writeLine("socket NOT acquired", DebugPrint_defs::Zero);
	  return;
  }

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);

  sei();
  simClk->start();

  do
  {
    transport0.run();

    current = simClk->getCurrent();
    if(4000 == current)
    //if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
    {
      debugPrint->writeLine("pA", DebugPrint_defs::Zero);
    }



    if(socket0->getRxData(sockRxData, &sockRxDataLen))
    {
	  // data received from socket0.
      memset(s, 0, sLen);
      sprintf(s, "socket0 rx: %s", sockRxData);
      debugPrint->writeLine(s, DebugPrint_defs::UnitTest0);
    }

    if(30000 == current)
    {
      //printf("at 30000\n");
      debugPrint->writeLine("at 30000", DebugPrint_defs::Zero);
    }

  }while(!simClk->getDone());

  endRun();


  //ASSERT_EQUALM("x should be 5", 5, current);

  if(TESTSYS_mode == TESTSYS_TEST_MODE_UNIT)
  {
    ASSERT(current == 20000);
    ASSERT_EQUAL(20000, current);
    ASSERT_EQUAL(20, swTimer_tickElapsed(startTick));
  }
}
