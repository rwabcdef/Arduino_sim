/*
 * Reader.hpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */

#ifndef READER_HPP_
#define READER_HPP_

#include "Reader_config.hpp"
#include "StateMachine.hpp"
#include "Frame.hpp"
#include "Transport.hpp"
#include "DebugPrint.hpp"
#include "DebugUser.hpp"

namespace SerLink
{
typedef void (*readHandler)(const TransportData* pRxData, TransportData* pAckData);
/*
typedef void (*reader_uart_init)(char* pRxBuffer, uint8_t rxBufferLen);
typedef bool (*reader_uart_checkFrameRx)();
typedef uint8_t (*reader_uart_getRxLenAndReset)();

class ReaderAdapter
{
public:
	reader_uart_init init;
	reader_uart_checkFrameRx checkFrameRx;
	reader_uart_getRxLenAndReset getRxLenAndReset;
};
*/

class Reader : public StateMachine, public DebugUser
{
private:
	//const uint8_t IDLE = 0;
	//const uint8_t ACKDELAY = 1;
	//const uint8_t TXACKWAIT = 2;
	uint8_t id;
	// char* rxBuffer;
	//volatile char* ackBuffer;
	uint8_t bufferLen;
	uint16_t startTick;
	//DebugPrint* debugPrinter;
	char rxBuffer[UART_BUFF_LEN];
	char ackBuffer[UART_BUFF_LEN];
	char s[64];
	Frame rxFrame;
	Frame ackFrame;

	uint8_t idle();
	uint8_t ackDelay();
	uint8_t txAckWait();

	bool checkFrameRx();
	uint8_t getRxLenAndReset();
	uint8_t write(char* buffer);
	bool getTxBusy();
public:
	Reader(uint8_t id); // , DebugPrint* debugPrint = nullptr
	void run();
	uint8_t getCurrentState();
	char* getCurrentStateName();
};

}

#endif /* READER_HPP_ */
