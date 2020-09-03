#pragma once
#include "Registers.h"
#include "Ram.h"
#include "Z80CPU.h"
#include <bitset>
class Z80Emulator: public Z80CPU
{
public:
	Z80Emulator():Z80CPU(ram){}
	void Init_Emulator(std::string& input_file);
	void Execute();
private:
	Ram ram;
	bool isEmuInitialized;
};

