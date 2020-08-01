#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>


class KDB {
public:
	KDB(char* filePath);
	void outputData();
	std::string getEntryData(uint16_t);
private:
	static const uint8_t MAGIC_SIZE = 6;
	std::string magic;
	class Entry {
	public:
		static const uint8_t NAME_SIZE = 16;
		Entry(char* name, uint32_t blockListPtr);
		void addBlock(uint16_t size, uint32_t dataPtr);
		std::string name;
		uint32_t blockListPtr;
		class Block {
		public:
			Block(uint16_t size, uint32_t dataPtr);
			~Block();
			uint16_t size;
			uint32_t dataPtr;
			unsigned char* data;
				
		};
		std::vector<Block> blocks;
	};
	std::vector<Entry> entries;
};