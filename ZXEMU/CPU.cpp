#include "CPU.h"

CPU::CPU(unsigned __int8 *pram)
{
	ram = pram;
	PC = SP = IX = IY = AF = BC = DE = HL = 0x0000;
}

CPU::~CPU()
{
}

void CPU::LD16(unsigned __int16& reg, unsigned __int16 val)
{
	reg = val;
}

void CPU::LD8LO(unsigned __int16& reg, unsigned __int8 val)
{
	reg = (reg & 0xFF00) + val;
}

void CPU::LD8HI(unsigned __int16& reg, unsigned __int8 val)
{
	reg = (val << 8) | LOBYTE(reg);
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

void CPU::Step()
{
	unsigned __int8 prefix = 0;
	auto opcode = ram[PC++];
	if (opcode == 0xDD || opcode == 0xFD || opcode == 0xED)
	{
		prefix = opcode;
		opcode = ram[PC++];
	}

	if (opcode >= 0x00 && opcode <= 0x0F) Processing_00_0F(opcode, prefix);
	else if (opcode >= 0x10 && opcode <= 0x1F) Processing_10_1F(opcode, prefix);
	else if (opcode >= 0x20 && opcode <= 0x2F) Processing_20_2F(opcode, prefix);
	else if (opcode >= 0x30 && opcode <= 0x3F) Processing_30_3F(opcode, prefix);
	else if (opcode >= 0x40 && opcode <= 0x4F) Processing_40_4F(opcode, prefix);
	else if (opcode >= 0x50 && opcode <= 0x5F) Processing_50_5F(opcode, prefix);
	else if (opcode >= 0x60 && opcode <= 0x6F) Processing_60_6F(opcode, prefix);
	else if (opcode >= 0x70 && opcode <= 0x7F) Processing_70_7F(opcode, prefix);
	else if (opcode >= 0x80 && opcode <= 0x8F) Processing_80_8F(opcode, prefix);
	else if (opcode >= 0x90 && opcode <= 0x9F) Processing_90_9F(opcode, prefix);
	else if (opcode >= 0xA0 && opcode <= 0xAF) Processing_A0_AF(opcode, prefix);
	else if (opcode >= 0xB0 && opcode <= 0xBF) Processing_B0_BF(opcode, prefix);
	else if (opcode >= 0xC0 && opcode <= 0xCF) Processing_C0_CF(opcode, prefix);
	else if (opcode >= 0xD0 && opcode <= 0xDF) Processing_D0_DF(opcode, prefix);
	else if (opcode >= 0xE0 && opcode <= 0xEF) Processing_E0_EF(opcode, prefix);
	else if (opcode >= 0xF0 && opcode <= 0xFF) Processing_F0_FF(opcode, prefix);
}

void CPU::Processing_00_0F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x00:
		break;
	case 0x01:		// LD BC,NN 01 NN NN
		LD16(BC, ram[PC + 1] * 256 + ram[PC]);
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
		LD8HI(BC, ram[PC]);
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
		LD8HI(AF, ram[BC]);
		break;
	case 0x0B:
		break;
	case 0x0C:
		break;
	case 0x0D:
		break;
	case 0x0E:		// LD C,N	0E NN
		LD8LO(BC, ram[PC]);
		PC += 1;
		break;
	case 0x0F:
		break;
	default:
		break;
	}
}

void CPU::Processing_10_1F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x10:
		break;
	case 0x11:		// LD DE,NN		11 NN NN
		LD16(DE, ram[PC + 1] * 256 + ram[PC]);
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
	case 0x16:		// LD D,N	16 NN
		LD8HI(DE, ram[PC]);
		PC += 1;
		break;
	case 0x17:
		break;
	case 0x18:
		break;
	case 0x19:
		break;
	case 0x1A:		// LD A,(DE)	1A
		LD8HI(AF, ram[DE]);
		break;
	case 0x1B:
		break;
	case 0x1C:
		break;
	case 0x1D:
		break;
	case 0x1E:		// LD E,N	1E NN
		LD8LO(DE, ram[PC]);
		PC += 1;
		break;
	case 0x1F:
		break;
	default:
		break;
	}
}

void CPU::Processing_20_2F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x20:
		break;
	case 0x21:
		if (prefix == 0xDD)
			LD16(IX, ram[PC + 1] * 256 + ram[PC]);		// LD IX, NN	DD 21 NN NN
		else if(prefix == 0xFD)
			LD16(IY, ram[PC + 1] * 256 + ram[PC]);		// LD IY, NN	FD 21 NN NN
		else
			LD16(HL, ram[PC + 1] * 256 + ram[PC]);		// LD HL, NN	21 NN NN
		PC += 2;
		break;
	case 0x22:
		break;
	case 0x23:
		break;
	case 0x24:
		break;
	case 0x25:
		break;
	case 0x26:
		if(prefix == 0xDD)
			LD8HI(IX, ram[PC]);		// LD XH,N	DD 26 NN
		else if(prefix == 0xFD)
			LD8HI(IY, ram[PC]);		// LD YH,N	FD 26 NN
		else
			LD8HI(HL, ram[PC]);		// LD H,N	26 NN
		PC += 1;
		break;
	case 0x27:
		break;
	case 0x28:
		break;
	case 0x29:
		break;
	case 0x2A:
		if (prefix == 0xDD)
		{
			LD8LO(IX, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD IX,(NN)	DD 2A NN NN
			LD8HI(IX, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		else if(prefix == 0xFD)
		{
			LD8LO(IY, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD IY,(NN)	FD 2A NN NN
			LD8HI(IY, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		else
		{
			LD8LO(HL, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD HL,(NN)	2A NN NN
			LD8HI(HL, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		break;
	case 0x2B:
		break;
	case 0x2C:
		break;
	case 0x2D:
		break;
	case 0x2E:
		if (prefix == 0xDD)
			LD8LO(IX, ram[PC]);		// LD XL,N	DD 2E NN
		else if (prefix == 0xFD)
			LD8LO(IY, ram[PC]);		// LD YL,N	FD 2E NN
		else
			LD8LO(HL, ram[PC]);		// LD L,N	2E NN
		PC += 1;
		break;
	case 0x2F:
		break;
	default:
		break;
	}
}

void CPU::Processing_30_3F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x30:
		break;
	case 0x31:		// LD SP,NN	31 NN NN
		LD16(SP, ram[PC + 1] * 256 + ram[PC]);
		PC += 2;
		break;
	case 0x32:
		break;
	case 0x33:
		break;
	case 0x34:
		break;
	case 0x35:
		break;
	case 0x36:
		break;
	case 0x37:
		break;
	case 0x38:
		break;
	case 0x39:
		break;
	case 0x3A:		// LD A,(NN)	3A NN NN
		LD8HI(AF, ram[ram[PC + 1] * 256 + ram[PC]]);
		PC += 2;
		break;
	case 0x3B:
		break;
	case 0x3C:
		break;
	case 0x3D:
		break;
	case 0x3E:		// LD A,N	3E NN
		LD8HI(AF, ram[PC]);
		PC += 1;
		break;
	case 0x3F:
		break;
	default:
		break;
	}
}

void CPU::Processing_40_4F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x40:		// LD B,B	40
		break;
	case 0x41:		// LD B,C	41
		LD8HI(BC, LOBYTE(BC));
		break;
	case 0x42:		// LD B,D	42
		LD8HI(BC, HIBYTE(DE));
		break;
	case 0x43:		// LD B,E	43
		LD8HI(BC, LOBYTE(DE));
		break;
	case 0x44:
		if (prefix == 0xDD)
			LD8HI(BC, HIBYTE(IX));		// LD B,XH	DD 44
		else if(prefix == 0xFD)
			LD8HI(BC, HIBYTE(IY));		// LD B,YH	FD 44
		else
			LD8HI(BC, HIBYTE(HL));		// LD B,H	44
		break;
	case 0x45:
		if (prefix == 0xDD)
			LD8HI(BC, LOBYTE(IX));		// LD B,XL	DD 45
		else if(prefix == 0xFD)
			LD8HI(BC, LOBYTE(IY));		// LD B,YL	FD 45
		else
			LD8HI(BC, LOBYTE(HL));		// LD B,L	45
		break;
	case 0x46:
		if (prefix == 0xDD)
			LD8HI(BC, ram[IX + ram[PC++]]);		// LD B,(IX+s)	DD 46 ss
		else if (prefix == 0xFD)
			LD8HI(BC, ram[IY + ram[PC++]]);		// LD B,(IY+s)	FD 46 ss
		else
			LD8HI(BC, ram[HL]);					// LD B,(HL)	46
		break;
	case 0x47:
		if (prefix == 0xED)
			LD8HI(IR, HIBYTE(AF));		// LD I,A	ED 47
		else
			LD8HI(BC, HIBYTE(AF));		// LD B,A	47
		break;
	case 0x48:		// LD C,B	48
		LD8LO(BC, HIBYTE(BC));
		break;
	case 0x49:
		break;
	case 0x4A:		// LD C,D	4A
		LD8LO(BC, HIBYTE(DE));
		break;
	case 0x4B:
		if (prefix == 0xED)
		{
			LD8LO(BC, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD BC,(NN)	ED 4B NN NN
			LD8HI(BC, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		else
			LD8LO(BC, LOBYTE(DE));		// LD C,E	4B
		break;
	case 0x4C:
		if(prefix == 0xDD)
			LD8LO(BC, HIBYTE(IX));		// LD C,XH	DD 4C
		else if(prefix == 0xFD)
			LD8LO(BC, HIBYTE(IY));		// LD C,YH	FD 4C
		else
			LD8LO(BC, HIBYTE(HL));		// LD C,H	4C
		break;
	case 0x4D:
		if (prefix == 0xDD)
			LD8LO(BC, LOBYTE(IX));		// LD C,XL	DD 4D
		else if (prefix == 0xFD)
			LD8LO(BC, LOBYTE(IY));		// LD C,YL	FD 4D
		else
			LD8LO(BC, LOBYTE(HL));		// LD C,L	4D
		break;
	case 0x4E:
		if (prefix == 0xDD)
			LD8LO(BC, ram[IX + ram[PC++]]);		// LD C, (IX + s)	DD 4E ss
		else if (prefix == 0xFD)
			LD8LO(BC, ram[IY + ram[PC++]]);		// LD C,(IY+s)	FD 4E ss
		else
			LD8LO(BC, ram[HL]);					// LD C,(HL)	4E
		break;
	case 0x4F:
		if (prefix == 0xED)
			LD8LO(IR, HIBYTE(AF));		// LD R,A	ED 4F
		else
			LD8LO(BC, HIBYTE(AF));		// LD C,A	4F
		break;
	default:
		break;
	}
}

void CPU::Processing_50_5F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x50:		// LD D,B	50
		LD8HI(DE, HIBYTE(BC));
		break;
	case 0x51:		// LD D,C	51
		LD8HI(DE, LOBYTE(BC));
		break;
	case 0x52:		// LD D,D	52
		break;
	case 0x53:		// LD D,E	53
		LD8HI(DE, LOBYTE(DE));
		break;
	case 0x54:
		if(prefix == 0xDD)
			LD8HI(DE, HIBYTE(IX));		// LD D,XH	DD 54
		else if(prefix == 0xFD)
			LD8HI(DE, HIBYTE(IY));		// LD D,YH	FD 54
		else
			LD8HI(DE, HIBYTE(HL));		// LD D,H	54
		break;
	case 0x55:
		if (prefix == 0xDD)
			LD8HI(DE, LOBYTE(IX));		// LD D,XL	DD 55
		else if(prefix == 0xFD)
			LD8HI(DE, LOBYTE(IY));		// LD D, YL	FD 55
		else
			LD8HI(DE, LOBYTE(HL));		// LD D,L	55
		break;
	case 0x56:
		if (prefix == 0xDD)
			LD8HI(DE, ram[IX + ram[PC++]]);		// LD D,(IX+s)	DD 56 ss
		else if (prefix == 0xFD)
			LD8HI(DE, ram[IY + ram[PC++]]);		// LD D,(IY+s)	FD 56 ss
		else
			LD8HI(DE, ram[HL]);					// LD D,(HL)	56
		break;
	case 0x57:
		if (prefix == 0xED)
			LD8HI(AF, HIBYTE(IR));		// LD A,I	ED 57
		else
			LD8HI(DE, HIBYTE(AF));		// LD D,A	57
		break;
	case 0x58:
		LD8LO(DE, HIBYTE(BC));		// LD E,B	58
		break;
	case 0x59:
		LD8LO(DE, LOBYTE(BC));		// LD E,C	59
		break;
	case 0x5A:
		LD8LO(DE, HIBYTE(DE));		// LD E,D	5A
		break;
	case 0x5B:
		if (prefix == 0xED)
		{
			LD8LO(DE, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD DE,(NN)	ED 5B NN NN
			LD8HI(DE, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		// LD E,E	5B
		break;
	case 0x5C:
		if(prefix == 0xDD)
			LD8LO(DE, HIBYTE(IX));		// LD E,XH	DD 5C
		else if(prefix == 0xFD)
			LD8LO(DE, HIBYTE(IY));		// LD E,YH	FD 5C
		else
			LD8LO(DE, HIBYTE(HL));		// LD E,H	5C
		break;
	case 0x5D:
		if (prefix == 0xDD)
			LD8LO(DE, LOBYTE(IX));		// LD E,XL	DD 5D
		else if (prefix == 0xFD)
			LD8LO(DE, LOBYTE(IY));		// LD E,YL	FD 5D
		else
			LD8LO(DE, LOBYTE(HL));		// LD E,L	5D
		break;
	case 0x5E:		// LD E,(HL)	5E
		LD8LO(DE, ram[HL]);
		break;
	case 0x5F:
		if (prefix == 0xED)
			LD8HI(AF, LOBYTE(IR));		// LD A,R	ED 5F
		else
			LD8LO(DE, HIBYTE(AF));		// LD E,A	5F
		break;
	default:
		break;
	}
}

void CPU::Processing_60_6F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x60:
		if (prefix == 0xDD)
			LD8HI(IX, HIBYTE(BC));		// LD XH,B	DD 60
		else if (prefix == 0xFD)
			LD8HI(IY, HIBYTE(BC));		// LD YH,B	FD 60
		else
			LD8HI(HL, HIBYTE(BC));		// LD H,B	60
		break;
	case 0x61:
		if (prefix == 0xDD)
			LD8HI(IX, LOBYTE(BC));		// LD XH,C	DD 61
		else if (prefix == 0xFD)
			LD8HI(IY, LOBYTE(BC));		// LD YH,C	FD 61
		else
			LD8HI(HL, LOBYTE(BC));		// LD H,C	61
		break;
	case 0x62:
		if (prefix == 0xDD)
			LD8HI(IX, HIBYTE(DE));		// LD XH,D	DD 62
		else if (prefix == 0xFD)
			LD8HI(IY, HIBYTE(DE));		// LD YH,D	FD 62
		else
			LD8HI(HL, HIBYTE(DE));		// LD H,D	62
		break;
	case 0x63:
		if (prefix == 0xDD)
			LD8HI(IX, LOBYTE(DE));		// LD XH,E	DD 63
		else if (prefix == 0xFD)
			LD8HI(IY, LOBYTE(DE));		// LD YH,E	FD 63
		else
			LD8HI(HL, LOBYTE(DE));		// LD H,E	63
		break;
	case 0x64:
		break;
	case 0x65:
		if (prefix == 0xDD)
			LD8HI(IX, LOBYTE(IX));		// LD XH,XL	DD 65
		else if (prefix == 0xFD)
			LD8HI(IY, LOBYTE(IY));		// LD YH,YL	FD 65
		else
			LD8HI(HL, LOBYTE(HL));		// LD H,L	65
		break;
	case 0x66:		// LD H,(HL)	66
		LD8HI(HL, ram[HL]);
		break;
	case 0x67:
		if (prefix == 0xDD)
			LD8HI(IX, HIBYTE(AF));		// LD XH,A	DD 67
		else if (prefix == 0xFD)
			LD8HI(IY, HIBYTE(AF));		// LD YH,A	FD 67
		else
			LD8HI(HL, HIBYTE(AF));		// LD H,A	67
		break;
	case 0x68:
		if (prefix == 0xDD)
			LD8LO(IX, HIBYTE(BC));		// LD XL,B	DD 68
		else if (prefix == 0xFD)
			LD8LO(IY, HIBYTE(BC));		// LD YL,B	FD 68
		else
			LD8LO(HL, HIBYTE(BC));		// LD L,B	68
		break;
	case 0x69:
		if (prefix == 0xDD)
			LD8LO(IX, LOBYTE(BC));		// LD XL,C	DD 69
		else if (prefix == 0xFD)
			LD8LO(IY, LOBYTE(BC));		// LD YL,C	FD 69
		else
			LD8LO(HL, LOBYTE(BC));		// LD L,C	69
		break;
	case 0x6A:
		if (prefix == 0xDD)
			LD8LO(IX, HIBYTE(DE));		// LD XL,D	DD 6A
		else if (prefix == 0xFD)
			LD8LO(IY, HIBYTE(DE));		// LD YL,D	FD 6A
		else
			LD8LO(HL, HIBYTE(DE));		// LD L,D	6A
		break;
	case 0x6B:
		if (prefix == 0xED)
		{
			LD8LO(HL, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD HL,(NN)	ED 6B NN NN
			LD8HI(HL, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		else if (prefix == 0xDD)
			LD8LO(IX, LOBYTE(DE));		// LD XL,E	DD 6B
		else if (prefix == 0xFD)
			LD8LO(IY, LOBYTE(DE));		// LD YL,E	FD 6B
		else
			LD8LO(HL, LOBYTE(DE));		// LD L,E	6B
		break;
	case 0x6C:
		if (prefix == 0xDD)
			LD8LO(IX, HIBYTE(IX));		// LD XL,XH	DD 6C
		else if (prefix == 0xFD)
			LD8LO(IY, HIBYTE(IY));		// LD YL,YH	FD 6C
		else
			LD8LO(HL, HIBYTE(HL));		// LD L,H	6C
		break;
	case 0x6D:
		break;
	case 0x6E:		// LD L,(HL)	6E
		LD8LO(HL, ram[HL]);
		break;
	case 0x6F:
		if (prefix == 0xDD)
			LD8LO(IX, HIBYTE(AF));		// LD XL,A	DD 6F
		else if (prefix == 0xFD)
			LD8LO(IY, HIBYTE(AF));		// LD YL,A	FD 6F
		else
			LD8LO(HL, HIBYTE(AF));		// LD L,A	6F
		break;
	default:
		break;
	}
}

void CPU::Processing_70_7F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x70:
		break;
	case 0x71:
		break;
	case 0x72:
		break;
	case 0x73:
		break;
	case 0x74:
		break;
	case 0x75:
		break;
	case 0x76:
		break;
	case 0x77:
		break;
	case 0x78:		// LD A,B	78
		LD8HI(AF, HIBYTE(BC));
		break;
	case 0x79:		// LD A,C	79
		LD8HI(AF, LOBYTE(BC));
		break;
	case 0x7A:		// LD A,D	7A
		LD8HI(AF, HIBYTE(DE));
		break;
	case 0x7B:
		if (prefix == 0xED)
		{
			LD8LO(SP, ram[ram[PC + 1] * 256 + ram[PC] + 0]);		// LD SP,(NN)	ED 7B NN NN
			LD8HI(SP, ram[ram[PC + 1] * 256 + ram[PC] + 1]);
			PC += 2;
		}
		else
			LD8HI(AF, LOBYTE(DE));		// LD A,E	7B
		break;
	case 0x7C:
		if(prefix == 0xDD)
			LD8HI(AF, HIBYTE(IX));		// LD A,XH	DD 7C
		else if(prefix == 0xFD)
			LD8HI(AF, HIBYTE(IY));		// LD A,YH	FD 7C
		else
			LD8HI(AF, HIBYTE(HL));		// LD A,H	7C
		break;
	case 0x7D:
		if (prefix == 0xDD)
			LD8HI(AF, LOBYTE(IX));		// LD A,XL	DD 7D
		else if (prefix == 0xFD)
			LD8HI(AF, LOBYTE(IY));		// LD A,YL	FD 7D
		else
			LD8HI(AF, LOBYTE(HL));		// LD A,L	7D
		break;
	case 0x7E:
		if (prefix == 0xDD)
			LD8HI(AF, ram[IX + ram[PC++]]);		// LD A,(IX+s)	DD 7E ss
		else if (prefix == 0xFD)
			LD8HI(AF, ram[IY + ram[PC++]]);		// LD A,(IY+s)	FD 7E ss
		else
			LD8HI(AF, ram[HL]);					// LD A,(HL)	7E
		break;
	case 0x7F:
		break;
	default:
		break;
	}
}

void CPU::Processing_80_8F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x80:
		break;
	case 0x81:
		break;
	case 0x82:
		break;
	case 0x83:
		break;
	case 0x84:
		break;
	case 0x85:
		break;
	case 0x86:
		break;
	case 0x87:
		break;
	case 0x88:
		break;
	case 0x89:
		break;
	case 0x8A:
		break;
	case 0x8B:
		break;
	case 0x8C:
		break;
	case 0x8D:
		break;
	case 0x8E:
		break;
	case 0x8F:
		break;
	default:
		break;
	}
}

void CPU::Processing_90_9F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x90:
		break;
	case 0x91:
		break;
	case 0x92:
		break;
	case 0x93:
		break;
	case 0x94:
		break;
	case 0x95:
		break;
	case 0x96:
		break;
	case 0x97:
		break;
	case 0x98:
		break;
	case 0x99:
		break;
	case 0x9A:
		break;
	case 0x9B:
		break;
	case 0x9C:
		break;
	case 0x9D:
		break;
	case 0x9E:
		break;
	case 0x9F:
		break;
	default:
		break;
	}
}

void CPU::Processing_A0_AF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xA0:
		break;
	case 0xA1:
		break;
	case 0xA2:
		break;
	case 0xA3:
		break;
	case 0xA4:
		break;
	case 0xA5:
		break;
	case 0xA6:
		break;
	case 0xA7:
		break;
	case 0xA8:
		break;
	case 0xA9:
		break;
	case 0xAA:
		break;
	case 0xAB:
		break;
	case 0xAC:
		break;
	case 0xAD:
		break;
	case 0xAE:
		break;
	case 0xAF:
		break;
	default:
		break;
	}
}

void CPU::Processing_B0_BF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xB0:
		break;
	case 0xB1:
		break;
	case 0xB2:
		break;
	case 0xB3:
		break;
	case 0xB4:
		break;
	case 0xB5:
		break;
	case 0xB6:
		break;
	case 0xB7:
		break;
	case 0xB8:
		break;
	case 0xB9:
		break;
	case 0xBA:
		break;
	case 0xBB:
		break;
	case 0xBC:
		break;
	case 0xBD:
		break;
	case 0xBE:
		break;
	case 0xBF:
		break;
	default:
		break;
	}
}

void CPU::Processing_C0_CF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xC0:
		break;
	case 0xC1:
		break;
	case 0xC2:
		break;
	case 0xC3:
		break;
	case 0xC4:
		break;
	case 0xC5:
		break;
	case 0xC6:
		break;
	case 0xC7:
		break;
	case 0xC8:
		break;
	case 0xC9:
		break;
	case 0xCA:
		break;
	case 0xCB:
		break;
	case 0xCC:
		break;
	case 0xCD:
		break;
	case 0xCE:
		break;
	case 0xCF:
		break;
	default:
		break;
	}
}

void CPU::Processing_D0_DF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xD0:
		break;
	case 0xD1:
		break;
	case 0xD2:
		break;
	case 0xD3:
		break;
	case 0xD4:
		break;
	case 0xD5:
		break;
	case 0xD6:
		break;
	case 0xD7:
		break;
	case 0xD8:
		break;
	case 0xD9:		// EXX D9
		EXX();
		break;
	case 0xDA:
		break;
	case 0xDB:
		break;
	case 0xDC:
		break;
	case 0xDD:
		break;
	case 0xDE:
		break;
	case 0xDF:
		break;
	default:
		break;
	}
}

void CPU::Processing_E0_EF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xE0:
		break;
	case 0xE1:
		break;
	case 0xE2:
		break;
	case 0xE3:
		break;
	case 0xE4:
		break;
	case 0xE5:
		break;
	case 0xE6:
		break;
	case 0xE7:
		break;
	case 0xE8:
		break;
	case 0xE9:
		break;
	case 0xEA:
		break;
	case 0xEB:		// EX DE,HL
		EX16(DE, HL);
		break;
	case 0xEC:
		break;
	case 0xED:
		break;
	case 0xEE:
		break;
	case 0xEF:
		break;
	default:
		break;
	}
}

void CPU::Processing_F0_FF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xF0:
		break;
	case 0xF1:
		break;
	case 0xF2:
		break;
	case 0xF3:
		break;
	case 0xF4:
		break;
	case 0xF5:
		break;
	case 0xF6:
		break;
	case 0xF7:
		break;
	case 0xF8:
		break;
	case 0xF9:
		if (prefix == 0xDD)
			LD16(SP, IX);		// LD SP,HL	F9
		else if (prefix == 0xFD)
			LD16(SP, IY);		// LD SP,IX	DD F9
		else
			LD16(SP, HL);		// LD SP,IY	FD F9
		break;
	case 0xFA:
		break;
	case 0xFB:
		break;
	case 0xFC:
		break;
	case 0xFD:
		break;
	case 0xFE:
		break;
	case 0xFF:
		break;
	default:
		break;
	}
}
