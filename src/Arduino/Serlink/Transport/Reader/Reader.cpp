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
	//this->rxBuffer = rxBuffer;
	//this->ackBuffer = ackBuffer;
	this->bufferLen = UART_BUFF_LEN;
	//this->debugPrinter = debugPrint;
	this->currentState = IDLE;
	this->debugLevel = DebugPrint_defs::UartRx;
	this->debugOn = true;

	uart_init((char*) this->rxBuffer, this->bufferLen);
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

uint8_t Reader::idle()
{

	cli();
	if(this->checkFrameRx())
	{
		uint8_t rxLen = this->getRxLenAndReset();



		//Frame::fromString((char*) this->rxBuffer, &this->rxFrame);
		Frame::fromString((char*) this->rxBuffer, &this->rxFrame);

		if(this->rxFrame.type == Frame::TYPE_TRANSMISSION)
		{
			this->rxFrame.copy(&this->ackFrame);
			this->ackFrame.type = Frame::TYPE_ACK;
			this->ackFrame.dataLen = Frame::ACK_OK;
			memset(&this->ackFrame.data, 0, Frame::MAX_DATALEN);

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

		this->write((char*)this->ackBuffer);
		//this->write((char*)"hello\n\0");

		return TXACKWAIT;
	}
	return ACKDELAY;
}
uint8_t Reader::txAckWait()
{
	if(!this->getTxBusy())
	{
		sprintf(this->s, "Reader::txAckWait() done\n\0", 0);
		this->debugWrite(this->s);
		return IDLE;
	}
	return TXACKWAIT;
}
//-------------------------------------------------------------
bool Reader::checkFrameRx()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_checkFrameRx();
	}

#endif
}
uint8_t Reader::getRxLenAndReset()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_getRxLenAndReset();
	}

#endif
}
uint8_t Reader::write(char* buffer)
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_write(buffer);
	}

#endif
}
bool Reader::getTxBusy()
{
#ifdef READER_CONFIG__READER0

	if(this->id == READER_CONFIG__READER0_ID)
	{
		return uart_getTxBusy();
	}

#endif
}
