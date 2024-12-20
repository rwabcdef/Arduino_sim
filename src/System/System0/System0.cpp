/*
 * System0.cpp
 *
 *  Created on: 13 Dec 2024
 *      Author: rw123
 */


#include "System0.hpp"

//-------------------------------------------------
// reader0 & writer0

char readerRxBuffer[UART_BUFF_LEN];
char readerAckBuffer[UART_BUFF_LEN];
char writerTxBuffer[UART_BUFF_LEN];

char writerTxFrameBuffer[UART_BUFF_LEN];
char writerAckFrameBuffer[UART_BUFF_LEN];
SerLink::Frame writerTxFrame(writerTxFrameBuffer);
SerLink::Frame writerAckFrame(writerAckFrameBuffer);

char readerRxFrameBuffer[UART_BUFF_LEN];
char readerAckFrameBuffer[UART_BUFF_LEN];
SerLink::Frame readerRxFrame(readerRxFrameBuffer);
SerLink::Frame readerAckFrame(readerAckFrameBuffer);

SerLink::Writer writer0(WRITER_CONFIG__WRITER0_ID, writerTxBuffer,
    UART_BUFF_LEN, &writerTxFrame, &writerAckFrame);

SerLink::Reader reader0(READER_CONFIG__READER0_ID, readerRxBuffer, readerAckBuffer,
    UART_BUFF_LEN, &readerRxFrame, &readerAckFrame, &writer0);

//----------------------------------------------
// transport0

char transport0RxFrameBuffer[UART_BUFF_LEN];
char transport0TxFrameBuffer[UART_BUFF_LEN];

SerLink::Frame transport0RxFrame(transport0RxFrameBuffer);
SerLink::Frame transport0TxFrame(transport0TxFrameBuffer);

SerLink::Transport transport0(&reader0, &writer0, &transport0RxFrame, &transport0TxFrame);
//----------------------------------------------

