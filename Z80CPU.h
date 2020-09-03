#pragma once
#include <bitset>
#include "Registers.h"
#include "Ram.h"


class Z80CPU
{
private :
	Ram &ram; // refrance to Ram

protected:
	Registers _registers;
	Z80CPU(Ram &ram);

	void nop();
	// set a flag to a given value (true or false)
	void setFlag(const flag::StatusFlag statusFlag, bool value);
	void add_a_r(uint8_t reg);
	void add_hl(uint8_t reg);
	void inc_r(uint8_t reg);
	void inc_bc();
	uint8_t inc(const uint8_t value);
	void dec_r(uint8_t reg);
	void dec_bc();
	void inc_hl();
	void dec_hl();
	uint8_t  dec(const uint8_t value);
	void ld(char which);
	void ld_e();
	void ld_de(uint8_t value);
	void ld_de();
	void ld(const uint16_t addressReg, uint8_t src);
	void ld_acc_address(uint16_t addressReg);
	void ld_nn_hl();
	void ld_hl_nn();
	void rlca();
	void rrca();
	void rra();
	void rla();

	// Jump group
	void djnz();
	void jr();
	void jr(const bool jump_condition);
	uint16_t  nextw();
	void jump();
	uint16_t get_hl();
	uint16_t get_de();
	uint16_t get_bc();
	void daa();
	void cpl();
};

