/*
 * Event.hpp
 *
 *  Created on: 4 Jun 2024
 *      Author: rob
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <stdint.h>


namespace ArdMod
{

class Event{

public:

  Event(char act);
  void serialise(char* str, uint8_t* strLen);


  char action;

};

}



#endif /* EVENT_HPP_ */
