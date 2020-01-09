#include "CPU.h"

CPU::CPU(unsigned __int8 *pram)
{
	ram = pram;
	PC = SP = IX = IY = AF = BC = DE = HL = 0x0000;
}

CPU::~CPU()
{
}

void CPU::Step()
{
	auto op = ram[PC++];

	switch (op)
	{
	case 0x00:
		break;
	case 0x01:		// LD BC,NN 01 NN NN
		LD16(BC, (ram[PC + 1] << 8) + ram[PC]);
		PC += 2;
		break;
	case 0x02:
		break;
	case 0x03:
		break;
	case 0x04:
		break;
	case 0x05:
		break;
	case 0x06:		// LD B,N 06 NN
		LD16(BC, (ram[PC] << 8) | LOBYTE(BC));
		PC += 1;
		break;
	case 0x07:
		break;
	case 0x08:		// EX AF, A'F' 08
		EX16(AF, _AF);
		break;
	case 0x09:
		break;
	case 0x0A:		// LD A,(BC)
		LD16(AF, HIBYTE(AF) | ram[BC]);
		break;
	case 0x0B:
		break;
	case 0x0C:
		break;
	case 0x0D:
		break;
	case 0x0E:
		break;
	case 0x0F:
		break;
	case 0xD9:		// EXX D9
		EXX();
		break;
	case 0xEB:		// EX DE,HL
		EX16(DE, HL);
		break;
	default:
		break;
	}
}

void CPU::LD16(unsigned __int16& reg, unsigned __int16 val)
{
	reg = val;
}

void CPU::LD8LO(unsigned __int16& reg, unsigned __int8 val)
{
}

void CPU::LD8HI(unsigned __int16& reg, unsigned __int8 val)
{
}

void CPU::EX16(unsigned __int16& reg1, unsigned __int16& reg2)
{
	auto tmp = reg1;
	reg1 = reg2;
	reg2 = tmp;
}

void CPU::EXX()
{
	EX16(AF, _AF);
	EX16(BC, _BC);
	EX16(DE, _DE);
	EX16(HL, _HL);
}
