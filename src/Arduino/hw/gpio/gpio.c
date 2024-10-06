/*
 * gpio.c
 *
 *  Created on: 6 Jul 2024
 *      Author: rw123
 */

#include "env.h"
#include "gpio.h"
#include "DebugPrintFromC.hpp"

const uint8_t bitMask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

const uint8_t invBitMask[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

const char pinNumbers[] = {'0', '1', '2', '3', '4', '5', '6', '7'};

// Returns address of port register if port and pin are legal values,
// otherwise returns NULL.
static uint8_t* validatePortPin(uint8_t port, uint8_t pin);

#if defined(ENV_CONFIG__SYSTEM_PC)
#include <string.h>
static bool debugOn = true;
static void getPinSetDebugStr(uint8_t port, uint8_t pin, bool state, char* dest);
#endif

//--------------------------------------------------------------------------------
void gpio_setPinDirection(uint8_t port, uint8_t pin, uint8_t direction)
{
  // Validation
  uint8_t* portReg = validatePortPin(port, pin);

  if(portReg != NULL)
  {

  }
  else{ /* do nothing */ }

  // Unfinnished !
}
//--------------------------------------------------------------------------------
void gpio_setPinHigh(uint8_t port, uint8_t pin)
{
  uint8_t* portReg = validatePortPin(port, pin);

    if(portReg != NULL)
    {
      (*portReg) |= bitMask[pin];;
    }
    else{ /* do nothing */ }

#if defined(ENV_CONFIG__SYSTEM_PC)
    char s[12] = {0};
    getPinSetDebugStr(port, pin, true, s);
    debugWrite(s, Gpio);
#endif

}
//--------------------------------------------------------------------------------
void gpio_setPinLow(uint8_t port, uint8_t pin)
{
  uint8_t* portReg = validatePortPin(port, pin);

  if(portReg != NULL)
  {
    (*portReg) &= invBitMask[pin];
  }
  else{ /* do nothing */ }

#if defined(ENV_CONFIG__SYSTEM_PC)
    char s[12] = {0};
    getPinSetDebugStr(port, pin, true, s);
    debugWrite(s, Gpio);
#endif
}
//--------------------------------------------------------------------------------
void gpio_setDebugOn(bool on)
{
#if defined(ENV_CONFIG__SYSTEM_PC)
  debugOn = on;
#endif
}
//--------------------------------------------------------------------------------
uint8_t* validatePortPin(uint8_t port, uint8_t pin)
{
  if(pin > 7)
  {
    // pin is out of range
    return NULL;
  }
  if(port == GPIO_REG__PORTB)
  {
    return &PORTB;
  }
  else if(port == GPIO_REG__PORTC)
  {
    return &PORTC;
  }
  else if(port == GPIO_REG__PORTD)
  {
    return &PORTD;
  }
  else{ /* do nothing */ }

  return NULL;
}
//--------------------------------------------------------------------------------
#if defined(ENV_CONFIG__SYSTEM_PC)
void getPinSetDebugStr(uint8_t port, uint8_t pin, bool state, char* dest)
{
  char stateStr[5] = {0};
  char portChar = 'X';
  char pinChar = 'X';
  if(state)
  {
    strncpy(stateStr, "high\0", 5);
  }
  else
  {
    strncpy(stateStr, "low\0\0", 5);
  }

  if(port == GPIO_REG__PORTB){ portChar = 'B'; }
  else if(port == GPIO_REG__PORTC){ portChar = 'C'; }
  else if(port == GPIO_REG__PORTD){ portChar = 'D'; }

  if(pin < 8){ pinChar = pinNumbers[pin]; }

  sprintf(dest, "Pin %c%c %s", portChar, pinChar, stateStr);
}
#endif
