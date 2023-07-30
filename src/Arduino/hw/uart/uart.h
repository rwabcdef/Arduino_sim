#ifndef UART_H
#define UART_H


//#include <avr/io.h>
//#include "wiring_private.h"

#define UART_BUFF_LEN 32


#include <stdint.h>
#include "env.h"
//--------------------------------------------------------------------------------
void uart_init(char* pRxBuffer, uint8_t rxBufferLen);
//--------------------------------------------------------------------------------
bool uart_checkFrameRx();
//--------------------------------------------------------------------------------
uint8_t uart_getRxLenAndReset();
//--------------------------------------------------------------------------------
#define UART_STATUS_OK 1
#define UART_STATUS_BUSY 2

//uint8_t uart_write(char* buffer, uint8_t len);
uint8_t uart_write(char* buffer);
//--------------------------------------------------------------------------------
bool uart_getTxBusy();
//--------------------------------------------------------------------------------

#endif
