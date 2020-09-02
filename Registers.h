#pragma once

// Represents static flags
const bool set = true;
const bool reset = false;

struct Registers
{
    uint16_t pc, sp, ix, iy; // special purpose registers
    uint16_t mem_ptr; // "wz" register
    uint8_t a, b, c, d, e, h, l; // main registers
	uint16_t f;
    uint8_t i, r; // interrupt vector, memory refresh
};
namespace flag {
	// Represents static flags
	const bool set = true;
	const bool reset = false;

	enum StatusFlag : int {
		C = 0,
		N = 1,
		P = 2,
		X = 3,
		H = 4,
		Y = 5,
		Z = 6,
		S = 7
	};
}