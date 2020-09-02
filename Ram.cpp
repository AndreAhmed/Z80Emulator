#include "Ram.h"
#include <fstream>

void Ram::loadFromMemory(std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::in);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    memory.reserve(fileSize);

    // read the data:
    memory.insert(memory.begin(),
        std::istream_iterator<uint8_t>(file),
        std::istream_iterator<uint8_t>());



}

uint8_t Ram::read(uint16_t address) const
{
	return memory[address];
}

void Ram::write(uint16_t address, uint8_t data)
{
	memory[address] = data;
}