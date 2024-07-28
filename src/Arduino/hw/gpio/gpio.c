/*
 * gpio.c
 *
 *  Created on: 6 Jul 2024
 *      Author: rw123
 */

#include "env.h"
#include "gpio.h"

const uint8_t bitMask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

const uint8_t invBitMask[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

static uint8_t* validatePortPin(uint8_t port, uint8_t pin);

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
