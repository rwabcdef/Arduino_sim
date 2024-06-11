/*
 * ArdMod_LedRunner.hpp
 *
 *  Created on: 11 Jun 2024
 *      Author: rw123
 */

#ifndef ARDMOD_LEDRUNNER_HPP_
#define ARDMOD_LEDRUNNER_HPP_

#include <stdint.h>
#include "StateMachine.hpp"
#include "DebugUser.hpp"
#include "ArdMod_LedEvent.hpp"

namespace ArdMod
{

class LedRunner : public StateMachine, public DebugUser
{
  public:
    LedRunner();
    void setEvent(LedEvent& event);

  private:
    LedEvent event;
};

}




#endif /* ARDMOD_LEDRUNNER_HPP_ */
