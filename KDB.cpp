#include <fstream>
#include <iostream>

#include "KDB.hpp"
#include "LFSR.hpp"

//Uses kdb file located at <filePath> and stores all data from kdb in this instance.
KDB::KDB(char* filePath):
	magic(std::string(MAGIC_SIZE,' ')),
	entries()
{

    //Open the correct file.
    std::ifstream file;
    file.open(filePath, std::ios::binary); //adding std::ios::binary http://www.cplusplus.com/doc/tutorial/files/


    //Get the magic
    char* magicPtr = nullptr;
    magicPtr = new char[6];
    file.read(magicPtr, 6); //I wasn't familiar with this function. http://www.cplusplus.com/doc/tutorial/files/
    for (uint16_t i = 0; i < MAGIC_SIZE; i++) {
        this->magic[i] = magicPtr[i];
    }
    delete[] magicPtr;
    magicPtr = nullptr;


    //Get entry names and respective pointers
    int entryListPtr;
    file.read(reinterpret_cast<char*>(&entryListPtr), sizeof(entryListPtr)); //Adapting this function to work for an integer https://stackoverflow.com/questions/32066027/read-32-bit-integer-from-binary-file-in-c
    file.seekg(entryListPtr, std::ios::beg);
    int nextFourBytes;
    while (file.read(reinterpret_cast<char*>(&nextFourBytes), sizeof(nextFourBytes)), nextFourBytes ^ 0xFFFFFFFF) {
        file.seekg(-4, std::ios::cur);
        char* entryName = nullptr;
        entryName = new char[16];
        file.read(entryName, 16);
        uint32_t blockListPtr;
        file.read(reinterpret_cast<char*>(&blockListPtr), sizeof(blockListPtr));
        this->entries.push_back(Entry(entryName, blockListPtr));
    }


    //Get blocks for each entry
    for (Entry& entry : this->entries) {
        file.seekg(entry.blockListPtr, std::ios::beg);

        while (file.read(reinterpret_cast<char*>(&nextFourBytes), sizeof(nextFourBytes)), nextFourBytes ^ 0xFFFFFFFF) {
            file.seekg(-4, std::ios::cur);
            uint16_t blockSize;
            file.read(reinterpret_cast<char*>(&blockSize), sizeof(blockSize));
            uint32_t dataPtr;
            file.read(reinterpret_cast<char*>(&dataPtr), sizeof(dataPtr));
            entry.addBlock(blockSize, dataPtr);
        }
    }


    //Gets data for each block
    for (Entry& entry : this->entries) {
        for (Entry::Block& block : entry.blocks) {
            file.seekg(block.dataPtr);
            file.read(*reinterpret_cast<char**>(&block.data), block.size);
        }
    }


    //Close the file
    file.close();
}

//Output decrypted data from all entries
void KDB::outputData() {

    //Combine codeblocks and write to file
    for (Entry& entry : this->entries) {
        uint64_t dataSize = 0;
        for (KDB::Entry::Block& block : entry.blocks) {
            dataSize += block.size;
        }
        unsigned char* dataStream = nullptr;
        dataStream = new unsigned char[dataSize];
        uint64_t buffer = 0;
        for (Entry::Block& block : entry.blocks) {
            for (uint32_t i = 0; i < block.size; i++, buffer++) {
                dataStream[buffer] = block.data[i];

            }
        }

        std::cout << entry.name << ": ";
        unsigned char* output = Crypt(dataStream, dataSize, 0x4F574154);
        std::cout.write(reinterpret_cast<char*>(output), dataSize);
        std::cout << "\n";


        delete[] output;
        output = nullptr;
        delete[] dataStream;
        dataStream = nullptr;
    }
}

//Returns a string of decrepted data from <index>'th entry
std::string KDB::getEntryData(uint16_t index)
{
    //Combine codeblocks and write to file
    Entry& entry = this->entries[index];
    uint64_t dataSize = 0;
    for (KDB::Entry::Block& block : entry.blocks) {
        dataSize += block.size;
    }
    unsigned char* dataStream = nullptr;
    dataStream = new unsigned char[dataSize];
    uint64_t buffer = 0;
    for (Entry::Block& block : entry.blocks) {
        for (uint32_t i = 0; i < block.size; i++, buffer++) {
            dataStream[buffer] = block.data[i];

        }
    }
    
    std::string result(reinterpret_cast<char const*>(Crypt(dataStream, dataSize, 0x4F574154)), dataSize); //Using the reinterpret cast to construct a string https://stackoverflow.com/questions/1673445/how-to-convert-unsigned-char-to-stdstring-in-c

    delete[] dataStream;
    dataStream = nullptr;

    return result;
}

//Initialize an entry
KDB::Entry::Entry(char* name, uint32_t blockListPtr):
	name(std::string(NAME_SIZE, ' ')),
	blockListPtr(blockListPtr)
{
	for (uint16_t i = 0; i < NAME_SIZE; i++) {
		this->name[i] = name[i];
	}
}

//Add a block to the current entry
void KDB::Entry::addBlock(uint16_t size, uint32_t dataPtr) {
	blocks.push_back(*(new KDB::Entry::Block(size, dataPtr)));
}

//Initialize a block to prepare for data input
KDB::Entry::Block::Block(uint16_t size, uint32_t dataPtr) :
	size(size),
	dataPtr(dataPtr),
	data(new unsigned char[size])
{}


KDB::Entry::Block::~Block()
{
	delete[] data;
}
