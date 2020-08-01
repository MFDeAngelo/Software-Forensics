#pragma once

unsigned char* Crypt(unsigned char* data, int dataLength, unsigned int initialValue, unsigned int feedback);
unsigned char* Crypt(unsigned char* data, int dataLength, unsigned int initialValue);