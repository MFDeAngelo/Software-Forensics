
#include <iostream>


#include "LFSR.hpp"

int main() {
	//An example of my working Crypt function
	unsigned char* word = nullptr;
	word = new unsigned char[5];
	word[0] = 0xCD;
	word[1] = 0x01;
	word[2] = 0xEF;
	word[3] = 0xD7;
	word[4] = 0x30;
	unsigned char* output = nullptr;
	output = new unsigned char[5];
	output = Crypt(word, 5, 0x12345678);
	
	std::cout << "Input : 0xCD 0x01 0xEF 0xD7 0x30" << std::endl;
	std::cout << "Output: ";
	for (int i = 0; i < 5; i++) {
		std::cout << output[i];
	}
	std::cout << std::endl;
	delete[] output;
	return 0;
}