/*
 * ArdMod_LedModule.hpp
 *
 *  Created on: 11 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_LEDMODULE_HPP_
#define ARDMOD_LEDMODULE_HPP_

#include <stdint.h>
#include "StateMachine.hpp"
#include "DebugUser.hpp"
#include "ArdMod_LedEvent.hpp"

namespace ArdMod
{

class LedModule : public StateMachine, public DebugUser
{
  public:
    LedModule();
    void setEvent(LedEvent& event);

  private:
    //LedEvent event;
};

}



#endif /* ARDMOD_LEDMODULE_HPP_ */
