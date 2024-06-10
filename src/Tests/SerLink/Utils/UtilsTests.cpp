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
#include <SerLink_Utils.hpp>
#include "UtilsTests.hpp"


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

void UtilsTests::strToUint8()
{
  uint8_t strLen = 4, value;
  char str[strLen];

  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "253", 3);
  value = Utils::strToUint8(str);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "253", 3);
  value = Utils::strToUint8(str, 3);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "253", 3);
  value = Utils::strToUint8(str, 2);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "042", 3);
  value = Utils::strToUint8(str);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "042", 3);
  value = Utils::strToUint8(str, 3);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "060", 3);
  value = Utils::strToUint8(str, 3);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "007", 3);
  value = Utils::strToUint8(str);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
  memset(str, 0, strLen);
  strncpy(str, "007", 3);
  value = Utils::strToUint8(str, 3);
  printf("value [%s]: %d\n", str, value);
  //--------------------------------------
}
}
