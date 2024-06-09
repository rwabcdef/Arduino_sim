/*
 * UtilsTests.cpp
 *
 *  Created on: 9 Jun 2024
 *      Author: rw123
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UtilsTests.hpp"
#include "Utils.hpp"


namespace SerLink
{

void UtilsTests::uint16ToStr()
{
  uint8_t strLen = 8;
  char str[strLen];

  //--------------------------------------
  uint16_t value = 42361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 42361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 5);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 42361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 3);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 40361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 40361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 5);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 40361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 4);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 40361;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 3);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 508;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 3);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 508;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 4);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
  value = 508;
  memset(str, 0, strLen);
  Utils::uint16ToStr(value, str, 5);
  printf("value: %d\tconversion: %s\n", value, str);
  //--------------------------------------
}

}
