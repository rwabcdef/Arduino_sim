/*
 * Frame.cpp
 *
 *  Created on: 22 Apr 2023
 *      Author: rw123
 */
#include "Frame.hpp"
#include<string.h>
#include<stdlib.h>
#include <stdio.h>

using namespace SerLink;

static const char digits[10] = {'0','1','2','3','4','5','6','7','8','9'};

Frame::Frame(char* pBuffer)
{
	strncpy(this->protocol, "00000", Frame::LEN_PROTOCOL);
	this->type = '0';
	this->rollCode = 0;
	this->dataLen = 0;
	this->buffer = pBuffer;
	//memset(this->data, 0, Frame::MAX_DATALEN);
}

Frame::Frame(char* pProtocol, char type, uint16_t rollCode, char* pBuffer, uint16_t dataLen, char* pData)
{
	strncpy(this->protocol, pProtocol, Frame::LEN_PROTOCOL);
	this->type = type;
	this->rollCode = rollCode;
	this->dataLen = dataLen;
	this->buffer = pBuffer;
	if(this->dataLen >= Frame::ACK_OK)
	{
	  // dataLen field is being used as ack status - so DO NOT copy data into buffer
	}
	else
	{
	  // Copy data into this frame data buffer
	  strncpy(this->buffer, pData, this->dataLen);
	}
}

void Frame::setProtocol(char* pProtocol)
{
  strncpy(this->protocol, pProtocol, Frame::LEN_PROTOCOL);
}

void Frame::toString(char* const pStr, uint8_t* pRetCode)
{
  uint16_t eofIndex;
  if(this->dataLen >= Frame::ACK_OK)
  {
    eofIndex = Frame::LEN_HEADER;
  }
  else
  {
    eofIndex = Frame::LEN_HEADER + this->dataLen;
  }

  // clear buffer
  memset(pStr, 0, eofIndex + 2);

	strncpy(pStr, this->protocol, Frame::LEN_PROTOCOL);
	pStr[Frame::INDEX_START_TYPE] = this->type;
	Frame::int3dToStr(this->rollCode, &pStr[Frame::INDEX_START_ROLLCODE]);
	Frame::int3dToStr(this->dataLen, &pStr[Frame::INDEX_START_DATALEN]);

	if(this->dataLen < Frame::ACK_OK)
	{
	  // Copy data
	  strncpy(&pStr[Frame::INDEX_START_DATA], this->buffer, this->dataLen);
	}

	pStr[eofIndex] = '\n';
}

void Frame::fromString(char* str, Frame* pFrame)
{
	strncpy(pFrame->protocol, str, Frame::LEN_PROTOCOL);
	pFrame->type = str[Frame::INDEX_START_TYPE];
	Frame::str3dToInt(&str[Frame::INDEX_START_ROLLCODE], &pFrame->rollCode);
	Frame::str3dToInt(&str[Frame::INDEX_START_DATALEN], (uint16_t*)&pFrame->dataLen);

  if(pFrame->dataLen < Frame::ACK_OK)
  {
    strncpy(pFrame->buffer, &str[Frame::INDEX_START_DATA], pFrame->dataLen);
  }
}

void Frame::copy(Frame* copy)
{
	strncpy(copy->protocol, this->protocol, Frame::LEN_PROTOCOL);
	copy->type = this->type;
	copy->rollCode = this->rollCode;
	copy->dataLen = this->dataLen;

  if(this->dataLen < Frame::ACK_OK)
  {
    strncpy(copy->buffer, this->buffer, this->dataLen);
  }
}

void Frame::incRollCode(uint16_t* rollCode)
{
  uint8_t maxValue = 5;
  if(Frame::LEN_ROLLCODE == 1) { maxValue = 9; }
  else if(Frame::LEN_ROLLCODE == 2) { maxValue = 99; }
  else { maxValue = 999; }

  if(*rollCode >= maxValue)
  {
    *rollCode = 0;
  }

  (*rollCode)++;
}

void Frame::int3dToStr(const uint16_t value, char* pStr)
{
  uint16_t hundreds = value / 100;
  uint16_t tens = (value - (hundreds * 100)) / 10;
  uint16_t units = value % 10;

	pStr[0] = digits[hundreds];
	pStr[1] = digits[tens];
	pStr[2] = digits[units];
}

void Frame::str3dToInt(char* str, uint16_t* value)
{
	uint16_t hundredsComponent = Frame::digitCharValue(str[0]) * 100;
	uint16_t tensComponent = Frame::digitCharValue(str[1]) * 10;
	uint16_t unitsComponent = Frame::digitCharValue(str[2]);

	*value = hundredsComponent + tensComponent + unitsComponent;
}

uint8_t Frame::digitCharValue(char c)
{
	uint8_t i;
	for(i=0; i<10; i++)
	{
		if(c == digits[i])
		{
			return i;
		}
	}
	return 0;
}

void Frame::toDebugString(char* pStr)
{
	uint16_t index = 0;
	char protocol[6] = {0};
	strncpy(protocol, this->protocol, Frame::LEN_PROTOCOL);
	index += sprintf(&pStr[index], "protocol: %s\n", protocol);
	index += sprintf(&pStr[index], "type: %c\n", this->type);
	index += sprintf(&pStr[index], "rollCode: %d\n", this->rollCode);
	index += sprintf(&pStr[index], "dataLen: %d\n", this->dataLen);
	index += sprintf(&pStr[index], "buffer: %s\n", this->buffer);
}
