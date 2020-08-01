#include <string>
#include <fstream>
#include <iostream>

#include "KDB.hpp"
#include "MD5.hpp"


int main(int argc, char* argv[]) {
	//Create output directory.
	int index = 0;
	while (argv[2][index] == '.' || argv[2][index] == '/') index++;
	std::string directoryName(argv[2] + index); //Ignore initial .
	directoryName = directoryName.substr(0, directoryName.find(".")) + "_Repaired";
	std::string dosCommand = "mkdir " + directoryName;
	system(dosCommand.c_str());
	system("CLS");


	//Get the magic bytes from file 1
	KDB kdb(argv[1]);
	std::string magicString = kdb.getEntryData(0);
	unsigned char* magicBytes = nullptr;
	uint32_t magicBytesCount = magicString.length();
	magicBytes = new unsigned char[magicBytesCount];
	for (int i = 0; i < magicBytesCount; i++) {
		magicBytes[i] = magicString[i];
	}


	//Set up file 2 and missing bytes
	std::ifstream file;
	file.open(argv[2], std::ios::binary);
	unsigned char* jpegMissingBytes = nullptr;
	jpegMissingBytes = new unsigned char[3];
	jpegMissingBytes[0] = 0xFF;
	jpegMissingBytes[1] = 0xD8;
	jpegMissingBytes[2] = 0xFF;


	//Iterate over file scanning for magic bytes
	int i;
	unsigned char currentByte;
	bool next;
	while (!file.eof()) {
		next = false;
		for (i = 0; i < magicBytesCount; i++) {
			file.read(reinterpret_cast<char*>(&currentByte), sizeof(currentByte));
			
			if (magicBytes[i] != currentByte) {
				next = true;
				break;
			}
		}
		if (next) continue;
		uint64_t startJPEG = static_cast<uint64_t>(file.tellg()) - 3;

		//Looking for end of JPG file
		std::array<unsigned char, 2> trailingBytes = { 0xFF, 0xD9 };
		next = false;
		while (!file.eof()) {
			next = false;
			for (i = 0; i < trailingBytes.size(); i++) {
				file.read(reinterpret_cast<char*>(&currentByte), sizeof(currentByte));

				if (trailingBytes[i] != currentByte) {
					next = true;
					break;
				}
			}
			if (!next) { 
				break;
			}
		}
		uint64_t endJPEG = static_cast<uint64_t>(file.tellg());


		//Output file information
		unsigned char* jpegBytes = nullptr;
		jpegBytes = new unsigned char[endJPEG - startJPEG];
		file.seekg(startJPEG, std::ios::beg);
		file.read(*reinterpret_cast<char**>(&jpegBytes), endJPEG - startJPEG);
		for (int i = 0; i < 3; i++) {
			jpegBytes[i] = jpegMissingBytes[i];
		}
		std::ofstream jpeg;
		std::string path = "./" + directoryName + "/" + std::to_string(startJPEG) + ".jpg";
		jpeg.open(path, std::ios::binary);
		jpeg.write(*reinterpret_cast<char**>(&jpegBytes), endJPEG - startJPEG);
		jpeg.close();

		std::cout << "Offset: " << startJPEG - 3 << std::endl;
		std::cout << "Size: " << endJPEG - startJPEG << " bytes" << std::endl;
		std::cout << "MD5 Hash: " << md5(jpegBytes, endJPEG-startJPEG) << std::endl;
		std::cout << "Path: " << path << std::endl << std::endl;
		
		file.seekg(endJPEG);

		delete[] jpegBytes;
		jpegBytes = nullptr;

	}

	//Clean up
	delete[] jpegMissingBytes;
	jpegMissingBytes = nullptr;
	delete[] magicBytes;
	magicBytes = nullptr;
	return 0;
}