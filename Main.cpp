/**************************************************************
Z80 Emulator by Ahmed S. Tolba. 
02.09.2020
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "Z80Emulator.h"

using namespace std;

int main()
{
	Z80Emulator emu;
	std::string input("zexdoc.com");
	emu.Init_Emulator(input);

	for (;;) // better than while(1), compiler might check if 1 is true or not :D, hence it consume cycles :/
	{
	
		emu.Execute();
	}


	return 0;
}