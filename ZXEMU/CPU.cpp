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
	auto opcode = ram[PC++];

	if (opcode >= 0x00 && opcode <= 0x0F) Processing_00_0F(opcode);
	else if (opcode >= 0x10 && opcode <= 0x1F) Processing_10_1F(opcode);
/*
	case 0xD9:		// EXX D9
		EXX();
		break;
	case 0xEB:		// EX DE,HL
		EX16(DE, HL);
		break;
	default:
		break;
	}
*/
}

void CPU::Processing_00_0F(unsigned __int8 opcode)
{
	switch (opcode)
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
	default:
		break;
	}
}

void CPU::Processing_10_1F(unsigned __int8 opcode)
{
	switch (opcode)
	{
	case 0x10:
		break;
	case 0x11:		// LD DE,NN		11 NN NN
		LD16(DE, (ram[PC + 1] << 8) + ram[PC]);
		PC += 2;
		break;
	case 0x12:
		break;
	case 0x13:
		break;
	case 0x14:
		break;
	case 0x15:
		break;
	case 0x16:
		break;
	case 0x17:
		break;
	case 0x18:
		break;
	case 0x19:
		break;
	case 0x1A:
		break;
	case 0x1B:
		break;
	case 0x1C:
		break;
	case 0x1D:
		break;
	case 0x1E:
		break;
	case 0x1F:
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
