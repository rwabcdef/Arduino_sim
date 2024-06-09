/*
 * Utils.cpp
 *
 *  Created on: 9 Jun 2024
 *      Author: rw123
 */
#include "Utils.hpp"

namespace SerLink
{

static const char digits[10] = {'0','1','2','3','4','5','6','7','8','9'};

void Utils::uint16ToStr(uint16_t value, char* str, uint8_t numDigits)
{
  uint16_t working = value;
  uint8_t i, j;

  // Zero pad the output string
  for(i=0; i<numDigits; i++)
  {
    str[i] = '0';
  }

  if(numDigits < 5)
  {
    // Reduce value to remove unwanted most significant digit(s).
    uint16_t modVal = 1;
    for(i=0; i<numDigits; i++)
    {
      modVal *= 10;
    }
    working = working % modVal;
  }

  for(i=0; i<numDigits; i++)
  {
    uint16_t divider = 1;
    for(j=0; j<(numDigits - i - 1); j++)
    {
      divider *= 10;
    }
    uint8_t digit = working / divider;
    str[i] = digits[digit];

    uint16_t subVal = digit * divider;
    working -= subVal;
  }
  str[i] = 0;
}

}


