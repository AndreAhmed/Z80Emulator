#include <string>
#include <iostream>
#include "Z80Emulator.h"



void Z80Emulator::Init_Emulator(std::string &input_file)
{
	ram.loadFromMemory(input_file);
	_registers.pc = 0x00;
	_registers.sp = 0xFFFF;
	_registers.a = 0xFF;
	
 }

void Z80Emulator::Execute()
{

	const uint8_t opcode = ram.read(_registers.pc);
	// Decode and execute opcode
	switch (opcode) {
	case 0x00:
		nop();
		break;
	case 0x01:
		ld(_registers.b); // LD dd, nn
		break;
	case 0x02:
		ld(_registers.b, _registers.a); // LD (BC), A
		break;
	case 0x03:
		inc_bc();
		break;
	case 0x04:
		inc_r(_registers.b);
		break;
	case 0x05:
		dec_r(_registers.b);
		break;
	case 0x06:
		ld('b'); // LD r, n
		break;
	case 0x07:
		rlca();
		break;
	case 0x08:
		
		break;
	case 0x09:
		add_hl(_registers.b);
	case 0x0A:
		ld_acc_address(_registers.b);
		break;
	case 0x0B:
		dec_bc();
		break;
	case 0x0C:
		inc_r(_registers.c);
		break;
	case 0x0D:
		dec_r(_registers.c);
		break;
	case 0x0E:
		ld('c');
		break;
	case 0x0F:
		rrca();
		break;
	case 0x10:
		djnz();
		break;
	case 0x11:
		ld_de();
		break;
	case 0x12:
		ld(_registers.d, _registers.a);
		break;
	case 0x13:
		ld_de(get_hl()+1);
		break;
	case 0x14:
		inc_r(_registers.d);
		break;
	case 0x15:
		dec_r(_registers.d);
		break;
	case 0x16:
		ld('d');
		break;
	case 0x17:
		rla();
		break;
	case 0x18:
		jr();
		break;
	case 0x19:
		add_hl(_registers.d);
		break;
	case 0x1A:
		ld_acc_address(_registers.d);
		break;
	case 0x1B:
		(_registers.d);
		break;
	case 0x1C:
		inc_r(_registers.e);
		break;
	case 0x1D:
		dec_r(_registers.e);
		break;
	case 0x1E:
		ld('e');
		break;
	case 0x1F:
		rra();
		break;
	case 0x20:
		jr(!flag::Z);
		break;
	case 0x21:
		ld(_registers.h);
		break;
	case 0x22:
		ld_nn_hl();
		break;
	case 0x23:
		inc_hl();
		break;
	case 0x24:
		inc_r(_registers.h);
		break;
	case 0x25:
		dec_r(_registers.h);
		break;
	case 0x26:
		ld(_registers.h);
		break;
	case 0x27:
		daa();
		break;
	case 0x28:
		jr(flag::Z);
		break;
	case 0x29:
		add_hl(_registers.h);
		break;
	case 0x2A:
		ld_hl_nn();
		break;
	case 0x2B:
		dec_hl();
		break;
	case 0x2C:
		inc_r(_registers.l);
		break;
	case 0x2D:
		dec_r(_registers.l);
		break;
	case 0x2F:
		cpl();
		break;
	case 0xC3:
		jump();
		break;
	case 0xF9:
		_registers.sp = get_hl();
		break;
	case 0x30:
		jr(!flag::C);
		break;
	case 0x31:

	default:
		std::cerr << "Encountered illegal or unimplemented opcode: 0x" << std::hex << std::uppercase << static_cast<int>(opcode) << std::endl;
		break;
	}

}
