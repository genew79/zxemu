#pragma once

#define LOBYTE(w) ((unsigned __int8)(w))
#define HIBYTE(w) ((unsigned __int8)(((unsigned __int16)(w) >> 8) & 0xFF))

class CPU
{
protected:
	unsigned __int8 *ram;
public:
	unsigned __int16 PC;
	unsigned __int16 SP;
	unsigned __int16 IX;
	unsigned __int16 IY;
	unsigned __int16 AF;
	unsigned __int16 BC;
	unsigned __int16 DE;
	unsigned __int16 HL;
	unsigned __int16 _AF;
	unsigned __int16 _BC;
	unsigned __int16 _DE;
	unsigned __int16 _HL;
public:
	CPU(unsigned __int8 *pram);
	~CPU();
	void Step();
protected:
	void Processing_00_0F(unsigned __int8 opcode);
	void Processing_10_1F(unsigned __int8 opcode);
	void LD16(unsigned __int16& reg, unsigned __int16 val);
	void LD8LO(unsigned __int16& reg, unsigned __int8 val);
	void LD8HI(unsigned __int16& reg, unsigned __int8 val);
	void EX16(unsigned __int16& reg1, unsigned __int16& reg2);
	void EXX();
};
