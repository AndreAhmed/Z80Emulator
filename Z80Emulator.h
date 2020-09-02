#pragma once
#include "Registers.h"
#include "Ram.h"
#include <bitset>
class Z80Emulator
{
public:
	void Init_Emulator(std::string& input_file);
	void Execute();
private:
	Registers _registers;
	Ram ram;
	bool isEmuInitialized;

	 
	void nop()
	{
		++_registers.pc;
	}
	// set a flag to a given value (true or false)
	void  setFlag(const flag::StatusFlag statusFlag, bool value)
	{
		_registers.f &= ~(0x01 << statusFlag); // force that value to 0
		_registers.f |= (static_cast<uint8_t>(value) << statusFlag); // set flag with appropriate value
	}

	void add_a_r(uint8_t reg)
	{
		uint8_t acc = _registers.a;
		uint8_t val = reg;
		uint8_t result = acc + val;

		setFlag(flag::S, static_cast<int8_t>(result) < 0);
		setFlag(flag::Z, result == 0);
		bool halfCarry = (acc & 0x0F) + (val & 0x0F) > 15;
		setFlag(flag::H, halfCarry);
		bool overflow = acc + val > 255;
		setFlag(flag::P, overflow);
		setFlag(flag::N, false);
		setFlag(flag::C, overflow);
		_registers.a = result;
		++_registers.pc;
	}

	void add_hl(uint8_t reg)
	{
		uint8_t hl = _registers.a;
		uint8_t other = reg;
		uint8_t result = hl + other;

		setFlag(flag::S, static_cast<int8_t>(result) < 0);
		setFlag(flag::Z, result == 0);
		bool overflow = hl + other > 65535;
		setFlag(flag::C, overflow);
		setFlag(flag::P, overflow);
		bool halfCarry = (hl & 0x0FFF) + (other & 0x0FFF) > 4095;
		setFlag(flag::H, halfCarry);
		setFlag(flag::N, false);
		_registers.h = result;
		_registers.pc += 1;
	}

	void inc_r(uint8_t reg)
	{
		_registers.r = inc(reg);
		++_registers.pc;
	}
	void inc_bc()
	{
		uint16_t val =  get_bc() + 1;
		_registers.b = val >> 8;
		_registers.c = val & 0xFF;
	}


	uint8_t inc(const uint8_t value)
	{
		uint8_t result = value + 1;
		setFlag(flag::S, static_cast<int8_t>(result) < 0);
		setFlag(flag::Z, result == 0);
		bool halfCarry = (value & 0x0F) + 0x01 > 15;
		setFlag(flag::H, halfCarry);
		setFlag(flag::P, value == 0x7F);
		setFlag(flag::N, false);
		return result;
	}

	void dec_r(uint8_t reg)
	{
		_registers.r = dec(reg);
		++_registers.pc;
	}

	void dec_bc()
	{
		uint16_t val = get_bc() - 1;
		_registers.b = val >> 8;
		_registers.c = val & 0xFF;
	}

	void inc_hl()
	{
		uint16_t val = get_hl() + 1;
		_registers.h = val >> 8;
		_registers.l = val & 0xFF;
	}

	void dec_hl()
	{
		uint16_t val = get_hl() - 1;
		_registers.h = val >> 8;
		_registers.l = val & 0xFF;
	}
	uint8_t dec(const uint8_t value)
	{
		uint8_t result = value - 1;
		setFlag(flag::S, static_cast<int8_t>(result) < 0);
		setFlag(flag::Z, result == 0);
		setFlag(flag::H, (value & 0x0F) < 0x01); // Borrow occurs if lower nibble subtrahend > lower nibble minuend
		setFlag(flag::P, value == 0x80);
		setFlag(flag::N, true);
		return result;
	}

	void ld(char which)
	{
		uint8_t value = ram.read(_registers.pc + 1);
		switch (which)
		{
		case 'c':
			_registers.c = value;
			break;
		case 'b':
			_registers.b = value;
			break;
		case 'd':
			_registers.d = value;
			break;
		case 'e':
			_registers.e = value;
			break;
		default:
			break;
		}
		
		_registers.pc += 2;
	}

	void ld_e()
	{
		uint8_t value = ram.read(_registers.pc + 1);
		_registers.e = value;
		_registers.pc += 2;
	}

	void ld_de(uint8_t value)
	{
		_registers.d = value >> 8;
		_registers.e = value & 0xFF;
		_registers.pc += 3;
	}
	void ld_de()
	{
		uint8_t high = ram.read(_registers.pc + 1 );
		uint8_t low = ram.read(_registers.pc + 2);
		_registers.d = low >> 8;
		_registers.e= high & 0xFF;
		_registers.pc += 3;
	}

	void ld(const uint16_t addressReg, uint8_t src)
	{
		ram.write(addressReg, src);
		_registers.pc += 1;
	}

	void ld_acc_address(uint16_t addressReg)
	{
		uint16_t address = addressReg;
		uint8_t value = ram.read(address);
		_registers.a =  value;
		_registers.pc += 1;
	}

	void ld_nn_hl()
	{
		uint8_t low_order = ram.read(_registers.pc + 1);
		uint8_t high_order = ram.read(_registers.pc + 2);
		uint16_t address = (high_order << 8) | low_order;
		ram.write(address, _registers.l);
		ram.write(address + 1, _registers.h);
		_registers.pc += 3;
	}

	void ld_hl_nn()
	{
		uint8_t low_order = ram.read(_registers.pc + 1);
		uint8_t high_order = ram.read(_registers.pc + 2);
		uint16_t address = (high_order << 8) | low_order;
		_registers.l = ram.read(address);
		_registers.h = ram.read(address + 1);
		_registers.pc += 3;
	}

	void rlca()
	{
		uint8_t acc = _registers.a;
		uint8_t shifted = (acc << 1) | (acc >> 7);
		setFlag(flag::H, false);
		setFlag(flag::N, false);
		setFlag(flag::C, acc >> 7);
		_registers.a= shifted;
		_registers.pc += 1;
	}

	void rrca()
	{
		uint8_t acc = _registers.a;
		uint8_t shifted = (acc >> 1) | (acc << 7);
		setFlag(flag::H, false);
		setFlag(flag::N, false);
		setFlag(flag::C, acc & 0x01);
		_registers.a= shifted;
		_registers.pc += 1;
	}

	void rra()
	{
		uint8_t acc = _registers.a;
		uint8_t shifted = (acc >> 1) | (_registers.c << 7);
		setFlag(flag::H, false);
		setFlag(flag::N, false);
		setFlag(flag::C, acc & 0x01);
		_registers.a= shifted;
		_registers.pc += 1;
	}

	void rla()
	{
		uint8_t acc = _registers.a;
		uint8_t shifted = (acc << 1) | _registers.c;
		setFlag(flag::H, false);
		setFlag(flag::N, false);
		setFlag(flag::C, acc >> 7);
		_registers.a= shifted;
		_registers.pc += 1;
	}

	// Jump group

	void djnz()
	{
		uint8_t b_value = --_registers.b;
		uint8_t offset = ram.read(_registers.pc + 1);
		_registers.pc += 2;
		if (b_value != 0) { // jump
			_registers.pc += static_cast<int8_t>(offset); // TODO: does static cast need to be here for proper pos/neg offset?
		}
	}

	void jr()
	{
		uint8_t offset = ram.read(_registers.pc + 1);
		_registers.pc += 2 + static_cast<int8_t>(offset);
	}

	void jr(const bool jump_condition)
	{
		_registers.pc += 2;
		uint8_t offset = ram.read(_registers.pc + 1);
		if (jump_condition) {
			_registers.pc += static_cast<int8_t>(offset);
		}
	}
	uint16_t nextw() {
		uint16_t r1 = ram.read(_registers.pc+1);
		return r1;
	}
	void jump()
	{
		_registers.pc = nextw();
	}

	uint16_t get_hl() {
		_registers.pc++;
		return (_registers.h << 8) | _registers.l;
	}
	uint16_t get_de() {
		_registers.pc++;
		return (_registers.d << 8) | _registers.e;
	}

	uint16_t get_bc() {
		_registers.pc++;
		return (_registers.b << 8) | _registers.c;
	}
	void daa()
	{
		// Implements algorithm from http://www.worldofspectrum.org/faq/reference/z80reference.htm
		const uint8_t acc = _registers.a;
		uint8_t correction = 0;
		if (acc > 0x99 || _registers.c) {
			correction |= 0x60;
			setFlag(flag::C, true);
		}
		else {
			setFlag(flag::C, false);
		}

		if ((acc & 0x0F) > 9 || _registers.h) {
			correction |= 0x06;
		}

		uint8_t result = acc;
		if (!flag::N) {
			result += correction;
		}
		else {
			result -= correction;
		}

		setFlag(flag::S, result >> 7);
		setFlag(flag::Z, result == 0);
		setFlag(flag::H, ((acc ^ result) >> 4) & 0x01);
		bool is_even_parity = std::bitset<8>(result).count() % 2 == 0; // should inline to popcount intrinsic on x86
		setFlag(flag::P, is_even_parity);
		_registers.a= result;
		_registers.pc += 1;
	}

	void cpl()
	{
		_registers.a= ~_registers.a;
		setFlag(flag::H, true);
		setFlag(flag::N, true);
		_registers.pc += 2;
	}


};

