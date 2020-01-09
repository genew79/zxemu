#include "Registers.h"

Registers::Registers()
{
	PC = SP = IX = IY = AF = BC = DE = HL = 0x0000;
}

Registers::~Registers()
{
}

void Registers::LD16(unsigned __int16& reg, unsigned __int16 val)
{
	reg = val;
}

void Registers::LD8LO(unsigned __int16& reg, unsigned __int8 val)
{
}

void Registers::LD8HI(unsigned __int16& reg, unsigned __int8 val)
{
}

void Registers::EX16(unsigned __int16& reg1, unsigned __int16& reg2)
{
	auto tmp = reg1;
	reg1 = reg2;
	reg2 = tmp;
}

void Registers::EXX()
{
	EX16(AF, _AF);
	EX16(BC, _BC);
	EX16(DE, _DE);
	EX16(HL, _HL);
}
