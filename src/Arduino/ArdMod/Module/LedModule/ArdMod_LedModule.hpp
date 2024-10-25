/*
 * ArdMod_LedModule.hpp
 *
 *  Created on: 11 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_LEDMODULE_HPP_
#define ARDMOD_LEDMODULE_HPP_

#include <stdint.h>
#include <stdbool.h>
#include "StateMachine.hpp"
#include "DebugUser.hpp"
#include "ArdMod_LedEvent.hpp"

namespace ArdMod
{

/*
namespace LedModule_defs
{
  typedef void (*ledOn)(uint8_t port, uint8_t pin);
  typedef void (*ledOff)(uint8_t port, uint8_t pin);
}
*/

class LedModule : public EventConsumer , public StateMachine, public DebugUser
{
  public:
    LedModule(uint8_t port, uint8_t pin);
    //void setEvent(LedEvent& event);
    void run();

  private:
    uint8_t port;
    uint8_t pin;
    //bool inputEvent;
    //LedEvent event;
    uint16_t periodCount;
    //LedModule_defs::ledOn ledOn;
    //LedModule_defs::ledOff ledOff;

    // State methods
    uint8_t on();
    uint8_t off();
    uint8_t delay();

    uint8_t common();
};

}

#endif /* ARDMOD_LEDMODULE_HPP_ */
