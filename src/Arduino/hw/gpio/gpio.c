/*
 * gpio.c
 *
 *  Created on: 6 Jul 2024
 *      Author: rw123
 */

#include "env_config.h"
#include "gpio.h"

//--------------------------------------------------------------------------------
void gpio_setPinDirection(uint8_t port, uint8_t pin, uint8_t direction)
{
  // Validation
  if(port != GPIO_REG__PORTB || port != GPIO_REG__PORTC || port != GPIO_REG__PORTD)
  {
    return;
  }

  // Unfinnished !
}
//--------------------------------------------------------------------------------
void gpio_setPinState(uint8_t port, uint8_t pin, bool state)
{

}
//--------------------------------------------------------------------------------
