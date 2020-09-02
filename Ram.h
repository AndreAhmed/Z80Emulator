#pragma once
#include <string>
#include <vector>
class Ram
{
public: 
	void loadFromMemory(std::string& fileName);
	uint8_t read(uint16_t address) const;
	void write(uint16_t address, uint8_t data);
private:
	static const int MAX_MEMORY_LENGTH = 65536;
	std::vector<uint8_t> memory;

};

