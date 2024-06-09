/*
 * Utils.hpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <stdint.h>

namespace SerLink
{
class Utils{
public:
	static void uint16ToStr(uint16_t value, char* str, uint8_t numDigits = 5);

};

}
#endif /* UTILS_HPP_ */
