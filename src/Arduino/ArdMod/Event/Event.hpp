/*
 * Event.hpp
 *
 *  Created on: 4 Jun 2024
 *      Author: rob
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

namespace ArdMod
{

class Event{

public:

  Event(char cmd, char comp = '0');
  char getCommand();
  char getComponent();

private:
  char commmand;
  char component;

};

}



#endif /* EVENT_HPP_ */
