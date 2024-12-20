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
#include "Transport.hpp"
#include "EventSocket.hpp"
#include <string.h>
#include <stdio.h>

// uart rx interrupt
char rxISR_FrameBuffer[UART_BUFF_LEN];
char rxISR_Buffer[UART_BUFF_LEN];

static char rxFrameFromReader0Buffer[UART_BUFF_LEN];
static SerLink::Frame rxFrameFromReader0(rxFrameFromReader0Buffer);  // received frame from reader0

static char txFrameBuffer[UART_BUFF_LEN];
static char ackFrameBuffer[UART_BUFF_LEN];
static char ackISR_Buffer[UART_BUFF_LEN];

//-----------------------
// ledSocket
static char ledSocketRxFrameBuffer[UART_BUFF_LEN];
static char ledSocketTxFrameBuffer[UART_BUFF_LEN];

static SerLink::Frame ledSocketRxFrame(ledSocketRxFrameBuffer);
static SerLink::Frame ledSocketTxFrame(ledSocketTxFrameBuffer);

static SerLink::Socket* ledSocket;
//-----------------------
static char ledEvSockData[UART_BUFF_LEN];
static ArdMod::LedEvent ledEv;
//static ArdMod::Event* pledEv = (ArdMod::Event*) &ledEv;
static ArdMod::EventReadSocket ledEvSock(ledSocket, ledEvSockData, &ledEv);
//-----------------------

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
  uint8_t retCode;
  const uint16_t sLen = 32;
  char s[sLen] = {0};

  // Frame that is initially received
  static SerLink::Frame rxISR_Frame("TST04", SerLink::Frame::TYPE_TRANSMISSION, 615, rxISR_FrameBuffer, 1, "1");

  writer0.debugOn = true;
  reader0.init();  // Initialises uart hardware & buffers

  initDebug();
  initRun(60000);

  setSet_UDRIE0_CallBack(&tc_Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&tc_Clr_UDRIE0_CallBack);

  // Setup initial receive frame.
  rxISR_Frame.toString((char*) rxISR_Buffer, &retCode);
  tc_pUartRxInterrupt = InterruptSchedule::buildStringEvent(&tc_uartRx, (const char*)rxISR_Buffer, 6000, 530);
  interruptRunner->RegisterInterruptSchedule(tc_pUartRxInterrupt);

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);

  sei();
  simClk->start();

  do
  {
    writer0.run();
    reader0.run();

    current = simClk->getCurrent();
    if(4000 == current)
    //if(swTimer_tickCheckTimeout(&mainSwTimerTick, 4))
    {
      debugPrint->writeLine("pA", DebugPrint_defs::Zero);
    }

    if(reader0.getRxFrame(&rxFrameFromReader0))
    {
      // Frame received from reader0.
      memset(s, 0, sLen);
      sprintf(s, "rx frame: ", nullptr);
      uint8_t retCode = 0;
      rxFrameFromReader0.toString(&s[10], &retCode);
      debugPrint->writeLine(s, DebugPrint_defs::UnitTest0);
    }

  }while(!simClk->getDone());

  endRun();
}

//-------------------------------------------------------------------
// Test of tx & ack functionality of writer0 with testComponents & system0 libs.
void LedEventTests::TxThenAck1()
{
  uint64_t current = 0;
  uint8_t retCode;
  const uint16_t sLen = 32;
  char s[sLen] = {0};
  bool txFrameSent = false;

  // Frame that is initially sent
  static SerLink::Frame txFrame("TST04", SerLink::Frame::TYPE_TRANSMISSION, 615, txFrameBuffer, 6, "hello1");

  // Ack frame that is received (i.e. from simulated remote device).
  static SerLink::Frame ackFrame("TST04", SerLink::Frame::TYPE_ACK, 615, ackFrameBuffer, SerLink::Frame::ACK_OK, "");

  writer0.debugOn = true;
  reader0.init();  // Initialises uart hardware & buffers

  initDebug();
  initRun(60000);

  setSet_UDRIE0_CallBack(&tc_Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&tc_Clr_UDRIE0_CallBack);

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);

  sei();
  simClk->start();

  do
  {
    writer0.run();
    reader0.run();

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


      writer0.sendFrame(&txFrame);                // start send of tx frame
      txFrameSent = true;

      // Set up ack frame to be received
      uint8_t retCode;
      ackFrame.toString(ackISR_Buffer, &retCode);

      tc_pUartRxInterrupt = InterruptSchedule::buildStringEvent(&tc_uartRx, ackISR_Buffer,
      simClk->getCurrent() + 20000, 530);

      interruptRunner->RegisterInterruptSchedule(tc_pUartRxInterrupt);
    }

    if(txFrameSent)
    {
      uint8_t status = writer0.getStatus();
      if(status == SerLink::Writer::STATUS_OK){
        char t[16];
        writer0.getStatusStr(t);
        sprintf(s, "writer0 %s\n", t);
        //printf("%s\n", debugStr);
        debugPrint->writeLine(s, DebugPrint_defs::UnitTest0);
        txFrameSent = false;
      }
    }

  }while(!simClk->getDone());

  endRun();
}
//-------------------------------------------------------------------
// Test of tx & ack functionality of writer0 with testComponents & system0 libs.
void LedEventTests::SockTxThenAck1()
{
  uint64_t current = 0;
  uint8_t retCode;
  const uint16_t sLen = 32;
  char s[sLen] = {0};
  bool txFrameSent = false;

  // Frame that is initially sent
  static SerLink::Frame txFrame("TST04", SerLink::Frame::TYPE_TRANSMISSION, 615, txFrameBuffer, 6, "hello1");

  // Ack frame that is received (i.e. from simulated remote device).
  static SerLink::Frame ackFrame("TST04", SerLink::Frame::TYPE_ACK, 615, ackFrameBuffer, SerLink::Frame::ACK_OK, "");

  writer0.debugOn = true;
  reader0.init();  // Initialises uart hardware & buffers

  initDebug();
  initRun(60000);

  setSet_UDRIE0_CallBack(&tc_Set_UDRIE0_CallBack);
  setClr_UDRIE0_CallBack(&tc_Clr_UDRIE0_CallBack);

  ledSocket = transport0.acquireSocket("TST04", &ledSocketRxFrame, &ledSocketTxFrame, 615);
  if(nullptr == ledSocket)
  {
    debugPrint->writeLine("socket NOT acquired", DebugPrint_defs::Zero);
    return;
  }

  debugPrint->writeLine("pStart", DebugPrint_defs::Zero);

  sei();
  simClk->start();

  do
  {
//    writer0.run();
//    reader0.run();
    transport0.run();

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

      if(SerLink::Socket::TX_STATUS_IDLE == ledSocket->getAndClearSendStatus())
      {
        ledSocket->sendData("hello Dave", 10, true);   // use socket to send frame
        txFrameSent = true;

        // Set up ack frame to be received
        uint8_t retCode;
        ackFrame.toString(ackISR_Buffer, &retCode);

        tc_pUartRxInterrupt = InterruptSchedule::buildStringEvent(&tc_uartRx, ackISR_Buffer,
        simClk->getCurrent() + 20000, 530);

        interruptRunner->RegisterInterruptSchedule(tc_pUartRxInterrupt);
      }


      //writer0.sendFrame(&txFrame);                // start send of tx frame
      //txFrameSent = true;


    }

    if(txFrameSent)
    {
      uint8_t status = writer0.getStatus();
      if(status == SerLink::Writer::STATUS_OK){
        char t[16];
        writer0.getStatusStr(t);
        sprintf(s, "writer0 %s\n", t);
        //printf("%s\n", debugStr);
        debugPrint->writeLine(s, DebugPrint_defs::UnitTest0);
        txFrameSent = false;
      }
    }

  }while(!simClk->getDone());

  endRun();
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


