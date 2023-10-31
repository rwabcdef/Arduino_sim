/*
 * Reader.cpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */

#include "Reader.hpp"
#include "ATmega328.hpp"
#include "swTimer.h"
#include <string.h>
#include <stdio.h>

#define IDLE 0
#define ACKDELAY 1
#define TXACKWAIT 2

using namespace SerLink;


Reader::Reader(uint8_t id)
{
	this->id = id;
	this->rxFlag = false;
	//this->rxBuffer = rxBuffer;
	//this->ackBuffer = ackBuffer;
	this->bufferLen = UART_BUFF_LEN;
	//this->debugPrinter = debugPrint;
	this->currentState = IDLE;
	this->debugLevel = DebugPrint_defs::UartRx;
	this->debugOn = true;

	for(uint8_t i=0; i<READER_CONFIG__MAX_NUM_INSTANT_HANDLERS; i++)
	{
	  this->handlerRegistrations[i].handler = nullptr;
	  memset(this->handlerRegistrations[i].protocol, 0, Frame::LEN_PROTOCOL);
	}
	this->numInstantHandlers = 0;

#ifdef READER_CONFIG__READER0

  if(this->id == READER_CONFIG__READER0_ID)
  {
    // Initialise uart hardware & driver layer
    uart_init((char*) this->rxBuffer, this->bufferLen);
  }

#endif

}
void Reader::run()
{
	switch(this->currentState)
	{
		case IDLE: { this->currentState = this->idle(); break;}
		case ACKDELAY: { this->currentState = this->ackDelay(); break;}
		case TXACKWAIT: { this->currentState = this->txAckWait(); break;}
	}
}

char* Reader::getCurrentStateName()
{

}

bool Reader::registerInstantCallback(char* protocol, readHandler handler)
{
  if(this->numInstantHandlers < (READER_CONFIG__MAX_NUM_INSTANT_HANDLERS - 1))
  {
    strncpy(this->handlerRegistrations[this->numInstantHandlers].protocol, protocol, Frame::LEN_PROTOCOL);
    this->handlerRegistrations[this->numInstantHandlers].handler = handler;
    this->numInstantHandlers++;
    return true;
  }
  else
  {
    // No more registrations are available
    return false;
  }
}

readHandler Reader::getInstantHandler(char* protocol)
{
  for(uint8_t i=0; i<READER_CONFIG__MAX_NUM_INSTANT_HANDLERS; i++)
  {
    if(0 == strncmp(this->handlerRegistrations[i].protocol, protocol, Frame::LEN_PROTOCOL))
    {
      return this->handlerRegistrations[i].handler;
    }
  }
  return nullptr;
}

bool Reader::getRxFrame(Frame& rxFrame)
{
  if(this->rxFlag)
  {
    this->rxFlag = false;

    this->rxFrame.copy(&rxFrame);

    return true;
  }
  else
  {
    return false;
  }
}
//-----------------------------------------------------------------
// Start of state methods

uint8_t Reader::idle()
{

	cli();
	if(this->checkUartFrameRx())
	{
		uint8_t rxLen = this->getUartRxLenAndReset();

		// Read frame from uart.
		Frame::fromString((char*) this->rxBuffer, &this->rxFrame);

		this->rxFlag = true;

		if(this->rxFrame.type == Frame::TYPE_TRANSMISSION)
		{
			this->rxFrame.copy(&this->ackFrame);
			this->ackFrame.type = Frame::TYPE_ACK;
			this->ackFrame.dataLen = Frame::ACK_OK;
			memset(&this->ackFrame.data, 0, Frame::MAX_DATALEN);

			readHandler instantHandler = this->getInstantHandler(this->rxFrame.protocol);
			if(instantHandler == nullptr)
			{
			  // No instant (i.e. piggyback) handler has been found - so do nothing.
			}
			else
			{
			  // An instant (i.e. piggyback) handler has been found for this protocol,
			  // so call it now.
			  // The instant handler callback sets the ackFrame's data & dataLen.
			  instantHandler(this->rxFrame, &this->ackFrame.dataLen, this->ackFrame.data);
			}

			swTimer_tickReset(&this->startTick);

//			for(int i=0; i<1000000; i++){
//			  volatile int n = i * 7;
//			}

			//sprintf(this->s, "uart rx: %s", this->rxBuffer);
      //this->debugWrite(this->s);
			sei();
			//return IDLE;
			return ACKDELAY;
		}
		else if(this->rxFrame.type == Frame::TYPE_UNIDIRECTION)
		{

		}
		else
		{

		}
//
//		swTimer_tickReset(&this->startTick);
//		sei();
//		return ACKDELAY;
	}
	sei();

	return IDLE;
}
uint8_t Reader::ackDelay()
{
	if(swTimer_tickCheckTimeout(&this->startTick, 10))
	{
		//sprintf(this->s, "Reader::ackDelay() done\n\0", 0);

		//this->debugWrite(this->s);


		memset((char*)this->ackBuffer, 0, this->bufferLen);
		uint8_t ret;
		//this->ackFrame.toString((char*)this->ackBuffer, &ret);
		this->ackFrame.toString((char*)this->ackBuffer, &ret);
		//char ackBuffer[64] = {0};
		//sprintf(ackBuffer, "TestAck\n", 0);

		sprintf(this->s, "ack frame: %s\n\0", this->ackBuffer);
		this->debugWrite(this->s);

		this->uartWrite((char*)this->ackBuffer);
		//this->write((char*)"hello\n\0");

		return TXACKWAIT;
	}
	return ACKDELAY;
}
uint8_t Reader::txAckWait()
{
	if(!this->getUartTxBusy())
	{
		sprintf(this->s, "Reader::txAckWait() done\n\0", 0);
		this->debugWrite(this->s);
		return IDLE;
	}
	return TXACKWAIT;
}
// end of state methods
//-------------------------------------------------------------
bool Reader::checkUartFrameRx()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_checkFrameRx();
	}

#endif
}
uint8_t Reader::getUartRxLenAndReset()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_getRxLenAndReset();
	}

#endif
}
uint8_t Reader::uartWrite(char* buffer)
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_write(buffer);
	}

#endif
}
bool Reader::getUartTxBusy()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_getTxBusy();
	}

#endif
}
