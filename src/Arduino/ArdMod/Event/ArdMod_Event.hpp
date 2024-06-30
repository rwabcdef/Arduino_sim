/*
 * Event.hpp
 *
 *  Created on: 4 Jun 2024
 *      Author: rob
 */

#ifndef ARDMOD_EVENT_HPP_
#define ARDMOD_EVENT_HPP_

#include <stdint.h>


namespace ArdMod
{

class Event{

public:

  Event();
  virtual uint8_t serialise(char* str);
  static bool deSerialise(char* str, Event* outEvent);
  virtual void copy(Event* copyEvent);

  char action;

};

}



#endif /* ARDMOD_EVENT_HPP_ */
