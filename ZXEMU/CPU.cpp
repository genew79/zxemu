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

void CPU::PUSH(unsigned __int16& reg)
{
	SP = SP - 2;
	MEM16(ram[SP]) = reg;
}

void CPU::POP(unsigned __int16& reg)
{
	reg = MEM16(ram[SP]);
	SP = SP + 2;
}

void CPU::RST(unsigned __int16 addr)
{
	SP = SP - 2;
	MEM16(ram[SP]) = PC;
	PC = addr;
}

void CPU::CALL(unsigned __int16 addr)
{
	SP = SP - 2;
	MEM16(ram[SP]) = PC + 2;
	PC = addr;
}

unsigned __int8 CPU::Add8(unsigned __int8 reg1, unsigned __int8 reg2, bool withoutCF)
{
	unsigned __int16 res = reg1 + reg2;
	auto flags = LOBYTE(AF);
	flags = flags & ~(SF | ZF | NF | F5 | F3 | PF | HF);
	flags |= LOBYTE(res) & SF;		// SF
	flags |= (LOBYTE(res) == 0 ? ZF : 0);		// ZF
	flags |= ((((reg1 & 0x0F) + (reg2 & 0x0F)) & 0x10) ? HF : 0);		// HF
	flags |= (((LOBYTE(res) ^ reg1) & 0b10000000) != 0 ? PF : 0);		// PF
	if (!withoutCF)
	{
		flags &= ~CF;
		flags |= (res > 255 ? CF : 0);		// CF
	}
	flags |= (LOBYTE(res) & (F5 | F3));
	AF = 256 * HIBYTE(AF) + flags;
	return LOBYTE(res);
}

unsigned __int8 CPU::ADC8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto flags = LOBYTE(AF);
	return Add8(reg1, reg2 + (flags & CF ? 1 : 0));
}

unsigned __int16 CPU::ADD16(unsigned __int16 reg1, unsigned __int16 reg2)
{
	unsigned __int32 res = reg1 + reg2;
	auto flags = LOBYTE(AF);
	flags = flags & ~(NF | CF | F5 | F3 | HF);
	flags |= (((reg1 & 0x0FFF) + (reg2 & 0x0FFF)) >> 8) & 0x10;		// HF
	flags |= (res > 0xFFFF ? CF : 0);		// CF
	flags |= (HIBYTE(res) & (F5 | F3));
	AF = 256 * HIBYTE(AF) + flags;
	return (unsigned __int16)res;
}

unsigned __int16 CPU::ADC16(unsigned __int16 reg1, unsigned __int16 reg2)
{
	auto flags = LOBYTE(AF);
	unsigned __int32 res = reg1 + reg2 + (flags & CF ? 1 : 0);
	flags = flags & ~(SF | ZF | NF | CF | F5 | F3 | HF | PF);
	flags |= (res > 0xFFFF ? CF : 0);		// CF
	flags |= (LOBYTE(res) == 0 ? ZF : 0);		// ZF
	flags |= (((reg1 & 0x0FFF) + (reg2 & 0x0FFF) + (flags & CF ? 1 : 0)) >> 8) & 0x10;		// HF
	flags |= ((((unsigned __int16)res ^ reg1) & 0x8000) != 0 ? PF : 0);		// PF
	flags |= (HIBYTE(res) & (SF | F5 | F3));
	AF = 256 * HIBYTE(AF) + flags;
	return LOWORD(res);
}

unsigned __int8 CPU::SUB8(unsigned __int8 reg1, unsigned __int8 reg2, bool withoutCF)
{
	unsigned __int16 res = reg1 - reg2;
	auto flags = LOBYTE(AF);
	flags = flags & ~(SF | ZF | NF | F5 | F3 | PF | HF);
	flags |= LOBYTE(res) & SF;		// SF
	flags |= (LOBYTE(res) == 0 ? ZF : 0);		// ZF
	flags |= ((((reg1 & 0x0F) - (reg2 & 0x0F)) & 0x10) ? HF : 0);		// HF
	flags |= (((LOBYTE(res) ^ reg1) & SF) != 0 ? PF : 0);		// PF
	if (!withoutCF)
	{
		flags &= ~CF;
		flags |= (res > 255 ? CF : 0);		// CF
	}
	flags |= (LOBYTE(res) & (F5 | F3));
	flags |= NF;
	AF = 256 * HIBYTE(AF) + flags;
	return LOBYTE(res);
}

unsigned __int8 CPU::SBC8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto flags = LOBYTE(AF);
	return SUB8(reg1, reg2 + (flags & CF ? 1 : 0));
}

unsigned __int16 CPU::SBC16(unsigned __int16 reg1, unsigned __int16 reg2)
{
	auto flags = LOBYTE(AF);
	unsigned __int32 res = reg1 - reg2 - (flags & CF ? 1 : 0);
	flags = flags & ~(SF | ZF | CF | F5 | F3 | HF | PF);
	flags |= (((reg1 & 0x0FFF) - (reg2 & 0x0FFF) - (flags & CF ? 1 : 0)) >> 8) & 0x10;		// HF
	flags |= (res > 0xFFFF ? CF : 0);		// CF
	flags |= (LOWORD(res) == 0 ? ZF : 0);		// ZF
	flags |= (((LOWORD(res) ^ reg1) & 0x8000) != 0 ? PF : 0);		// PF
	flags |= (HIBYTE(res) & (SF | F5 | F3));
	flags |= NF;
	AF = 256 * HIBYTE(AF) + flags;
	return (unsigned __int16)res;
}

void CPU::CP8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto res = SUB8(reg1, reg2);
	auto flags = LOBYTE(AF) & ~(F5 | F3) | (res & (F3 | F5));
	AF = 256 * HIBYTE(AF) + flags;
}

unsigned __int8 CPU::AND8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto flags = PF;
	auto res = reg1 & reg2;
	flags |= (res == 0 ? ZF : 0);		// ZF
	for (int i = 0x80; i > 0; i /= 2) if (res & i) flags ^= PF;		// PF
	flags |= (res & (SF | F3 | F5));
	flags |= HF;
	AF = 256 * HIBYTE(AF) + flags;
	return res;
}

unsigned __int8 CPU::OR8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto flags = PF;
	auto res = reg1 | reg2;
	flags |= (res == 0 ? ZF : 0);		// ZF
	for (int i = 0x80; i > 0; i /= 2) if (res & i) flags ^= PF;		// PF
	flags |= (res & (SF | F3 | F5));
	AF = 256 * HIBYTE(AF) + flags;
	return res;
}

unsigned __int8 CPU::XOR8(unsigned __int8 reg1, unsigned __int8 reg2)
{
	auto flags = PF;
	auto res = reg1 ^ reg2;
	flags |= (res == 0 ? ZF : 0);		// ZF
	for (int i = 0x80; i > 0; i /= 2) if (res & i) flags ^= PF;		// PF
	flags |= (res & (SF | F3 | F5));
	AF = 256 * HIBYTE(AF) + flags;
	return res;
}

unsigned __int8 CPU::SRL8(unsigned __int8 reg1)
{
	auto flags = PF;
	auto res = reg1 >> 1;
	flags |= (res == 0 ? ZF : 0);		// ZF
	for (int i = 0x80; i > 0; i /= 2) if (res & i) flags ^= PF;		// PF
	flags |= (reg1 & CF);
	flags |= (res & (SF | F3 | F5));
	AF = 256 * HIBYTE(AF) + flags;
	return res;
}

unsigned __int8 CPU::SRA8(unsigned __int8 reg1)
{
	auto flags = PF;
	auto res = reg1 >> 1;
	flags |= (reg1 & CF);
	flags |= (res == 0 ? ZF : 0);		// ZF
	flags |= (res & (SF | F3 | F5));
	AF = 256 * HIBYTE(AF) + flags;
	return res;
}


void CPU::Stop(unsigned __int8 opcode)
{
//	throw opcode;
}

void CPU::Step()
{
	unsigned __int8 prefix = 0;
	auto opcode = ram[PC++];
	if (opcode == 0xDD || opcode == 0xFD || opcode == 0xED || opcode == 0xCB)
	{
		prefix = opcode;
		opcode = ram[PC++];
	}

	if((prefix == 0xDD || prefix == 0xFD) && opcode == 0xCB) Processing_DD_FD_CB(opcode, prefix);
	else if (opcode >= 0x00 && opcode <= 0x0F) Processing_00_0F(opcode, prefix);
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

void CPU::Processing_DD_FD_CB(unsigned __int8 opcode, unsigned __int8 prefix)
{
	__int8 offset = ram[PC++];
	opcode = ram[PC++];

	switch (opcode)
	{
	case 0x38:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL B,(IX+s)		DD CB ss 38
			BC = 256 * ram[IX + offset] + LOBYTE(BC);
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL B,(IY+s)		FD CB ss 38
			BC = 256 * ram[IY + offset] + LOBYTE(BC);
		}
		break;
	case 0x39:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL C,(IX+s)		DD CB ss 39
			BC = 256 * HIBYTE(BC) + ram[IX + offset];
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL C,(IY+s)		FD CB ss 39
			BC = 256 * HIBYTE(BC) + ram[IY + offset];
		}
		break;
	case 0x3A:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL D,(IX+s)		DD CB ss 3A
			DE = 256 * ram[IX + offset] + LOBYTE(DE);
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL D,(IY+s)		FD CB ss 3A
			DE = 256 * ram[IY + offset] + LOBYTE(DE);
		}
		break;
	case 0x3B:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL E,(IX+s)		DD CB ss 3B
			DE = 256 * HIBYTE(DE) + ram[IX + offset];
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL E,(IY+s)		FD CB ss 3B
			DE = 256 * HIBYTE(DE) + ram[IY + offset];
		}
		break;
	case 0x3C:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL H,(IX+s)		DD CB ss 3C
			HL = 256 * ram[IX + offset] + LOBYTE(HL);
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL H,(IY+s)		FD CB ss 3C
			HL = 256 * ram[IY + offset] + LOBYTE(HL);
		}
		break;
	case 0x3D:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL L,(IX+s)		DD CB ss 3D
			HL = 256 * HIBYTE(HL) + ram[IX + offset];
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL L,(IY+s)		FD CB ss 3D
			HL = 256 * HIBYTE(HL) + ram[IY + offset];
		}
		break;
	case 0x3E:
		if(prefix == 0xDD)
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL (IX+s)	DD CB ss 3E
		else if(prefix == 0xFD)
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL (IY+s)	FD CB ss 3E
		break;
	case 0x3F:
		if (prefix == 0xDD)
		{
			ram[IX + offset] = SRL8(ram[IX + offset]);	// SRL A,(IX+s)		DD CB ss 3F
			AF = 256 * ram[IX + offset] + LOBYTE(AF);
		}
		else if (prefix == 0xFD)
		{
			ram[IY + offset] = SRL8(ram[IY + offset]);	// SRL A,(IY+s)		FD CB ss 3F
			AF = 256 * ram[IY + offset] + LOBYTE(AF);
		}
		break;
	}
}

void CPU::Processing_00_0F(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0x00:
		break;
	case 0x01:		// LD BC,NN		01 NN NN
		LD16(BC, MEM16(ram[PC]));
		PC += 2;
		break;
	case 0x02:
		ram[BC] = HIBYTE(AF);		// LD (BC),A	02
		break;
	case 0x03:
		BC++;		// INC BC		03
		break;
	case 0x04:
		BC = 256 * Add8(HIBYTE(BC), 1, true) + LOBYTE(BC);		// INC B	04
		break;
	case 0x05:
		BC = 256 * SUB8(HIBYTE(BC), 1, true) + LOBYTE(BC);		// DEC B	05
		break;
	case 0x06:		// LD B,N		06 NN
		LD8HI(BC, ram[PC]);
		PC += 1;
		break;
	case 0x07:
		Stop(opcode);
		break;
	case 0x08:		// EX AF, A'F'		08
		EX16(AF, _AF);
		break;
	case 0x09:
		if (prefix == 0xDD)
			IX = ADD16(IX, BC);			// ADD IX,BC	DD 09
		else if (prefix == 0xFD)
			IY = ADD16(IY, BC);			// ADD IY,BC	FD 09
		else
			HL = ADD16(HL, BC);			// ADD HL,BC	09
		break;
	case 0x0A:		// LD A,(BC)
		LD8HI(AF, ram[BC]);
		break;
	case 0x0B:
		BC--;		// DEC BC		1B
		break;
	case 0x0C:
		BC = 256 * HIBYTE(BC) + Add8(LOBYTE(BC), 1, true);		// INC C	0C
		break;
	case 0x0D:
		BC = 256 * HIBYTE(BC) + SUB8(LOBYTE(BC), 1, true);		// DEC C	0D
		break;
	case 0x0E:		// LD C,N		0E NN
		LD8LO(BC, ram[PC]);
		PC += 1;
		break;
	case 0x0F:
		Stop(opcode);
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
		BC = (HIBYTE(BC) - 1) * 256 + LOBYTE(BC);
		if (HIBYTE(BC) == 0)
			PC += 1;
		else
			PC = PC + (__int8)ram[PC] + 1;		// DJNZ s		10 ss
		break;
	case 0x11:		// LD DE,NN		11 NN NN
		LD16(DE, ram[PC + 1] * 256 + ram[PC]);
		PC += 2;
		break;
	case 0x12:
		ram[DE] = HIBYTE(AF);		// LD (DE),A	12
		break;
	case 0x13:
		DE++;		// INC DE		13
		break;
	case 0x14:
		DE = 256 * Add8(HIBYTE(DE), 1, true) + LOBYTE(DE);		// INC D	14
		break;
	case 0x15:
		DE = 256 * SUB8(HIBYTE(DE), 1, true) + LOBYTE(DE);		// DEC D	15
		break;
	case 0x16:		// LD D,N	16 NN
		LD8HI(DE, ram[PC]);
		PC += 1;
		break;
	case 0x17:
		Stop(opcode);
		break;
	case 0x18:
		PC = PC + (__int8)ram[PC] + 1;		// JR s		18 ss
		break;
	case 0x19:
		if (prefix == 0xDD)
			IX = ADD16(IX, DE);				// ADD IX,DE	DD 19
		else if (prefix == 0xFD)
			IY = ADD16(IY, DE);				// ADD IY,DE	FD 19
		else
			HL = ADD16(HL, DE);				// ADD HL,DE	19
		break;
	case 0x1A:		// LD A,(DE)	1A
		LD8HI(AF, ram[DE]);
		break;
	case 0x1B:
		DE--;		// DEC DE		1B
		break;
	case 0x1C:
		DE = 256 * HIBYTE(DE) + Add8(LOBYTE(DE), 1, true);		// INC E	1C
		break;
	case 0x1D:
		DE = 256 * HIBYTE(DE) + SUB8(LOBYTE(DE), 1, true);		// DEC E	1D
		break;
	case 0x1E:		// LD E,N	1E NN
		LD8LO(DE, ram[PC]);
		PC += 1;
		break;
	case 0x1F:
		Stop(opcode);
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
		if (AF & 0b01000000)
			PC += 1;
		else
			PC = PC + (__int8)ram[PC] + 1;		// JR NZ,s		20 ss
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
		if (prefix == 0xDD)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(IX);		// LD (NN),IX	DD 22 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(IX);
			PC += 2;
		}
		else if (prefix == 0xFD)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(IY);		// LD (NN),IY	FD 22 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(IY);
			PC += 2;
		}
		else
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(HL);		// LD (NN),HL	22 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(HL);
			PC += 2;
		}
		break;
	case 0x23:
		if (prefix == 0xDD)
			IX++;					// INC IX	DD 23
		else if (prefix == 0xFD)
			IY++;					// INC IY	FD 23
		else
			HL++;					// INC HL	23
		break;
	case 0x24:
		if (prefix == 0xDD)
		{
			IX = 256 * Add8(HIBYTE(IX), 1, true) + LOBYTE(IX);		// INC XH	DD 24
		}
		else if (prefix == 0xFD)
		{
			IY = 256 * Add8(HIBYTE(IY), 1, true) + LOBYTE(IY);		// INC YH	FD 24
		}
		else
		{
			HL = 256 * Add8(HIBYTE(HL), 1, true) + LOBYTE(HL);		// INC H	24
		}
		break;
	case 0x25:
		if (prefix == 0xDD)
			IX = 256 * SUB8(HIBYTE(IX), 1, true) + LOBYTE(IX);		// DEC XH	DD 25
		else if (prefix == 0xFD)
			IY = 256 * SUB8(HIBYTE(IY), 1, true) + LOBYTE(IY);		// DEC YH	FD 25
		else
			HL = 256 * SUB8(HIBYTE(HL), 1, true) + LOBYTE(HL);		// DEC H	25
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
		Stop(opcode);
		break;
	case 0x28:
		if (prefix == 0xCB)
			BC = 256 * SRA8(HIBYTE(BC)) + LOBYTE(BC);				// SRA B	CB 28
		else
		{
			if (AF & 0b01000000)
				PC = PC + (__int8)ram[PC] + 1;		// JR Z,s		28 ss
			else
				PC += 1;
		}
		break;
	case 0x29:
		if (prefix == 0xCB)
			BC = 256 * HIBYTE(BC) + SRA8(LOBYTE(BC));				// SRA C	CB 29
		else if (prefix == 0xDD)
			IX = ADD16(IX, IX);			// ADD IX,IX	DD 29
		else if (prefix == 0xFD)
			IY = ADD16(IY, IY);			// ADD IY,IY	FD 29
		else
			HL = ADD16(HL, HL);			// ADD HL,HL	29
		break;
	case 0x2A:
		if (prefix == 0xCB)
			DE = 256 * SRA8(HIBYTE(DE)) + LOBYTE(DE);				// SRA D	CB 2A
		else if (prefix == 0xDD)
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
		if (prefix == 0xCB)
			DE = 256 * HIBYTE(DE) + SRA8(LOBYTE(DE));				// SRA E	CB 2B
		else if (prefix == 0xDD)
			IX--;		// DEC IX		DD 2B
		else if (prefix == 0xFD)
			IY--;		// DEC IY		FD 2B
		else
			HL--;		// DEC HL		2B
		break;
	case 0x2C:
		if (prefix == 0xCB)
			HL = 256 * SRA8(HIBYTE(HL)) + LOBYTE(HL);				// SRA H	CB 2C
		else if (prefix == 0xDD)
			IX = 256 * HIBYTE(IX) + Add8(LOBYTE(IX), 1, true);		// INC XL	DD 2C
		else if (prefix == 0xFD)
			IY = 256 * HIBYTE(IY) + Add8(LOBYTE(IY), 1, true);		// INC YL	FD 2C
		else
			HL = 256 * HIBYTE(HL) + Add8(LOBYTE(HL), 1, true);		// INC L	2C
		break;
	case 0x2D:
		if (prefix == 0xCB)
			HL = 256 * HIBYTE(HL) + SRA8(LOBYTE(HL));				// SRA L	CB 2D
		else if (prefix == 0xDD)
			IX = 256 * HIBYTE(IX) + SUB8(LOBYTE(IX), 1, true);		// DEC XL	DD 2D
		else if (prefix == 0xFD)
			IY = 256 * HIBYTE(IY) + SUB8(LOBYTE(IY), 1, true);		// DEC YL	FD 2D
		else
			HL = 256 * HIBYTE(HL) + SUB8(LOBYTE(HL), 1, true);		// DEC L	2D
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
		if (prefix == 0xCB)
			AF = 256 * SRA8(HIBYTE(AF)) + LOBYTE(AF);				// SRA A	CB 2F
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
		if (AF & 0b00000001)
			PC += 1;
		else
			PC = PC + (__int8)ram[PC] + 1;		// JR NC,s		30 ss
		break;
	case 0x31:		// LD SP,NN	31 NN NN
		LD16(SP, ram[PC + 1] * 256 + ram[PC]);
		PC += 2;
		break;
	case 0x32:		// LD (NN),A	32 NN NN
		ram[ram[PC + 1] * 256 + ram[PC]] = HIBYTE(AF);
		PC += 2;
		break;
	case 0x33:
		SP++;
		break;
	case 0x34:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = Add8(ram[IX + (__int8)ram[PC]], 1, true);		// INC (IX+s)	DD 34 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = Add8(ram[IY + (__int8)ram[PC]], 1, true);		// INC (IY+s)	FD 34 ss
			PC += 1;
		}
		else
			ram[HL] = Add8(ram[HL], 1, true);		// INC (HL)		34
		break;
	case 0x35:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = SUB8(ram[IX + (__int8)ram[PC]], 1, true);		// DEC (IX+s)	DD 35 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = SUB8(ram[IY + (__int8)ram[PC]], 1, true);		// DEC (IY+s)	FD 35 ss
			PC += 1;
		}
		else
			ram[HL] = SUB8(ram[HL], 1, true);		// DEC (HL)		35
		break;
	case 0x36:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = ram[PC + 1];		// LD (IX+s),N		DD 36 ss NN
			PC += 2;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = ram[PC + 1];		// LD (IY+s),N		FD 36 ss NN
			PC += 2;
		}
		else
		{
			ram[HL] = ram[PC];		// LD (HL),N	36
			PC += 1;
		}
		break;
	case 0x37:
		Stop(opcode);
		break;
	case 0x38:
		if (prefix == 0xCB)
			BC = 256 * SRL8(HIBYTE(BC)) + LOBYTE(BC);			// SRL B	CB 38
		else
		{
			if (AF & 0b00000001)
				PC = PC + (__int8)ram[PC] + 1;		// JR C,s		38 ss
			else
				PC += 1;
		}
		break;
	case 0x39:
		if (prefix == 0xCB)
			BC = 256 * HIBYTE(BC) + SRL8(LOBYTE(BC));		// SRL C	CB 39
		else if (prefix == 0xDD)
			IX = ADD16(IX, SP);			// ADD IX,SP	DD 39
		else if (prefix == 0xFD)
			IY = ADD16(IY, SP);			// ADD IY,SP	FD 39
		else
			HL = ADD16(HL, SP);			// ADD HL,SP	39
		break;
	case 0x3A:
		if (prefix == 0xCB)
			DE = 256 * SRL8(HIBYTE(DE)) + LOBYTE(DE);		// SRL D	CB 3A
		else
		{
			LD8HI(AF, ram[ram[PC + 1] * 256 + ram[PC]]);	// LD A,(NN)	3A NN NN
			PC += 2;
		}
		break;
	case 0x3B:
		if (prefix == 0xCB)
			DE = 256 * HIBYTE(DE) + SRL8(LOBYTE(DE));		// SRL E	CB 3B
		else
			SP--;		// DEC SP		3B
		break;
	case 0x3C:
		if (prefix == 0xCB)
			HL = 256 * SRL8(HIBYTE(HL)) + LOBYTE(HL);		// SRL H	CB 3C
		else
			AF = 256 * Add8(HIBYTE(AF), 1, true) + LOBYTE(AF);		// INC A	3C
		break;
	case 0x3D:
		if (prefix == 0xCB)
			HL = 256 * HIBYTE(HL) + SRL8(LOBYTE(HL));		// SRL L	CB 3D
		else
			AF = 256 * SUB8(HIBYTE(AF), 1, true) + LOBYTE(AF);		// DEC A	3D
		break;
	case 0x3E:
		if (prefix == 0xCB)
			ram[HL] = SRL8(ram[HL]);	// SRL (HL)		CB 3E
		else
		{
			LD8HI(AF, ram[PC]);		// LD A,N	3E NN
			PC += 1;
		}
		break;
	case 0x3F:
		if(prefix == 0xCB)
			AF = 256 * SRL8(HIBYTE(AF)) + LOBYTE(AF);			// SRL A	CB 3F
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
		Stop(opcode);
		break;
	case 0x41:		// LD B,C	41
		LD8HI(BC, LOBYTE(BC));
		break;
	case 0x42:
		if (prefix == 0xED)
			HL = SBC16(HL, BC);			// SBÑ HL,BC	ED 42
		else
			LD8HI(BC, HIBYTE(DE));		// LD B,D	42
		break;
	case 0x43:
		if (prefix == 0xED)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(BC);		// LD (NN),BC	ED 43 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(BC);
			PC += 2;
		}
		else
			LD8HI(BC, LOBYTE(DE));		// LD B,E	43
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
		{
			LD8HI(BC, ram[IX + (__int8)ram[PC]]);		// LD B,(IX+s)	DD 46 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8HI(BC, ram[IY + (__int8)ram[PC]]);		// LD B,(IY+s)	FD 46 ss
			PC += 1;
		}
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
		Stop(opcode);
		break;
	case 0x4A:
		if (prefix == 0xED)
			HL = ADC16(HL, BC);			// ADÑ HL,BC	ED 4A
		else
			LD8LO(BC, HIBYTE(DE));		// LD C,D	4A
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
		{
			LD8LO(BC, ram[IX + (__int8)ram[PC]]);		// LD C, (IX + s)	DD 4E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8LO(BC, ram[IY + (__int8)ram[PC]]);		// LD C,(IY+s)	FD 4E ss
			PC += 1;
		}
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
		if (prefix == 0xED)
			HL = SBC16(HL, DE);			// SBÑ HL,DE	ED 52
		break;
	case 0x53:
		if (prefix == 0xED)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(DE);		// LD (NN),DE	ED 53 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(DE);
			PC += 2;
		}
		else
			LD8HI(DE, LOBYTE(DE));		// LD D,E	53
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
		{
			LD8HI(DE, ram[IX + (__int8)ram[PC]]);		// LD D,(IX+s)	DD 56 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8HI(DE, ram[IY + (__int8)ram[PC]]);		// LD D,(IY+s)	FD 56 ss
			PC += 1;
		}
		else
			LD8HI(DE, ram[HL]);					// LD D,(HL)	56
		break;
	case 0x57:
		if (prefix == 0xED)
		{
			LD8HI(AF, HIBYTE(IR));		// LD A,I	ED 57
			LD8LO(AF, LOBYTE(AF) & 0b01101101);
			if (HIBYTE(AF) == 0x00) LD8LO(AF, LOBYTE(AF) | 0b01000000);
			LD8LO(AF, LOBYTE(AF) | (IFF2 ? 0b00000100 : 0x00));
		}
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
		if (prefix == 0xED)
		{
			HL = ADC16(HL, DE);			// ADÑ HL,DE	ED 5A
		}
		else
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
	case 0x5E:
		if (prefix == 0xDD)
		{
			LD8LO(DE, ram[IX + (__int8)ram[PC]]);		// LD E,(IX+s)	DD 5E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8LO(DE, ram[IY + (__int8)ram[PC]]);		// LD E,(IY+s)	FD 5E ss
			PC += 1;
		}
		else
			LD8LO(DE, ram[HL]);					// LD E,(HL)	5E
		break;
	case 0x5F:
		if (prefix == 0xED)
		{
			LD8HI(AF, LOBYTE(IR));		// LD A,R	ED 5F
			LD8LO(AF, LOBYTE(AF) & 0b01101101);
			if (HIBYTE(AF) == 0x00) LD8LO(AF, LOBYTE(AF) | 0b01000000);
			LD8LO(AF, LOBYTE(AF) | (IFF2 ? 0b00000100 : 0x00));
		}
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
		if (prefix == 0xED)
			HL = SBC16(HL, HL);			// SBÑ HL,HL	ED 62
		else if (prefix == 0xDD)
			LD8HI(IX, HIBYTE(DE));		// LD XH,D	DD 62
		else if (prefix == 0xFD)
			LD8HI(IY, HIBYTE(DE));		// LD YH,D	FD 62
		else
			LD8HI(HL, HIBYTE(DE));		// LD H,D	62
		break;
	case 0x63:
		if (prefix == 0xED)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(HL);		// LD (NN),HL	ED 63 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(HL);
			PC += 2;
		}
		else if (prefix == 0xDD)
			LD8HI(IX, LOBYTE(DE));		// LD XH,E	DD 63
		else if (prefix == 0xFD)
			LD8HI(IY, LOBYTE(DE));		// LD YH,E	FD 63
		else
			LD8HI(HL, LOBYTE(DE));		// LD H,E	63
		break;
	case 0x64:
		Stop(opcode);
		break;
	case 0x65:
		if (prefix == 0xDD)
			LD8HI(IX, LOBYTE(IX));		// LD XH,XL	DD 65
		else if (prefix == 0xFD)
			LD8HI(IY, LOBYTE(IY));		// LD YH,YL	FD 65
		else
			LD8HI(HL, LOBYTE(HL));		// LD H,L	65
		break;
	case 0x66:
		if (prefix == 0xDD)
		{
			LD8HI(HL, ram[IX + (__int8)ram[PC]]);		// LD H,(IX+s)	DD 66 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8HI(HL, ram[IY + (__int8)ram[PC]]);		// LD H,(IY+s)	FD 66 ss
			PC += 1;
		}
		else
			LD8HI(HL, ram[HL]);					// LD H,(HL)	66
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
		if (prefix == 0xED)
			HL = ADC16(HL, HL);			// ADÑ HL,HL	ED 6A
		else if (prefix == 0xDD)
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
		Stop(opcode);
		break;
	case 0x6E:
		if (prefix == 0xDD)
		{
			LD8LO(HL, ram[IX + (__int8)ram[PC]]);		// LD L,(IX+s)	DD 6E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8LO(HL, ram[IY + (__int8)ram[PC]]);		// LD L,(IY+s)	FD 6E ss
			PC += 1;
		}
		else
			LD8LO(HL, ram[HL]);					// LD L,(HL)	6E
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
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = HIBYTE(BC);		// LD (IX+s),B		DD 70 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = HIBYTE(BC);		// LD (IY+s),B		FD 70 ss
			PC += 1;
		}
		else
			ram[HL] = HIBYTE(BC);		// LD (HL),B	70
		break;
	case 0x71:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = LOBYTE(BC);		// LD (IX+s),C		DD 71 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = LOBYTE(BC);		// LD (IY+s),C		FD 71 ss
			PC += 1;
		}
		else
			ram[HL] = LOBYTE(BC);		// LD (HL),C	71
		break;
	case 0x72:
		if (prefix == 0xED)
			HL = SBC16(HL, SP);			// SBÑ HL,SP	ED 72
		else if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = HIBYTE(DE);		// LD (IX+s),D		DD 72 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = HIBYTE(DE);		// LD (IY+s),D		FD 72 ss
			PC += 1;
		}
		else
			ram[HL] = HIBYTE(DE);		// LD (HL),D	72
		break;
	case 0x73:
		if (prefix == 0xED)
		{
			ram[ram[PC + 1] * 256 + ram[PC]] = LOBYTE(SP);		// LD (NN),SP	ED 73 NN NN
			ram[ram[PC + 1] * 256 + ram[PC] + 1] = HIBYTE(SP);
			PC += 2;
		}
		else if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = LOBYTE(DE);		// LD (IX+s),E		DD 73 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = LOBYTE(DE);		// LD (IY+s),E		FD 73 ss
			PC += 1;
		}
		else
			ram[HL] = LOBYTE(DE);		// LD (HL),E	73
		break;
	case 0x74:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = HIBYTE(HL);		// LD (IX+s),H		DD 74 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = HIBYTE(HL);		// LD (IY+s),H		FD 74 ss
			PC += 1;
		}
		else
			ram[HL] = HIBYTE(HL);		// LD (HL),H	74
		break;
	case 0x75:
		if (prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = LOBYTE(HL);		// LD (IX+s),L		DD 75 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = LOBYTE(HL);		// LD (IY+s),L		FD 75 ss
			PC += 1;
		}
		else
			ram[HL] = LOBYTE(HL);		// LD (HL),L	75
		break;
	case 0x76:
		Stop(opcode);
		break;
	case 0x77:
		if(prefix == 0xDD)
		{
			ram[IX + (__int8)ram[PC]] = HIBYTE(AF);		// LD (IX+s),A		DD 77 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			ram[IY + (__int8)ram[PC]] = HIBYTE(AF);		// LD (IY+s),A		FD 77 ss
			PC += 1;
		}
		else
			ram[HL] = HIBYTE(AF);		// LD (HL),A	77
		break;
	case 0x78:		// LD A,B	78
		LD8HI(AF, HIBYTE(BC));
		break;
	case 0x79:		// LD A,C	79
		LD8HI(AF, LOBYTE(BC));
		break;
	case 0x7A:
		if (prefix == 0xED)
			HL = ADC16(HL, SP);			// ADÑ HL,SP	ED 7A
		else
			LD8HI(AF, HIBYTE(DE));		// LD A,D	7A
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
		{
			LD8HI(AF, ram[IX + (__int8)ram[PC]]);		// LD A,(IX+s)	DD 7E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			LD8HI(AF, ram[IY + (__int8)ram[PC]]);		// LD A,(IY+s)	FD 7E ss
			PC += 1;
		}
		else
			LD8HI(AF, ram[HL]);					// LD A,(HL)	7E
		break;
	case 0x7F:
		Stop(opcode);
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
		AF = 256 * Add8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// ADD A,B		80
		break;
	case 0x81:
		AF = 256 * Add8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// ADD A,C		81
		break;
	case 0x82:
		AF = 256 * Add8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// ADD A,D		82
		break;
	case 0x83:
		AF = 256 * Add8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// ADD A,E		83
		break;
	case 0x84:
		if (prefix == 0xDD)
			AF = 256 * Add8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);	// ADD A,XH		DD 84
		else if(prefix == 0xFD)
			AF = 256 * Add8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);	// ADD A,YH		FD 84
		else
			AF = 256 * Add8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);	// ADD A,H		84
		break;
	case 0x85:
		if (prefix == 0xDD)
			AF = 256 * Add8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);	// ADD A,XL		DD 85
		else if (prefix == 0xFD)
			AF = 256 * Add8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);	// ADD A,YL		FD 85
		else
			AF = 256 * Add8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);	// ADD A,L		85
		break;
	case 0x86:
		if (prefix == 0xDD)
		{
			AF = 256 * Add8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);			// ADD A,(IX+s)		DD 86 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * Add8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);			// ADD A,(IY+s)		FD 86 ss
			PC += 1;
		}
		else
		{
			AF = 256 * Add8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);			// ADD A,(HL)	86
		}
		break;
	case 0x87:
		AF = 256 * Add8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// ADD A,A		87
		break;
	case 0x88:
		AF = 256 * ADC8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// ADC A,B		88
		break;
	case 0x89:
		AF = 256 * ADC8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// ADD A,C		89
		break;
	case 0x8A:
		AF = 256 * ADC8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// ADC A,D		8A
		break;
	case 0x8B:
		AF = 256 * ADC8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// ADC A,E		8B
		break;
	case 0x8C:
		if (prefix == 0xDD)
			AF = 256 * ADC8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);	// ADC A,XH		DD 8C
		else if (prefix == 0xFD)
			AF = 256 * ADC8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);	// ADC A,YH		FD 8C
		else
			AF = 256 * ADC8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);	// ADC A,H	8C
		break;
	case 0x8D:
		if (prefix == 0xDD)
			AF = 256 * ADC8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);	// ADC A,XL		DD 8D
		else if (prefix == 0xFD)
			AF = 256 * ADC8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);	// ADC A,YL		FD 8D
		else
			AF = 256 * ADC8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);	// ADC A,L		8D
		break;
	case 0x8E:
		if (prefix == 0xDD)
		{
			AF = 256 * ADC8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);			// ADC A,(IX+s)		DD 8E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * ADC8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);			// ADC A,(IY+s)		FD 8E ss
			PC += 1;
		}
		else
		{
			AF = 256 * ADC8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);			// ADC A,(HL)	8E
		}
		break;
	case 0x8F:
		AF = 256 * ADC8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// ADC A,A		8F
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
		AF = 256 * SUB8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// SUB B		90
		break;
	case 0x91:
		AF = 256 * SUB8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// SUB C		91
		break;
	case 0x92:
		AF = 256 * SUB8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// SUB D		92
		break;
	case 0x93:
		AF = 256 * SUB8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// SUB E		93
		break;
	case 0x94:
		if(prefix == 0xDD)
			AF = 256 * SUB8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);		// SUB XH		DD 94
		else if(prefix == 0xFD)
			AF = 256 * SUB8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);		// SUB YH		FD 94
		else
			AF = 256 * SUB8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);		// SUB H		94
		break;
	case 0x95:
		if (prefix == 0xDD)
			AF = 256 * SUB8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);		// SUB XL		DD 95
		else if (prefix == 0xFD)
			AF = 256 * SUB8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);		// SUB YL		FD 95
		else
			AF = 256 * SUB8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);		// SUB L		95
		break;
	case 0x96:
		if (prefix == 0xDD)
		{
			AF = 256 * SUB8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);	// SUB (IX + s)		DD 96 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * SUB8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);	// SUB (IY + s)		FD 96 ss
			PC += 1;
		}
		else
			AF = 256 * SUB8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);		// SUB (HL)		96
		break;
	case 0x97:
		AF = 256 * SUB8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// SUB A		97
		break;
	case 0x98:
		AF = 256 * SBC8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// SBC A,B		98
		break;
	case 0x99:
		AF = 256 * SBC8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// SBC A,C		99
		break;
	case 0x9A:
		AF = 256 * SBC8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// SBC A,D		9A
		break;
	case 0x9B:
		AF = 256 * SBC8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// SBC A,E		9B
		break;
	case 0x9C:
		if (prefix == 0xDD)
			AF = 256 * SBC8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);		// SBC A,XH		DD 9C
		else if (prefix == 0xFD)
			AF = 256 * SBC8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);		// SBC A,YH		FD 9C
		else
			AF = 256 * SBC8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);		// SBC A,H		9C
		break;
	case 0x9D:
		if (prefix == 0xDD)
			AF = 256 * SBC8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);		// SBC A,XL		DD 9D
		else if (prefix == 0xFD)
			AF = 256 * SBC8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);		// SBC A,YL		FD 9D
		else
			AF = 256 * SBC8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);		// SBC A,L		9D
		break;
	case 0x9E:
		if (prefix == 0xDD)
		{
			AF = 256 * SBC8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);		// SBC A,(IX+s)		DD 9E ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * SBC8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);		// SBC A,(IY+s)		FD 9E ss
			PC += 1;
		}
		else
			AF = 256 * SBC8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);		// SBC A,(HL)	9E
		break;
	case 0x9F:
		AF = 256 * SBC8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// SBC A,A		9F
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
		AF = 256 * AND8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// AND B	A0
		break;
	case 0xA1:
		AF = 256 * AND8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// AND C	A1
		break;
	case 0xA2:
		AF = 256 * AND8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// AND D	A2
		break;
	case 0xA3:
		AF = 256 * AND8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// AND E	A3
		break;
	case 0xA4:
		if(prefix == 0xDD)
			AF = 256 * AND8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);		// AND XH	DD A4
		else if(prefix == 0xFD)
			AF = 256 * AND8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);		// AND YH	FD A4
		else
			AF = 256 * AND8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);		// AND H	A4
		break;
	case 0xA5:
		if (prefix == 0xDD)
			AF = 256 * AND8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);		// AND XL	DD A5
		else if (prefix == 0xFD)
			AF = 256 * AND8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);		// AND YL	FD A5
		else
			AF = 256 * AND8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);		// AND L	A5
		break;
	case 0xA6:
		if (prefix == 0xDD)
		{
			AF = 256 * AND8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);		// AND (IX+s)	DD A6 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * AND8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);		// AND (IY+s)	FD A6 ss
			PC += 1;
		}
		else
			AF = 256 * AND8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);		// AND (HL)		A6
		break;
	case 0xA7:
		AF = 256 * AND8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// AND A	A7
		break;
	case 0xA8:
		AF = 256 * XOR8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// XOR B	A8
		break;
	case 0xA9:
		AF = 256 * XOR8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// XOR C	A9
		break;
	case 0xAA:
		AF = 256 * XOR8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// XOR D	AA
		break;
	case 0xAB:
		AF = 256 * XOR8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// XOR E	AB
		break;
	case 0xAC:
		if (prefix == 0xDD)
			AF = 256 * XOR8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);		// XOR XH	DD AC
		else if (prefix == 0xFD)
			AF = 256 * XOR8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);		// XOR YH	FD AC
		else
			AF = 256 * XOR8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);		// XOR H	AC
		break;
	case 0xAD:
		if (prefix == 0xDD)
			AF = 256 * XOR8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);		// XOR XL	DD AD
		else if (prefix == 0xFD)
			AF = 256 * XOR8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);		// XOR YL	FD AD
		else
			AF = 256 * XOR8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);		// XOR L	AD
		break;
	case 0xAE:
		if (prefix == 0xDD)
		{
			AF = 256 * XOR8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);		// XOR (IX+s)	DD AE ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * XOR8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);		// XOR (IY+s)	FD AE ss
			PC += 1;
		}
		else
			AF = 256 * XOR8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);		// XOR (HL)		AE
		break;
	case 0xAF:
		AF = 256 * XOR8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// XOR A	AF
		break;
	default:
		break;
	}
}

void CPU::Processing_B0_BF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	unsigned __int8 tmp;

	switch (opcode)
	{
	case 0xB0:
		AF = 256 * OR8(HIBYTE(AF), HIBYTE(BC)) + LOBYTE(AF);		// OR B		B0
		break;
	case 0xB1:
		AF = 256 * OR8(HIBYTE(AF), LOBYTE(BC)) + LOBYTE(AF);		// OR C		B1
		break;
	case 0xB2:
		AF = 256 * OR8(HIBYTE(AF), HIBYTE(DE)) + LOBYTE(AF);		// OR D		B2
		break;
	case 0xB3:
		AF = 256 * OR8(HIBYTE(AF), LOBYTE(DE)) + LOBYTE(AF);		// OR E		B3
		break;
	case 0xB4:
		if (prefix == 0xDD)
			AF = 256 * OR8(HIBYTE(AF), HIBYTE(IX)) + LOBYTE(AF);		// OR XH	DD B4
		else if (prefix == 0xFD)
			AF = 256 * OR8(HIBYTE(AF), HIBYTE(IY)) + LOBYTE(AF);		// OR YH	FD B4
		else
			AF = 256 * OR8(HIBYTE(AF), HIBYTE(HL)) + LOBYTE(AF);		// OR H		B4
		break;
	case 0xB5:
		if (prefix == 0xDD)
			AF = 256 * OR8(HIBYTE(AF), LOBYTE(IX)) + LOBYTE(AF);		// OR XL	DD B5
		else if (prefix == 0xFD)
			AF = 256 * OR8(HIBYTE(AF), LOBYTE(IY)) + LOBYTE(AF);		// OR YL	FD B5
		else
			AF = 256 * OR8(HIBYTE(AF), LOBYTE(HL)) + LOBYTE(AF);		// OR L		B5
		break;
	case 0xB6:
		if (prefix == 0xDD)
		{
			AF = 256 * OR8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]) + LOBYTE(AF);		// OR (IX+s)	DD B6 ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			AF = 256 * OR8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]) + LOBYTE(AF);		// OR (IY+s)	FD B6 ss
			PC += 1;
		}
		else
			AF = 256 * OR8(HIBYTE(AF), ram[HL]) + LOBYTE(AF);		// OR (HL)	B6
		break;
	case 0xB7:
		AF = 256 * OR8(HIBYTE(AF), HIBYTE(AF)) + LOBYTE(AF);		// OR A		B7
		break;
	case 0xB8:
		CP8(HIBYTE(AF), HIBYTE(BC));	// CP B		B8
		break;
	case 0xB9:
		CP8(HIBYTE(AF), LOBYTE(BC));	// CP C		B9
		break;
	case 0xBA:
		CP8(HIBYTE(AF), HIBYTE(DE));	// CP D		BA
		break;
	case 0xBB:
		CP8(HIBYTE(AF), LOBYTE(DE));	// CP E		BB
		break;
	case 0xBC:
		if(prefix == 0xDD)
			CP8(HIBYTE(AF), HIBYTE(IX));	// CP XH	DD BC
		else if(prefix == 0xFD)
			CP8(HIBYTE(AF), HIBYTE(IY));	// CP XL	DD BD
		else
			CP8(HIBYTE(AF), HIBYTE(HL));	// CP H		BC
		break;
	case 0xBD:
		if (prefix == 0xDD)
			CP8(HIBYTE(AF), LOBYTE(IX));	// CP XL	DD BD
		else if (prefix == 0xFD)
			CP8(HIBYTE(AF), LOBYTE(IY));	// CP YL	FD BD
		else
			CP8(HIBYTE(AF), LOBYTE(HL));	// CP L		BD
		break;
	case 0xBE:
		if (prefix == 0xDD)
		{
			CP8(HIBYTE(AF), ram[IX + (__int8)ram[PC]]);		// CP (IX+s)	DD BE ss
			PC += 1;
		}
		else if (prefix == 0xFD)
		{
			CP8(HIBYTE(AF), ram[IY + (__int8)ram[PC]]);		// CP (IY+s)	FD BE ss
			PC += 1;
		}
		else
			CP8(HIBYTE(AF), ram[HL]);	// CP (HL)		BE
		break;
	case 0xBF:
		CP8(HIBYTE(AF), HIBYTE(AF));	// CP A		BF
		tmp = LOBYTE(AF) & ~(F5 | F3);
		AF = 256 * HIBYTE(AF) + (tmp | (HIBYTE(AF) & (F5 | F3)));
		break;
	default:
		break;
	}
}

void CPU::Processing_C0_CF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xC0:					// RET NZ		C0
		if (AF & 0b01000000)
			PC += 0;
		else
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		break;
	case 0xC1:
		POP(BC);			// POP BC		C1
		break;
	case 0xC2:
		if (AF & 0b01000000)
			PC += 2;
		else
			PC = MEM16(ram[PC]);		// JP NZ,NN		C2 NN NN
		break;
	case 0xC3:
		PC = MEM16(ram[PC]);			// JP NN	C3 NN NN
		break;
	case 0xC4:							// CALL NZ,NN		C4 NN NN
		if (AF & 0b01000000)
			PC += 2;
		else
			CALL(MEM16(ram[PC]));
		break;
	case 0xC5:
		PUSH(BC);			// PUSH BC		C5
		break;
	case 0xC6:
		AF = 256 * Add8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// ADD A,N	C6 NN
		PC += 1;
		break;
	case 0xC7:
		RST(0x0000);
		break;
	case 0xC8:						// RET Z		C8
		if (AF & 0b01000000)
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		else
			PC += 0;
		break;
	case 0xC9:						// RET		C9
		PC = MEM16(ram[SP]);
		SP = SP + 2;
		break;
	case 0xCA:
		if (AF & 0b01000000)
			PC = MEM16(ram[PC]);		// JP Z,NN		CA NN NN
		else
			PC += 2;
		break;
	case 0xCB:
		Stop(opcode);
		break;
	case 0xCC:						// CALL Z,NN		CC NN NN
		if (AF & 0b01000000)
			CALL(MEM16(ram[PC]));
		else
			PC += 2;
		break;
	case 0xCD:					// CALL NN		CD NN NN
		CALL(MEM16(ram[PC]));
		break;
	case 0xCE:
		AF = 256 * ADC8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// ADC A,N		CE NN
		PC += 1;
		break;
	case 0xCF:
		RST(0x0008);		// RST #8		CF
		break;
	default:
		break;
	}
}

void CPU::Processing_D0_DF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xD0:						// RET NC		D0
		if (AF & 0b00000001)
			PC += 0;
		else
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		break;
	case 0xD1:
		POP(DE);			// POP DE		D1
		break;
	case 0xD2:
		if (AF & 0b00000001)
			PC += 2;
		else
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP NC,NN		D2 NN NN
		break;
	case 0xD3:
		Stop(opcode);
		break;
	case 0xD4:						// CALL NC,NN	D4 NN NN
		if (AF & 0b00000001)
			PC += 2;
		else
			CALL(MEM16(ram[PC]));
		break;
	case 0xD5:
		PUSH(DE);		// PUSH DE		D5
		break;
	case 0xD6:
		AF = 256 * SUB8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// SUB N		D6 NN
		PC += 1;
		break;
	case 0xD7:
		RST(0x0010);		// RST #10		D7
		break;
	case 0xD8:						// RET C		D8
		if (AF & 0b00000001)
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		else
			PC += 0;
		break;
	case 0xD9:		// EXX D9
		EXX();
		break;
	case 0xDA:
		if (AF & 0b00000001)
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP C,NN		DA NN NN
		else
			PC += 2;
		break;
	case 0xDB:
		Stop(opcode);
		break;
	case 0xDC:							// CALL C,NN		DC NN NN
		if (AF & 0b00000001)
			CALL(MEM16(ram[PC]));
		else
			PC += 2;
		break;
	case 0xDD:
		Stop(opcode);
		break;
	case 0xDE:
		AF = 256 * SBC8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// SBC A,N		DE NN
		PC += 1;
		break;
	case 0xDF:
		RST(0x0018);		// RST #18		DF
		break;
	default:
		break;
	}
}

void CPU::Processing_E0_EF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xE0:						// RET PO		E0
		if (AF & 0b00000100)
			PC += 0;
		else
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		break;
	case 0xE1:
		if(prefix == 0xDD)
			POP(IX);			// POP IX		DD E1
		else if (prefix == 0xFD)
			POP(IY);			// POP IY		FD E1
		else
			POP(HL);			// POP HL		E1
		break;
	case 0xE2:
		if (AF & 0b00000100)
			PC += 2;
		else
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP PO,NN		E2 NN NN
		break;
	case 0xE3:		// EX (SP),HL	E3
		if (prefix == 0xDD)
		{
			EX16((unsigned __int16&)ram[SP], IX);
		}
		else if (prefix == 0xFD)
		{
			EX16((unsigned __int16&)ram[SP], IY);
		}
		else
		{
			EX16((unsigned __int16&)ram[SP], HL);
		}
		break;
	case 0xE4:						// CALL PO,NN		E4 NN NN
		if (AF & 0b00000100)
			PC += 2;
		else
			CALL(MEM16(ram[PC]));
		break;
	case 0xE5:
		if(prefix == 0xDD)
			PUSH(IX);		// PUSH IX		DD E5
		else if (prefix == 0xFD)
			PUSH(IY);		// PUSH IY		FD E5
		else
			PUSH(HL);		// PUSH HL		E5
		break;
	case 0xE6:
		AF = 256 * AND8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// AND N	E6 NN
		PC += 1;
		break;
	case 0xE7:
		RST(0x0020);		// RST #20		E7
		break;
	case 0xE8:						// RET PE		E8
		if (AF & 0b00000100)
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		else
			PC += 0;
		break;
	case 0xE9:
		if (prefix == 0xDD)
			PC = IX;				// JP (IX)	DD E9
		else if (prefix == 0xFD)
			PC = IY;				// JP (IY)	FD E9
		else
			PC = HL;				// JP (HL)	E9
		break;
	case 0xEA:
		if (AF & 0b00000100)
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP PE,NN		EA NN NN
		else
			PC += 2;
		break;
	case 0xEB:		// EX DE,HL
		EX16(DE, HL);
		break;
	case 0xEC:						// CALL PE,NN		EC NN NN
		if (AF & 0b00000100)
			CALL(MEM16(ram[PC]));
		else
			PC += 2;
		break;
	case 0xED:
		Stop(opcode);
		break;
	case 0xEE:
		AF = 256 * XOR8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// XOR N	EE NN
		PC += 1;
		break;
	case 0xEF:
		RST(0x0028);		// RST #28		EF
		break;
	default:
		break;
	}
}

void CPU::Processing_F0_FF(unsigned __int8 opcode, unsigned __int8 prefix)
{
	switch (opcode)
	{
	case 0xF0:						// RET P		F0
		if (AF & 0b10000000)
			PC += 0;
		else
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		break;
	case 0xF1:
		POP(AF);			// POP AF		F1
		break;
	case 0xF2:
		if (AF & 0b10000000)
			PC += 2;
		else
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP P,NN		F2 NN NN
		break;
	case 0xF3:
		Stop(opcode);
		break;
	case 0xF4:						// CALL P,NN		F4 NN NN
		if (AF & 0b10000000)
			PC += 2;
		else
			CALL(MEM16(ram[PC]));
		break;
	case 0xF5:
		PUSH(AF);					// PUSH AF		F5
		break;
	case 0xF6:
		AF = 256 * OR8(HIBYTE(AF), ram[PC]) + LOBYTE(AF);		// OR N		F6 NN
		PC += 1;
		break;
	case 0xF7:
		RST(0x0030);				// RST #30		F7
		break;
	case 0xF8:						// RET M		F8
		if (AF & 0b10000000)
		{
			PC = MEM16(ram[SP]);
			SP = SP + 2;
		}
		else
			PC += 0;
		break;
	case 0xF9:
		if (prefix == 0xDD)
			LD16(SP, IX);			// LD SP,HL	F9
		else if (prefix == 0xFD)
			LD16(SP, IY);			// LD SP,IX	DD F9
		else
			LD16(SP, HL);			// LD SP,IY	FD F9
		break;
	case 0xFA:
		if (AF & 0b10000000)
			PC = (ram[PC] + 256 * ram[PC + 1]);		// JP M,NN		FA NN NN
		else
			PC += 2;
		break;
	case 0xFB:
		Stop(opcode);
		break;
	case 0xFC:								// CALL M,NN		FC NN NN
		if (AF & 0b10000000)
			CALL(MEM16(ram[PC]));
		else
			PC += 2;
		break;
	case 0xFD:
		Stop(opcode);
		break;
	case 0xFE:
		CP8(HIBYTE(AF), ram[PC]);	// CP N		FE NN
		PC += 1;
		break;
	case 0xFF:
		RST(0x0038);		// RST #38		FF
		break;
	default:
		break;
	}
}
