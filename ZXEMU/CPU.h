#pragma once

#define LOBYTE(w) ((unsigned __int8)(w))
#define HIBYTE(w) ((unsigned __int8)(((unsigned __int16)(w) >> 8) & 0xFF))
#define LOWORD(w) ((unsigned __int16)(w))
#define HIWORD(w) ((unsigned __int16)(((unsigned __int32)(w) >> 16) & 0xFFFF))
#define MEM16(m) *(__int16*)&m

#define SF 0b10000000
#define ZF 0b01000000
#define F5 0b00100000
#define HF 0b00010000
#define F3 0b00001000
#define PF 0b00000100
#define VF 0b00000100
#define NF 0b00000010
#define CF 0b00000001

class CPU
{
protected:
	unsigned __int8 *ram;
public:
	unsigned __int16 PC;
	unsigned __int16 AF;
	unsigned __int16 BC;
	unsigned __int16 DE;
	unsigned __int16 HL;
	unsigned __int16 IX;
	unsigned __int16 IY;
	unsigned __int16 SP;
	unsigned __int16 IR;
	unsigned __int16 _AF;
	unsigned __int16 _BC;
	unsigned __int16 _DE;
	unsigned __int16 _HL;
	bool IFF2;
public:
	CPU(unsigned __int8 *pram);
	~CPU();
	void Step();
protected:
	void Processing_DD_FD_CB(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_00_0F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_10_1F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_20_2F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_30_3F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_40_4F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_50_5F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_60_6F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_70_7F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_80_8F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_90_9F(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_A0_AF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_B0_BF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_C0_CF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_D0_DF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_E0_EF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
	void Processing_F0_FF(unsigned __int8 opcode, unsigned __int8 prefix = 0);
public:
	static void LD16(unsigned __int16& reg, unsigned __int16 val);
	static void LD8LO(unsigned __int16& reg, unsigned __int8 val);
	static void LD8HI(unsigned __int16& reg, unsigned __int8 val);
	static void EX16(unsigned __int16& reg1, unsigned __int16& reg2);
	void EXX();
	void PUSH(unsigned __int16& reg);
	void POP(unsigned __int16& reg);
	void RST(unsigned __int16 addr);
	void CALL(unsigned __int16 addr);
	unsigned __int8 Add8(unsigned __int8 reg1, unsigned __int8 reg2, bool withoutCF = false);
	unsigned __int8 INC8(unsigned __int8 reg1);
	unsigned __int8 ADC8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int16 ADD16(unsigned __int16 reg1, unsigned __int16 reg2);
	unsigned __int16 ADC16(unsigned __int16 reg1, unsigned __int16 reg2);
	unsigned __int8 SUB8(unsigned __int8 reg1, unsigned __int8 reg2, bool withoutCF = false);
	unsigned __int8 SBC8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int16 SBC16(unsigned __int16 reg1, unsigned __int16 reg2);
	void CP8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int8 AND8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int8 OR8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int8 XOR8(unsigned __int8 reg1, unsigned __int8 reg2);
	unsigned __int8 SRL8(unsigned __int8 reg1);
	void Stop(unsigned __int8 opcode);
};
