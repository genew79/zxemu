#include "CppUnitTest.h"
#include "../ZxEmu/CPU.h"
#include "../ZxEmu/EmuModel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZxEmuTest
{		
	unsigned __int8 mem[65536];
	CPU *cpu;

	TEST_CLASS(CPUTest)
	{
	public:
		
		TEST_CLASS_INITIALIZE(TestClassInitialize)
		{
			cpu = new CPU(mem);
		}
		
		TEST_CLASS_CLEANUP(methodName)
		{
			delete cpu;
		}
		
		TEST_METHOD_INITIALIZE(TestMethodInitialization)
		{
			cpu->PC = 0x0000;
			cpu->AF = 0x0000;
			cpu->BC = 0x0000;
			cpu->DE = 0x0000;
			cpu->HL = 0x0000;
			cpu->IX = 0x0000;
			cpu->IY = 0x0000;
			cpu->SP = 0x0000;
			cpu->IR = 0x0000;
			cpu->_AF = 0x0000;
			cpu->_BC = 0x0000;
			cpu->_DE = 0x0000;
			cpu->_HL = 0x0000;
		}

		TEST_METHOD(TestLD16)
		{
			unsigned __int16 reg;
			unsigned __int16 val = 0xFFFF;
			CPU::LD16(reg, val);
			Assert::IsTrue(reg == 0xFFFF);
		}

		TEST_METHOD(TestLD8HI)
		{
			unsigned __int16 reg = 0xAAAA;
			unsigned __int8 val = 0xCC;
			CPU::LD8HI(reg, val);
			Assert::IsTrue(reg == 0xCCAA);
		}

		TEST_METHOD(TestLD8LO)
		{
			unsigned __int16 reg = 0xAAAA;
			unsigned __int8 val = 0xCC;
			CPU::LD8LO(reg, val);
			Assert::IsTrue(reg == 0xAACC);
		}

		TEST_METHOD(TestAdd8)
		{
			cpu->AF = 0x0000;
			auto res = cpu->Add8(0xFF, 0x01);
			Assert::AreEqual<unsigned __int8>(0x00, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			cpu->AF = 0x0000;
			res = cpu->Add8(0x0F, 0x09);
			Assert::AreEqual<unsigned __int8>(0x18, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | F3, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestSUB8)
		{
			cpu->AF = 0x0000;
			auto res = cpu->SUB8(0x00, 0x01);
			Assert::AreEqual<unsigned __int8>(0xFF, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			cpu->AF = 0x0000;
			res = cpu->SUB8(0x0F, 0x07);
			Assert::AreEqual<unsigned __int8>(0x08, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | NF, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestSBC16)
		{
			cpu->AF = 0x0000;
			auto res = cpu->SBC16(0x0001, 0x0001);
			Assert::AreEqual<__int16>(0x0000, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			cpu->AF = 0x0001;
			res = cpu->SBC16(0x0001, 0x0001);
			Assert::AreEqual<__int16>(-1, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestCP8)
		{
			cpu->AF = 0x0000;
			cpu->CP8(0x00, 0x01);
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			cpu->AF = 0x0000;
			cpu->CP8(0x0F, 0x07);
			Assert::AreEqual<unsigned __int8>(F3 | NF, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestAND8)
		{
			auto res = cpu->AND8(0xFF, 0xFF);
			Assert::AreEqual<unsigned __int8>(0xFF, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->AND8(0xF0, 0x0F);
			Assert::AreEqual<unsigned __int8>(0x00, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->AND8(0xFF, 0x80);
			Assert::AreEqual<unsigned __int8>(0x80, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestOR8)
		{
			auto res = cpu->OR8(0xF0, 0x0F);
			Assert::AreEqual<unsigned __int8>(0xFF, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->OR8(0x00, 0x00);
			Assert::AreEqual<unsigned __int8>(0x00, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->OR8(0x00, 0x8F);
			Assert::AreEqual<unsigned __int8>(0x8F, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestXOR8)
		{
			auto res = cpu->XOR8(0xF0, 0x0F);
			Assert::AreEqual<unsigned __int8>(0xFF, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->XOR8(0x00, 0x00);
			Assert::AreEqual<unsigned __int8>(0x00, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			res = cpu->XOR8(0xFF, 0x70);
			Assert::AreEqual<unsigned __int8>(0x8F, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(TestSRL8)
		{
			auto res = cpu->SRL8(0x1F);
			Assert::AreEqual<unsigned __int8>(0x0F, res, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
		}

		TEST_METHOD(Test0x01)	// LD BC,NN		01 NN NN
		{
			mem[0] = 0x01;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x02)	// LD (BC),A	02
		{
			mem[0] = 0x02;
			mem[0xABCD] = 0x00;
			cpu->AF = 0x12EE;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x03)	// INC BC	03
		{
			mem[0] = 0x03;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x04)	// INC B	04
		{
			mem[0] = 0x04;
			cpu->AF = 0x0002;
			cpu->BC = 0xFF00;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->BC, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x05)	// DEC B	05
		{
			mem[0] = 0x05;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x06)	// LD B,N	06 NN
		{
			mem[0] = 0x06;
			mem[1] = 0x12;
			cpu->BC = 0x0034;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x08)	// EX AF, A'F'		08
		{
			mem[0] = 0x08;
			cpu->AF = 0x1234;
			cpu->_AF = 0x5678;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x5678, L"Memory wrong");
			Assert::IsTrue(cpu->_AF == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x09)	// ADD HL,BC	09
		{
			mem[0] = 0x09;
			cpu->AF = 0x0002;
			cpu->HL = 0xFFFF;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD09)	// ADD IX,BC	DD 09
		{
			mem[0] = 0xDD;
			mem[1] = 0x09;
			cpu->AF = 0x0002;
			cpu->IX = 0xFFFF;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD09)	// ADD IY,BC	FD 09
		{
			mem[0] = 0xFD;
			mem[1] = 0x09;
			cpu->AF = 0x0002;
			cpu->IY = 0xFFFF;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x0A)	// LD A,(BC)	0A
		{
			mem[0] = 0x0A;
			mem[1] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x0B)	// DEC BC	0B
		{
			mem[0] = 0x0B;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x0C)	// INC C	0C
		{
			mem[0] = 0x0C;
			cpu->AF = 0x0002;
			cpu->BC = 0x00FF;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->BC, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x0D)	// DEC C	0D
		{
			mem[0] = 0x0D;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x0E)	// LD C,N	0E NN
		{
			mem[0] = 0x0E;
			mem[1] = 0xDD;
			cpu->BC = 0xCC00;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCCDD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x10)	// DJNZ s		10 ss
		{
			mem[0] = 0x10;
			mem[1] = (__int8)127;
			cpu->BC = 0x0200;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->BC = 0x0200;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0x11)	// LD DE,NN		11 NN NN
		{
			mem[0] = 0x11;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x12)	// LD (DE),A	12
		{
			mem[0] = 0x12;
			mem[0xABCD] = 0x00;
			cpu->AF = 0x12EE;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x13)	// INC DE	13
		{
			mem[0] = 0x13;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x14)	// INC D	14
		{
			mem[0] = 0x14;
			cpu->AF = 0x0002;
			cpu->DE = 0xFF00;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->DE, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x15)	// DEC D	15
		{
			mem[0] = 0x15;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x16)	// LD D,N	16 NN
		{
			mem[0] = 0x16;
			mem[1] = 0x12;
			cpu->DE = 0x0034;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x18)	// JR s		18 ss
		{
			mem[0] = 0x18;
			mem[1] = (__int8)127;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0x19)	// ADD HL,DE	19
		{
			mem[0] = 0x19;
			cpu->AF = 0x0002;
			cpu->HL = 0xFFFF;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD19)	// ADD IX,DE	DD 19
		{
			mem[0] = 0xDD;
			mem[1] = 0x19;
			cpu->AF = 0x0002;
			cpu->IX = 0xFFFF;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD19)	// ADD IY,DE	FD 19
		{
			mem[0] = 0xFD;
			mem[1] = 0x19;
			cpu->AF = 0x0002;
			cpu->IY = 0xFFFF;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x1A)	// LD A,(DE)	1A
		{
			mem[0] = 0x1A;
			mem[1] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x1B)	// DEC DE	1B
		{
			mem[0] = 0x1B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x1C)	// INC E	1C
		{
			mem[0] = 0x1C;
			cpu->AF = 0x0002;
			cpu->DE = 0x00FF;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->DE, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x1D)	// DEC E	1D
		{
			mem[0] = 0x1D;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x1E)	// LD E,N	1E NN
		{
			mem[0] = 0x1E;
			mem[1] = 0xDD;
			cpu->DE = 0xCC00;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCCDD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x20)	// JR NZ,s		20 ss
		{
			mem[0] = 0x20;
			mem[1] = (__int8)127;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0x21)	// LD HL, NN	21 NN NN
		{
			mem[0] = 0x21;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xDD21)	// LD IX, NN	DD 21 NN NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x21;
			mem[2] = 0x34;
			mem[3] = 0x12;
			cpu->IX = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFD21)	// LD IY, NN	FD 21 NN NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x21;
			mem[2] = 0x34;
			mem[3] = 0x12;
			cpu->IY = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x1234, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x22)	// LD (NN),HL	22 NN NN
		{
			mem[0] = 0x22;
			mem[1] = 0xCD;
			mem[2] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xDD22)	// LD (NN),IX	DD 22 NN NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x22;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->IX = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFD22)	// LD (NN),IY	FD 22 NN NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x22;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->IY = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x23)	// INC HL	23
		{
			mem[0] = 0x23;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD23)	// INC IX	DD 23
		{
			mem[0] = 0xDD;
			mem[1] = 0x23;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD23)	// INC IY	FD 23
		{
			mem[0] = 0xFD;
			mem[1] = 0x23;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x24)	// INC H	24
		{
			mem[0] = 0x24;
			cpu->AF = 0x0002;
			cpu->HL = 0xFF00;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD24)	// INC XH	DD 24
		{
			mem[0] = 0xDD;
			mem[1] = 0x24;
			cpu->AF = 0x0002;
			cpu->IX = 0xFF00;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD24)	// INC YH	FD 24
		{
			mem[0] = 0xFD;
			mem[1] = 0x24;
			cpu->AF = 0x0002;
			cpu->IY = 0xFF00;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x25)	// DEC H	25
		{
			mem[0] = 0x25;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD25)	// DEC XH	DD 25
		{
			mem[0] = 0xDD;
			mem[1] = 0x25;
			cpu->IX = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD25)	// DEC YH	FD 25
		{
			mem[0] = 0xFD;
			mem[1] = 0x25;
			cpu->IY = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x26)	// LD H,N	26 NN
		{
			mem[0] = 0x26;
			mem[1] = 0xCC;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xCC00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xDD26)	// LD XH,N	DD 26 NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x26;
			mem[2] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xCC00);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0xFD26)	// LD YH,N	FD 26 NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x26;
			mem[2] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xCC00);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x28)	// JR Z,s		28 ss
		{
			mem[0] = 0x28;
			mem[1] = (__int8)127;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0x29)	// ADD HL,HL	29
		{
			mem[0] = 0x29;
			cpu->AF = 0x0002;
			cpu->HL = 0x8000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD29)	// ADD IX,IX	DD 29
		{
			mem[0] = 0xDD;
			mem[1] = 0x29;
			cpu->AF = 0x0002;
			cpu->IX = 0x8000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD29)	// ADD IY,IY	FD 29
		{
			mem[0] = 0xFD;
			mem[1] = 0x29;
			cpu->AF = 0x0002;
			cpu->IY = 0x8000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x2A)	// LD HL,(NN)	2A NN NN
		{
			mem[0] = 0x2A;
			mem[1] = 0xCD;
			mem[2] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xFEEF);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0xDD2A)	// LD IX,(NN)	DD 2A NN NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x2A;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0xFD2A)	// LD IY,(NN)	FD 2A NN NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x2A;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0x2B)	// DEC HL	2B
		{
			mem[0] = 0x2B;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD2B)	// DEC IX	DD 2B
		{
			mem[0] = 0xDD;
			mem[1] = 0x2B;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD2B)	// DEC IY	FD 2B
		{
			mem[0] = 0xFD;
			mem[1] = 0x2B;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x2C)	// INC L	2C
		{
			mem[0] = 0x2C;
			cpu->AF = 0x0002;
			cpu->HL = 0x00FF;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD2C)	// INC XL	DD 2C
		{
			mem[0] = 0xDD;
			mem[1] = 0x2C;
			cpu->AF = 0x0002;
			cpu->IX = 0x00FF;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD2C)	// INC YL	FD 2C
		{
			mem[0] = 0xFD;
			mem[1] = 0x2C;
			cpu->AF = 0x0002;
			cpu->IY = 0x00FF;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x2D)	// DEC L	2D
		{
			mem[0] = 0x2D;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD2D)	// DEC XL	DD 2D
		{
			mem[0] = 0xDD;
			mem[1] = 0x2D;
			cpu->IX = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD2D)	// DEC YL	FD 2D
		{
			mem[0] = 0xFD;
			mem[1] = 0x2D;
			cpu->IY = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x2E)	// LD L,N	2E NN
		{
			mem[0] = 0x2E;
			mem[1] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00CC);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xDD2E)	// LD XL,N	DD 2E NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x2E;
			mem[2] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00CC);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0xFD2E)	// LD YL,N	FD 2E NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x2E;
			mem[2] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00CC);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x30)	// JR NC,s		30 ss
		{
			mem[0] = 0x30;
			mem[1] = (__int8)127;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0x31)	// LD SP,NN	31 NN NN
		{
			mem[0] = 0x31;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x32)	// LD (NN),A	32 NN NN
		{
			mem[0] = 0x32;
			mem[1] = 0xCD;
			mem[2] = 0xAB;
			mem[0xABCD] = 0x00;
			cpu->AF = 0xEF00;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0xEF);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x33)		// INC SP	33
		{
			mem[0] = 0x33;
			cpu->SP = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xABCE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x34)		// INC (HL)		34
		{
			mem[0] = 0x34;
			mem[0xA000] = 0xFF;
			cpu->AF = 0x0002;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD34)		// INC (IX+s)	DD 34 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x34;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xFF;
			cpu->AF = 0x0002;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xFF;
			cpu->AF = 0x0002;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD34)		// INC (IY+s)	FD 34 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x34;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xFF;
			cpu->AF = 0x0002;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xFF;
			cpu->AF = 0x0002;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x35)	// DEC (HL)		35
		{
			mem[0] = 0x35;
			mem[0xA000] = 0x00;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD35)		// DEC (IX+s)	DD 35 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x35;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x00;
			cpu->AF = 0x0002;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0xFF, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x00;
			cpu->AF = 0x0002;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0xFF, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD35)		// DEC (IY+s)	FD 35 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x35;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x00;
			cpu->AF = 0x0002;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0xFF, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x00;
			cpu->AF = 0x0002;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<__int16>(0xFF, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x36)		// LD (HL),N	36
		{
			mem[0] = 0x36;
			mem[1] = 0x12;
			mem[0xABCD] = 0x00;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDD36)		// LD (IX+s),N		DD 36 ss NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x36;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			mem[3] = 0x12;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFD36)	// LD (IY+s),N		FD 36 ss NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x36;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			mem[3] = 0x12;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x38)	// JR C,s		38 ss
		{
			mem[0] = 0x38;
			mem[1] = (__int8)127;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(129, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			mem[1] = (__int8)-128;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(-126, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xCB38)		// SRL B	CB 38
		{
			mem[0] = 0xCB;
			mem[1] = 0x38;
			cpu->BC = 0x1F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB38)		// SRL B,(IX+s)		DD CB ss 38
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x38;
			mem[0xA000 + 127] = 0x1F;
			cpu->BC = 0xFF00;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->BC = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB38)		// SRL B,(IY+s)		FD CB ss 38
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x38;
			mem[0xA000 + 127] = 0x1F;
			cpu->BC = 0xFF00;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->BC = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x39)	// ADD HL,SP	39
		{
			mem[0] = 0x39;
			cpu->AF = 0x0002;
			cpu->HL = 0xFFFF;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xCB39)		// SRL C	CB 39
		{
			mem[0] = 0xCB;
			mem[1] = 0x39;
			cpu->BC = 0x001F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDD39)	// ADD IX,SP	DD 39
		{
			mem[0] = 0xDD;
			mem[1] = 0x39;
			cpu->AF = 0x0002;
			cpu->IX = 0xFFFF;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IX, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD39)	// ADD IY,SP	FD 39
		{
			mem[0] = 0xFD;
			mem[1] = 0x39;
			cpu->AF = 0x0002;
			cpu->IY = 0xFFFF;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->IY, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB39)		// SRL C,(IX+s)		DD CB ss 39
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x39;
			mem[0xA000 + 127] = 0x1F;
			cpu->BC = 0x00FF;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->BC = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB39)		// SRL C,(IY+s)		FD CB ss 39
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x39;
			mem[0xA000 + 127] = 0x1F;
			cpu->BC = 0x00FF;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->BC = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x3A)	// LD A,(NN)	3A NN NN
		{
			mem[0] = 0x3A;
			mem[1] = 0xCD;
			mem[2] = 0xAB;
			mem[0xABCD] = 0xEF;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xEF00);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0xCB3A)		// SRL D	CB 3A
		{
			mem[0] = 0xCB;
			mem[1] = 0x3A;
			cpu->DE = 0x1F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3A)		// SRL D,(IX+s)		DD CB ss 3A
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3A;
			mem[0xA000 + 127] = 0x1F;
			cpu->DE = 0xFF00;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->DE = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3A)		// SRL D,(IY+s)		FD CB ss 3A
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3A;
			mem[0xA000 + 127] = 0x1F;
			cpu->DE = 0xFF00;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->DE = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x3B)	// DEC SP	3B
		{
			mem[0] = 0x3B;
			cpu->SP = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xABCC, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xCB3B)		// SRL E	CB 3B
		{
			mem[0] = 0xCB;
			mem[1] = 0x3B;
			cpu->DE = 0x001F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3B)		// SRL E,(IX+s)		DD CB ss 3B
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3B;
			mem[0xA000 + 127] = 0x1F;
			cpu->DE = 0x00FF;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->DE = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3B)		// SRL E,(IY+s)		FD CB ss 3B
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3B;
			mem[0xA000 + 127] = 0x1F;
			cpu->DE = 0x00FF;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->DE = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x3C)	// INC A	3C
		{
			mem[0] = 0x3C;
			cpu->AF = 0xFF02;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | PF | ZF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xCB3C)		// SRL H	CB 3C
		{
			mem[0] = 0xCB;
			mem[1] = 0x3C;
			cpu->HL = 0x1F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3C)		// SRL H,(IX+s)		DD CB ss 3C
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3C;
			mem[0xA000 + 127] = 0x1F;
			cpu->HL = 0xFF00;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->HL = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3C)		// SRL H,(IY+s)		FD CB ss 3C
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3C;
			mem[0xA000 + 127] = 0x1F;
			cpu->HL = 0xFF00;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->HL = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x3D)	// DEC A		3D
		{
			mem[0] = 0x3D;
			cpu->AF = 0x0086;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xCB3D)		// SRL L	CB 3D
		{
			mem[0] = 0xCB;
			mem[1] = 0x3D;
			cpu->HL = 0x001F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3D)		// SRL L,(IX+s)		DD CB ss 3D
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3D;
			mem[0xA000 + 127] = 0x1F;
			cpu->HL = 0x00FF;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->HL = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3D)		// SRL L,(IY+s)		FD CB ss 3D
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3D;
			mem[0xA000 + 127] = 0x1F;
			cpu->HL = 0x00FF;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->HL = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x3E)	// LD A,N	3E NN
		{
			mem[0] = 0x3E;
			mem[1] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xCC00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xCB3E)		// SRL (HL)		CB 3E
		{
			mem[0] = 0xCB;
			mem[1] = 0x3E;
			mem[0xA000] = 0x1F;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3E)		// SRL (IX+s)	DD CB ss 3E
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3E;
			mem[0xA000 + 127] = 0x1F;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3E)		// SRL (IY+s)	FD CB ss 3E
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3E;
			mem[0xA000 + 127] = 0x1F;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xCB3F)		// SRL A	CB 3F
		{
			mem[0] = 0xCB;
			mem[1] = 0x3F;
			cpu->AF = 0x1F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDDCB3F)		// SRL A,(IX+s)		DD CB ss 3F
		{
			mem[0] = 0xDD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3F;
			mem[0xA000 + 127] = 0x1F;
			cpu->AF = 0xFF00;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0xFDCB3F)		// SRL A,(IY+s)		FD CB ss 3F
		{
			mem[0] = 0xFD;
			mem[1] = 0xCB;
			mem[2] = 127;
			mem[3] = 0x3F;
			mem[0xA000 + 127] = 0x1F;
			cpu->AF = 0xFF00;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F3 | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x00, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x41)	// LD B,C	41
		{
			mem[0] = 0x41;
			cpu->BC = 0x00CD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCDCD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x42)	// LD B,D	42
		{
			mem[0] = 0x42;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED42)			// SBÑ HL,BC	ED 42
		{
			mem[0] = 0xED;
			mem[1] = 0x42;
			cpu->AF = 0x0001;
			cpu->HL = 0x0001;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(-1, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0001;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x43)	// LD B,E	43
		{
			mem[0] = 0x43;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED43)	// LD (NN),BC	ED 43 NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x43;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x44)	// LD B,H	44
		{
			mem[0] = 0x44;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD44)	// LD B,XH	DD 44
		{
			mem[0] = 0xDD;
			mem[1] = 0x44;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD44)	// LD B,YH	FD 44
		{
			mem[0] = 0xFD;
			mem[1] = 0x44;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x45)	// LD B,L	45
		{
			mem[0] = 0x45;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD45)	// LD B,XL	DD 45
		{
			mem[0] = 0xDD;
			mem[1] = 0x45;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD45)	// LD B,YL	FD 45
		{
			mem[0] = 0xFD;
			mem[1] = 0x45;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x46)	// LD B,(HL)	46
		{
			mem[0] = 0x46;
			mem[1] = 0xDD;
			cpu->BC = 0x00EE;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xDDEE);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD46)	// LD B,(IX+s)	DD 46 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x46;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD46)	// LD B,(IY+s)	FD 46 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x46;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x47)	// LD B,A	47
		{
			mem[0] = 0x47;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED47)	// LD I,A	ED 47
		{
			mem[0] = 0xED;
			mem[1] = 0x47;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IR == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x48)	// LD C,B	48
		{
			mem[0] = 0x48;
			cpu->BC = 0xAB00;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xABAB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x4A)	// LD C,D	4A
		{
			mem[0] = 0x4A;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED4A)			// ADÑ HL,BC	ED 4A
		{
			mem[0] = 0xED;
			mem[1] = 0x4A;
			cpu->AF = (NF | F5 | F3);
			cpu->HL = 0xFFFF;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | ZF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = (NF | F5 | F3 | CF);
			cpu->HL = 0xFFFF;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x0001, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x4B)	// LD C,E	4B
		{
			mem[0] = 0x4B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED4B)	// LD BC,(NN)	ED 4B NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x4B;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0x4C)	// LD C,H	4C
		{
			mem[0] = 0x4C;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD4C)	// LD C,XH	DD 4C
		{
			mem[0] = 0xDD;
			mem[1] = 0x4C;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD4C)	// LD C,YH	FD 4C
		{
			mem[0] = 0xFD;
			mem[1] = 0x4C;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x4D)	// LD C,L	4D
		{
			mem[0] = 0x4D;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD4D)	// LD C,XL	DD 4D
		{
			mem[0] = 0xDD;
			mem[1] = 0x4D;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD4D)	// LD C,YL	FD 4D
		{
			mem[0] = 0xFD;
			mem[1] = 0x4D;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x4E)	// LD C,(HL)	4E
		{
			mem[0] = 0x4E;
			mem[1] = 0xDD;
			cpu->BC = 0xEE00;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xEEDD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD4E)	// LD C,(IX+s)	DD 4E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x4E;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD4E)	// LD C,(IY+s)	FD 4E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x4E;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->BC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x4F)	// LD C,A	4F
		{
			mem[0] = 0x4F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED4F)	// LD R,A	ED 4F
		{
			mem[0] = 0xED;
			mem[1] = 0x4F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IR == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x50)	// LD D,B	50
		{
			mem[0] = 0x50;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x51)	// LD D,C	51
		{
			mem[0] = 0x51;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED52)			// SBÑ HL,DE	ED 52
		{
			mem[0] = 0xED;
			mem[1] = 0x52;
			cpu->AF = 0x0001;
			cpu->HL = 0x0001;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(-1, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x53)	// LD D,E	53
		{
			mem[0] = 0x53;
			cpu->DE = 0x00CD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCDCD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED53)	// LD (NN),DE	ED 53 NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x53;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x54)	// LD D,H	54
		{
			mem[0] = 0x54;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD54)	// LD D,XH	DD 54
		{
			mem[0] = 0xDD;
			mem[1] = 0x54;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD54)	// LD D,YH	FD 54
		{
			mem[0] = 0xFD;
			mem[1] = 0x54;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x55)	// LD D,L	55
		{
			mem[0] = 0x55;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD55)	// LD D,XL	DD 55
		{
			mem[0] = 0xDD;
			mem[1] = 0x55;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD55)	// LD D, YL	FD 55
		{
			mem[0] = 0xFD;
			mem[1] = 0x55;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x56)	// LD D,(HL)	56
		{
			mem[0] = 0x56;
			mem[1] = 0xDD;
			cpu->DE = 0x00EE;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xDDEE);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD56)	// LD D,(IX+s)	DD 56 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x56;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD56)	// LD D,(IY+s)	FD 56 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x56;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x57)	// LD D,A	57
		{
			mem[0] = 0x57;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED57)	// LD A,I	ED 57
		{
			mem[0] = 0xED;
			mem[1] = 0x57;
			cpu->AF = 0x1100;
			cpu->IR = 0x00CD;
			cpu->IFF2 = true;
			cpu->Step();
			Assert::IsTrue(HIBYTE(cpu->AF) == 0x00);
			Assert::IsTrue(LOBYTE(cpu->AF) == 0b01000100);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x58)	// LD E,B	58
		{
			mem[0] = 0x58;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x59)	// LD E,C	59
		{
			mem[0] = 0x59;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x5A)	// LD E,D	5A
		{
			mem[0] = 0x5A;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xABAB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED5A)			// ADÑ HL,DE	ED 5A
		{
			mem[0] = 0xED;
			mem[1] = 0x5A;
			cpu->AF = (NF | F5 | F3);
			cpu->HL = 0xFFFF;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | ZF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = (NF | F5 | F3 | CF);
			cpu->HL = 0xFFFF;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x0001, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xED5B)	// LD DE,(NN)	ED 5B NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x5B;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0x5C)	// LD E,H	5C
		{
			mem[0] = 0x5C;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD5C)	// LD E,XH	DD 5C
		{
			mem[0] = 0xDD;
			mem[1] = 0x5C;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD5C)	// LD E,YH	FD 5C
		{
			mem[0] = 0xFD;
			mem[1] = 0x5C;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x5D)	// LD E,L	5D
		{
			mem[0] = 0x5D;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD5D)	// LD E,XL	DD 5D
		{
			mem[0] = 0xDD;
			mem[1] = 0x5D;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD5D)	// LD E,YL	FD 5D
		{
			mem[0] = 0xFD;
			mem[1] = 0x5D;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x5E)	// LD E,(HL)	5E
		{
			mem[0] = 0x5E;
			mem[1] = 0xDD;
			cpu->DE = 0xEE00;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xEEDD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD5E)	// LD E,(IX+s)	DD 5E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x5E;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD5E)	// LD E,(IY+s)	FD 5E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x5E;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x5F)	// LD E,A	5F
		{
			mem[0] = 0x5F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED5F)	// LD A,R	ED 5F
		{
			mem[0] = 0xED;
			mem[1] = 0x5F;
			cpu->AF = 0x1100;
			cpu->IR = 0xAB00;
			cpu->IFF2 = true;
			cpu->Step();
			Assert::IsTrue(HIBYTE(cpu->AF) == 0x00);
			Assert::IsTrue(LOBYTE(cpu->AF) == 0b01000100);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x60)	// LD H,B	60
		{
			mem[0] = 0x60;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD60)	// LD XH,B	DD 60
		{
			mem[0] = 0xDD;
			mem[1] = 0x60;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD60)	// LD YH,B	FD 60
		{
			mem[0] = 0xFD;
			mem[1] = 0x60;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x61)	// LD H,C	61
		{
			mem[0] = 0x61;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD61)	// LD XH,C	DD 61
		{
			mem[0] = 0xDD;
			mem[1] = 0x61;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD61)	// LD YH,C	FD 61
		{
			mem[0] = 0xFD;
			mem[1] = 0x61;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x62)	// LD H,D	62
		{
			mem[0] = 0x62;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED62)			// SBÑ HL,HL	ED 62
		{
			mem[0] = 0xED;
			mem[1] = 0x62;
			cpu->AF = 0x0001;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(-1, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDD62)	// LD XH,D	DD 62
		{
			mem[0] = 0xDD;
			mem[1] = 0x62;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD62)	// LD YH,D	FD 62
		{
			mem[0] = 0xFD;
			mem[1] = 0x62;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x63)	// LD H,E	63
		{
			mem[0] = 0x63;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD63)	// LD XH,E	DD 63
		{
			mem[0] = 0xDD;
			mem[1] = 0x63;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD63)	// LD YH,E	FD 63
		{
			mem[0] = 0xFD;
			mem[1] = 0x63;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xCD00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xED63)	// LD (NN),HL	ED 63 NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x63;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x65)	// LD H,L	65
		{
			mem[0] = 0x65;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xCDCD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD65)	// LD XH,XL	DD 65
		{
			mem[0] = 0xDD;
			mem[1] = 0x65;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xCDCD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD65)	// LD YH,YL	FD 65
		{
			mem[0] = 0xFD;
			mem[1] = 0x65;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xCDCD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x66)	// LD H,(HL)	66
		{
			mem[0] = 0x66;
			mem[1] = 0xDD;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xDD01);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD66)	// LD H,(IX+s)	DD 66 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x66;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD66)	// LD H,(IY+s)	FD 66 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x66;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xDD00, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x67)	// LD H,A	67
		{
			mem[0] = 0x67;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD67)	// LD XH,A	DD 67
		{
			mem[0] = 0xDD;
			mem[1] = 0x67;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD67)	// LD YH,A	FD 67
		{
			mem[0] = 0xFD;
			mem[1] = 0x67;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xAB00);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x68)	// LD L,B	68
		{
			mem[0] = 0x68;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD68)	// LD XL,B	DD 68
		{
			mem[0] = 0xDD;
			mem[1] = 0x68;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD68)	// LD YL,B	FD 68
		{
			mem[0] = 0xFD;
			mem[1] = 0x68;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x69)	// LD L,C	69
		{
			mem[0] = 0x69;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD69)	// LD XL,C	DD 69
		{
			mem[0] = 0xDD;
			mem[1] = 0x69;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD69)	// LD YL,C	FD 69
		{
			mem[0] = 0xFD;
			mem[1] = 0x69;
			cpu->BC = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x6A)	// LD L,D	6A
		{
			mem[0] = 0x6A;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED6A)			// ADÑ HL,HL	ED 6A
		{
			mem[0] = 0xED;
			mem[1] = 0x6A;
			cpu->AF = (NF | F5 | F3);
			cpu->HL = 0x8000;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | ZF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = (NF | F5 | F3 | CF);
			cpu->HL = 0x8000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x0001, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDD6A)	// LD XL,D	DD 6A
		{
			mem[0] = 0xDD;
			mem[1] = 0x6A;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD6A)	// LD YL,D	FD 6A
		{
			mem[0] = 0xFD;
			mem[1] = 0x6A;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x6B)	// LD L,E	6B
		{
			mem[0] = 0x6B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD6B)	// LD XL,E	DD 6B
		{
			mem[0] = 0xDD;
			mem[1] = 0x6B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD6B)	// LD YL,E	FD 6B
		{
			mem[0] = 0xFD;
			mem[1] = 0x6B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00CD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xED6B)	// LD HL,(NN)	ED 6B NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x6B;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0x6C)	// LD L,H	6C
		{
			mem[0] = 0x6C;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0xABAB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD6C)	// LD XL,XH	DD 6C
		{
			mem[0] = 0xDD;
			mem[1] = 0x6C;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0xABAB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD6C)	// LD YL,YH	FD 6C
		{
			mem[0] = 0xFD;
			mem[1] = 0x6C;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0xABAB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x6E)	// LD L,(HL)	6E
		{
			mem[0] = 0x6E;
			mem[1] = 0xDD;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00DD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD6E)	// LD L,(IX+s)	DD 6E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x6E;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD6E)	// LD L,(IY+s)	FD 6E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x6E;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00DD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x6F)	// LD L,A	6F
		{
			mem[0] = 0x6F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD6F)	// LD XL,A	DD 6F
		{
			mem[0] = 0xDD;
			mem[1] = 0x6F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFD6F)	// LD YL,A	FD 6F
		{
			mem[0] = 0xFD;
			mem[1] = 0x6F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x00AB);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x70)	// LD (HL),B	70
		{
			mem[0] = 0x70;
			mem[0xABCD] = 0x00;
			cpu->BC = 0x1234;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD70)	// LD (IX+s),B	DD 70 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x70;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD70)	// LD (IY+s),B	FD 70 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x70;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x71)	// LD (HL),C	71
		{
			mem[0] = 0x71;
			mem[0xABCD] = 0x00;
			cpu->BC = 0x1234;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD71)	// LD (IX+s),C	DD 71 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x71;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD71)	// LD (IY+s),C	FD 71 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x71;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x72)	// LD (HL),D	72
		{
			mem[0] = 0x72;
			mem[0xABCD] = 0x00;
			cpu->DE = 0x1234;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xED72)			// SBÑ HL,SP	ED 72
		{
			mem[0] = 0xED;
			mem[1] = 0x72;
			cpu->AF = 0x0001;
			cpu->HL = 0x0001;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(-1, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDD72)	// LD (IX+s),D	DD 72 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x72;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD72)	// LD (IY+s),D	FD 72 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x72;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x73)	// LD (HL),E	73
		{
			mem[0] = 0x73;
			mem[0xABCD] = 0x00;
			cpu->DE = 0x1234;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD73)	// LD (IX+s),E	DD 73 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x73;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD73)	// LD (IY+s),E	FD 73 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x73;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xED73)	// LD (NN),SP	ED 73 NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x73;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0x00;
			mem[0xABCD + 1] = 0x00;
			cpu->SP = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x34 && mem[0xABCD + 1] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 4, L"PC wrong");
		}

		TEST_METHOD(Test0x74)	// LD (HL),H	74
		{
			mem[0] = 0x74;
			mem[0xABCD] = 0x00;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0xAB, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD74)	// LD (IX+s),H	DD 74 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x74;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD74)	// LD (IY+s),H	FD 74 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x74;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x75)	// LD (HL),L	75
		{
			mem[0] = 0x75;
			mem[0xABCD] = 0x00;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0xCD, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD75)	// LD (IX+s),L	DD 75 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x75;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD75)	// LD (IY+s),L	FD 75 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x75;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x34, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x77)	// LD (HL),A	77
		{
			mem[0] = 0x77;
			mem[0xABCD] = 0x00;
			cpu->AF = 0x12EE;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(mem[0xABCD] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD77)	// LD (IX+s),A	DD 77 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x77;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0x00;
			cpu->AF = 0x12EE;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD77)	// LD (IY+s),A	FD 77 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x77;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0x00;
			cpu->AF = 0x12EE;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 + 127] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0x00;
			cpu->PC = 0;
			cpu->Step();
			Assert::IsTrue(mem[0xA000 - 128] == 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x78)	// LD A,B	78
		{
			mem[0] = 0x78;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x1200);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x79)	// LD A,C	79
		{
			mem[0] = 0x79;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x7A)	// LD A,D	7A
		{
			mem[0] = 0x7A;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x1200);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED7A)			// ADÑ HL,SP	ED 7A
		{
			mem[0] = 0xED;
			mem[1] = 0x7A;
			cpu->AF = (NF | F5 | F3);
			cpu->HL = 0xFFFF;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | ZF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = (NF | F5 | F3 | CF);
			cpu->HL = 0xFFFF;
			cpu->SP = 0x0001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x0001, cpu->HL, L"Result wrong");
			Assert::AreEqual<unsigned __int8>(PF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x7B)	// LD A,E	7B
		{
			mem[0] = 0x7B;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xED7B)	// LD SP,(NN)	ED 7B NN NN
		{
			mem[0] = 0xED;
			mem[1] = 0x7B;
			mem[2] = 0xCD;
			mem[3] = 0xAB;
			mem[0xABCD + 0] = 0xEF;
			mem[0xABCD + 1] = 0xFE;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xFEEF);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0x7C)	// LD A,H	7C
		{
			mem[0] = 0x7C;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x1200);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD7C)	// LD A,XH	DD 7C
		{
			mem[0] = 0xDD;
			mem[1] = 0x7C;
			cpu->IX = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x1200);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x7D)	// LD A,L	7D
		{
			mem[0] = 0x7D;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x7E)	// LD A,(HL)	7E
		{
			mem[0] = 0x7E;
			mem[1] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDD7E)	// LD A,(IX+s)	DD 7E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x7E;
			mem[2] = (__int8)127;
			cpu->IX = 0xA000;
			mem[cpu->IX + 127] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IX - 128] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD7E)	// LD A,(IY+s)	FD 7E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x7E;
			mem[2] = (__int8)127;
			cpu->IY = 0xA000;
			mem[cpu->IY + 127] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = (__int8)-128;
			mem[cpu->IY - 128] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xDD7D)	// LD A,XL	DD 7D
		{
			mem[0] = 0xDD;
			mem[1] = 0x7D;
			cpu->IX = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x80)	// ADD A,B		80
		{
			mem[0] = 0x80;
			cpu->AF = 0xC886;
			cpu->BC = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x81)	// ADD A,C		81
		{
			mem[0] = 0x81;
			cpu->AF = 0xC886;
			cpu->BC = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x82)	// ADD A,D		82
		{
			mem[0] = 0x82;
			cpu->AF = 0xC886;
			cpu->DE = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x83)	// ADD A,E		83
		{
			mem[0] = 0x83;
			cpu->AF = 0xC886;
			cpu->DE = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x84)	// ADD A,H		84
		{
			mem[0] = 0x84;
			cpu->AF = 0xC886;
			cpu->HL = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD84)	// ADD A,XH		DD 84
		{
			mem[0] = 0xDD;
			mem[1] = 0x84;
			cpu->AF = 0xC886;
			cpu->IX = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD84)	// ADD A,YH		FD 84
		{
			mem[0] = 0xFD;
			mem[1] = 0x84;
			cpu->AF = 0xC886;
			cpu->IY = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x85)	// ADD A,L		85
		{
			mem[0] = 0x85;
			cpu->AF = 0xC886;
			cpu->HL = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD85)	// ADD A,XL		DD 85
		{
			mem[0] = 0xDD;
			mem[1] = 0x85;
			cpu->AF = 0xC886;
			cpu->IX = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD85)	// ADD A,YL		FD 85
		{
			mem[0] = 0xFD;
			mem[1] = 0x85;
			cpu->AF = 0xC886;
			cpu->IY = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x86)	// ADD A,(HL)	86
		{
			mem[0] = 0x86;
			mem[0xA000] = 0xC8;
			cpu->AF = 0xC886;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD86)	// ADD A,(IX+s)		DD 86 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x86;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD86)	// ADD A,(IY+s)		FD 86 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x86;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x87)	// ADD A,A		87
		{
			mem[0] = 0x87;
			cpu->AF = 0xC886;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x88)	// ADC A,B		88
		{
			mem[0] = 0x88;
			cpu->AF = 0xC886;
			cpu->BC = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x89)	// ADC A,C		89
		{
			mem[0] = 0x89;
			cpu->AF = 0xC886;
			cpu->BC = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x8A)	// ADC A,D		8A
		{
			mem[0] = 0x8A;
			cpu->AF = 0xC886;
			cpu->DE = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x8B)	// ADC A,E		8B
		{
			mem[0] = 0x8B;
			cpu->AF = 0xC886;
			cpu->DE = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->DE = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x8C)	// ADC A,H		8C
		{
			mem[0] = 0x8C;
			cpu->AF = 0xC886;
			cpu->HL = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD8C)	// ADC A,XH		DD 8C
		{
			mem[0] = 0xDD;
			mem[1] = 0x8C;
			cpu->AF = 0xC886;
			cpu->IX = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD8C)	// ADC A,YH		FD 8C
		{
			mem[0] = 0xFD;
			mem[1] = 0x8C;
			cpu->AF = 0xC886;
			cpu->IY = 0xC800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x8D)	// ADC A,L		8D
		{
			mem[0] = 0x8D;
			cpu->AF = 0xC886;
			cpu->HL = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD8D)	// ADC A,XL		DD 8D
		{
			mem[0] = 0xDD;
			mem[1] = 0x8D;
			cpu->AF = 0xC886;
			cpu->IX = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD8D)	// ADC A,YL		FD 8D
		{
			mem[0] = 0xFD;
			mem[1] = 0x8D;
			cpu->AF = 0xC886;
			cpu->IY = 0x00C8;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x8E)	// ADC A,(HL)	8E
		{
			mem[0] = 0x8E;
			mem[0xA000] = 0xC8;
			cpu->AF = 0xC886;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD8E)	// ADC A,(IX+s)		DD 8E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x8E;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xC8;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD8E)	// ADC A,(IY+s)		FD 8E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x8E;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xC8;
			cpu->AF = 0xC886;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0xC8;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x8F)	// ADC A,A		8F
		{
			mem[0] = 0x8F;
			cpu->AF = 0xC886;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x90)	// SUB B		90
		{
			mem[0] = 0x90;
			cpu->AF = 0xFF86;
			cpu->BC = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->BC = 0x0200;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x91)	// SUB C		91
		{
			mem[0] = 0x91;
			cpu->AF = 0xFF86;
			cpu->BC = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->BC = 0x0002;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x92)	// SUB D		92
		{
			mem[0] = 0x92;
			cpu->AF = 0xFF86;
			cpu->DE = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->DE = 0x0200;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x93)	// SUB E		93
		{
			mem[0] = 0x93;
			cpu->AF = 0xFF86;
			cpu->DE = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->DE = 0x0002;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x94)	// SUB H		94
		{
			mem[0] = 0x94;
			cpu->AF = 0xFF86;
			cpu->HL = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->HL = 0x0200;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD94)		// SUB XH		DD 94
		{
			mem[0] = 0xDD;
			mem[1] = 0x94;
			cpu->AF = 0xFF86;
			cpu->IX = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->IX = 0x0200;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD94)		// SUB YH		FD 94
		{
			mem[0] = 0xFD;
			mem[1] = 0x94;
			cpu->AF = 0xFF86;
			cpu->IY = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->IY = 0x0200;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x95)	// SUB L		95
		{
			mem[0] = 0x95;
			cpu->AF = 0xFF86;
			cpu->HL = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->HL = 0x0002;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD95)		// SUB XL		DD 95
		{
			mem[0] = 0xDD;
			mem[1] = 0x95;
			cpu->AF = 0xFF86;
			cpu->IX = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->IX = 0x0002;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD95)		// SUB YL		FD 95
		{
			mem[0] = 0xFD;
			mem[1] = 0x95;
			cpu->AF = 0xFF86;
			cpu->IY = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0186;
			cpu->IY = 0x0002;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | CF | HF | F5 | F3 | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x96)		// SUB (HL)		96
		{
			mem[0] = 0x96;
			mem[0xA000] = 0xFF;
			cpu->AF = 0xFF86;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD96)		// SUB (IX+s)		DD 96 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x96;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xFF;
			cpu->AF = 0xFF86;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0086;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | NF | HF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD96)		// SUB (IY+s)		FD 96 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x96;
			mem[2] = 127;
			mem[0xA000 + 127] = 0xFF;
			cpu->AF = 0xFF86;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0086;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | NF | HF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x97)	// SUB A		97
		{
			mem[0] = 0x97;
			cpu->AF = 0xFF86;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x98)	// SBC A,B		98
		{
			mem[0] = 0x98;
			cpu->AF = 0x0101;
			cpu->BC = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x99)	// SBC A,C		99
		{
			mem[0] = 0x99;
			cpu->AF = 0x0101;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x9A)	// SBC A,D		9A
		{
			mem[0] = 0x9A;
			cpu->AF = 0x0101;
			cpu->DE = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x9B)	// SBC A,E		9B
		{
			mem[0] = 0x9B;
			cpu->AF = 0x0101;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0x9C)	// SBC A,H		9C
		{
			mem[0] = 0x9C;
			cpu->AF = 0x0101;
			cpu->HL = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD9C)	// SBC A,XH		DD 9C
		{
			mem[0] = 0xDD;
			mem[1] = 0x9C;
			cpu->AF = 0x0101;
			cpu->IX = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD9C)	// SBC A,YH		FD 9C
		{
			mem[0] = 0xFD;
			mem[1] = 0x9C;
			cpu->AF = 0x0101;
			cpu->IY = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x9D)	// SBC A,L		9D
		{
			mem[0] = 0x9D;
			cpu->AF = 0x0101;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD9D)	// SBC A,XL		DD 9D
		{
			mem[0] = 0xDD;
			mem[1] = 0x9D;
			cpu->AF = 0x0101;
			cpu->IX = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFD9D)	// SBC A,YL		FD 9D
		{
			mem[0] = 0xFD;
			mem[1] = 0x9D;
			cpu->AF = 0x0101;
			cpu->IY = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0x9E)	// SBC A,(HL)	9E
		{
			mem[0] = 0x9E;
			mem[0xA000] = 0x01;
			cpu->AF = 0x0101;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDD9E)		// SBC A,(IX+s)		DD 9E ss
		{
			mem[0] = 0xDD;
			mem[1] = 0x9E;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x01;
			cpu->AF = 0x0100;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0101;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFD9E)		// SBC A,(IY+s)		FD 9E ss
		{
			mem[0] = 0xFD;
			mem[1] = 0x9E;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x01;
			cpu->AF = 0x0100;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0101;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0x9F)	// SBC A,A		9F
		{
			mem[0] = 0x9F;
			cpu->AF = 0x0101;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA0)	// AND B	A0
		{
			mem[0] = 0xA0;
			cpu->AF = 0xFF00;
			cpu->BC = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->BC = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA1)	// AND C	A1
		{
			mem[0] = 0xA1;
			cpu->AF = 0xFF00;
			cpu->BC = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->BC = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA2)	// AND D	A2
		{
			mem[0] = 0xA2;
			cpu->AF = 0xFF00;
			cpu->DE = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->DE = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA3)	// AND E	A3
		{
			mem[0] = 0xA3;
			cpu->AF = 0xFF00;
			cpu->DE = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->DE = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA4)	// AND H	A4
		{
			mem[0] = 0xA4;
			cpu->AF = 0xFF00;
			cpu->HL = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->HL = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDA4)		// AND XH	DD A4
		{
			mem[0] = 0xDD;
			mem[1] = 0xA4;
			cpu->AF = 0xFF00;
			cpu->IX = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IX = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDA4)		// AND YH	FD A4
		{
			mem[0] = 0xFD;
			mem[1] = 0xA4;
			cpu->AF = 0xFF00;
			cpu->IY = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IY = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xA5)	// AND L	A5
		{
			mem[0] = 0xA5;
			cpu->AF = 0xFF00;
			cpu->HL = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->HL = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDA5)		// AND XL	DD A5
		{
			mem[0] = 0xDD;
			mem[1] = 0xA5;
			cpu->AF = 0xFF00;
			cpu->IX = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IX = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDA5)		// AND YL	FD A5
		{
			mem[0] = 0xFD;
			mem[1] = 0xA5;
			cpu->AF = 0xFF00;
			cpu->IY = 0x00FF;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IY = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xA6)	// AND (HL)		A6
		{
			mem[0] = 0xA6;
			cpu->AF = 0xFF00;
			mem[0xA000] = 0xFF;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			mem[0xA000] = 0x0F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDA6)		// AND (IX+s)	DD A6 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0xA6;
			mem[2] = 127;
			cpu->AF = 0xFF00;
			mem[0xA000 + 127] = 0xFF;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			cpu->AF = 0xF000;
			mem[0xA000 - 128] = 0x0F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFDA6)		// AND (IY+s)	FD A6 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0xA6;
			mem[2] = 127;
			cpu->AF = 0xFF00;
			mem[0xA000 + 127] = 0xFF;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			cpu->AF = 0xF000;
			mem[0xA000 - 128] = 0x0F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xA7)	// AND A	A7
		{
			mem[0] = 0xA7;
			cpu->AF = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA8)	// XOR B	A8
		{
			mem[0] = 0xA8;
			cpu->AF = 0xF000;
			cpu->BC = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->BC = 0x7F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xA9)	// XOR C	A9
		{
			mem[0] = 0xA9;
			cpu->AF = 0xF000;
			cpu->BC = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->BC = 0x007F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xAA)	// XOR D	AA
		{
			mem[0] = 0xAA;
			cpu->AF = 0xF000;
			cpu->DE = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->DE = 0x7F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xAB)	// XOR E	AB
		{
			mem[0] = 0xAB;
			cpu->AF = 0xF000;
			cpu->DE = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->DE = 0x007F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xAC)	// XOR H	AC
		{
			mem[0] = 0xAC;
			cpu->AF = 0xF000;
			cpu->HL = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->HL = 0x7F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDAC)	// XOR XH	DD AC
		{
			mem[0] = 0xDD;
			mem[1] = 0xAC;
			cpu->AF = 0xF000;
			cpu->IX = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IX = 0x7F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDAC)	// XOR YH	FD AC
		{
			mem[0] = 0xFD;
			mem[1] = 0xAC;
			cpu->AF = 0xF000;
			cpu->IY = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IY = 0x7F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xAD)	// XOR L	AD
		{
			mem[0] = 0xAD;
			cpu->AF = 0xF000;
			cpu->HL = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->HL = 0x007F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDAD)	// XOR XL	DD AD
		{
			mem[0] = 0xDD;
			mem[1] = 0xAD;
			cpu->AF = 0xF000;
			cpu->IX = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IX = 0x007F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDAD)	// XOR YL	FD AD
		{
			mem[0] = 0xFD;
			mem[1] = 0xAD;
			cpu->AF = 0xF000;
			cpu->IY = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			cpu->IY = 0x007F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xAE)	// XOR (HL)		AE
		{
			mem[0] = 0xAE;
			mem[0xA000] = 0x0F;
			cpu->AF = 0xF000;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			mem[0xA000] = 0x7F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x7F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDAE)	// XOR (IX+s)	DD AE ss
		{
			mem[0] = 0xDD;
			mem[1] = 0xAE;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x0F;
			cpu->AF = 0xF000;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x7F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x7F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFDAE)	// XOR (IY+s)	FD AE ss
		{
			mem[0] = 0xFD;
			mem[1] = 0xAE;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x0F;
			cpu->AF = 0xF000;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x7F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x7F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xAF)	// XOR A	AF
		{
			mem[0] = 0xAF;
			cpu->AF = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB0)	// OR B		B0
		{
			mem[0] = 0xB0;
			cpu->AF = 0xF000;
			cpu->BC = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->BC = 0x8F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB1)	// OR C		B1
		{
			mem[0] = 0xB1;
			cpu->AF = 0xF000;
			cpu->BC = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->BC = 0x008F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB2)	// OR D		B2
		{
			mem[0] = 0xB2;
			cpu->AF = 0xF000;
			cpu->DE = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->DE = 0x8F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB3)	// OR E		B3
		{
			mem[0] = 0xB3;
			cpu->AF = 0xF000;
			cpu->DE = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->DE = 0x008F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB4)	// OR H		B4
		{
			mem[0] = 0xB4;
			cpu->AF = 0xF000;
			cpu->HL = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x8F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDB4)	// OR XH	DD B4
		{
			mem[0] = 0xDD;
			mem[1] = 0xB4;
			cpu->AF = 0xF000;
			cpu->IX = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IX = 0x8F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDB4)	// OR YH	FD B4
		{
			mem[0] = 0xFD;
			mem[1] = 0xB4;
			cpu->AF = 0xF000;
			cpu->IY = 0x0F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IY = 0x8F00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xB5)	// OR L		B5
		{
			mem[0] = 0xB5;
			cpu->AF = 0xF000;
			cpu->HL = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x008F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDB5)	// OR XL	DD B5
		{
			mem[0] = 0xDD;
			mem[1] = 0xB5;
			cpu->AF = 0xF000;
			cpu->IX = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IX = 0x008F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDB5)	// OR YL	FD B5
		{
			mem[0] = 0xFD;
			mem[1] = 0xB5;
			cpu->AF = 0xF000;
			cpu->IY = 0x000F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IY = 0x008F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xB6)	// OR (HL)	B6
		{
			mem[0] = 0xB6;
			mem[0xA000] = 0x0F;
			cpu->AF = 0xF000;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			mem[0xA000] = 0x8F;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDB6)	// OR (IX+s)	DD B6 ss
		{
			mem[0] = 0xDD;
			mem[1] = 0xB6;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x0F;
			cpu->AF = 0xF000;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x8F;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFDB6)	// OR (IY+s)	FD B6 ss
		{
			mem[0] = 0xFD;
			mem[1] = 0xB6;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x0F;
			cpu->AF = 0xF000;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x8F;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xB7)	// OR A		B7
		{
			mem[0] = 0xB7;
			cpu->AF = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB8)	// CP B		B8
		{
			mem[0] = 0xB8;
			cpu->AF = 0x2800;
			cpu->BC = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->BC = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, HIBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xB9)	// CP C		B9
		{
			mem[0] = 0xB9;
			cpu->AF = 0x2800;
			cpu->BC = 0x0028;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->BC = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, LOBYTE(cpu->BC), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xBA)	// CP D		BA
		{
			mem[0] = 0xBA;
			cpu->AF = 0x2800;
			cpu->DE = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->DE = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, HIBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xBB)	// CP E		BB
		{
			mem[0] = 0xBB;
			cpu->AF = 0x2800;
			cpu->DE = 0x0028;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->DE = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, LOBYTE(cpu->DE), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xBC)	// CP H		BC
		{
			mem[0] = 0xBC;
			cpu->AF = 0x2800;
			cpu->HL = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, HIBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDBC)		// CP XH		DD BC
		{
			mem[0] = 0xDD;
			mem[1] = 0xBC;
			cpu->AF = 0x2800;
			cpu->IX = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IX = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, HIBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDBC)		// CP YH		FD BC
		{
			mem[0] = 0xFD;
			mem[1] = 0xBC;
			cpu->AF = 0x2800;
			cpu->IY = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IY = 0x0100;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, HIBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xBD)	// CP L		BD
		{
			mem[0] = 0xBD;
			cpu->AF = 0x2800;
			cpu->HL = 0x0028;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->HL = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, LOBYTE(cpu->HL), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDBD)		// CP XL	DD BD
		{
			mem[0] = 0xDD;
			mem[1] = 0xBD;
			cpu->AF = 0x2800;
			cpu->IX = 0x0028;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IX = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, LOBYTE(cpu->IX), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDBD)		// CP YL	FD BD
		{
			mem[0] = 0xFD;
			mem[1] = 0xBD;
			cpu->AF = 0x2800;
			cpu->IY = 0x0028;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0x0000;
			cpu->IY = 0x0001;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, LOBYTE(cpu->IY), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xBE)	// CP (HL)		BE
		{
			mem[0] = 0xBE;
			mem[0xA000] = 0x28;
			cpu->AF = 0x2800;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
			cpu->PC = 0;
			mem[0xA000] = 0x01;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, mem[0xA000], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDBE)		// CP (IX+s)	DD BE ss
		{
			mem[0] = 0xDD;
			mem[1] = 0xBE;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x28;
			cpu->AF = 0x2800;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xFDBE)		// CP (IY+s)	FD BE ss
		{
			mem[0] = 0xFD;
			mem[1] = 0xBE;
			mem[2] = 127;
			mem[0xA000 + 127] = 0x28;
			cpu->AF = 0x2800;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, mem[0xA000 + 127], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
			cpu->PC = 0;
			mem[2] = -128;
			mem[0xA000 - 128] = 0x01;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, mem[0xA000 - 128], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 3, L"PC wrong");
		}

		TEST_METHOD(Test0xBF)	// CP A		BF
		{
			mem[0] = 0xBF;
			cpu->AF = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | F5 | F3 | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xC0)	// RET NZ		C0
		{
			mem[0] = 0xC0;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC1)	// POP BC		C1
		{
			mem[0] = 0xC1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->BC, L"BC wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xC2)	// JP NZ,NN		C2 NN NN
		{
			mem[0] = 0xC2;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC3)	// JP NN	C3 NN NN
		{
			mem[0] = 0xC3;
			mem[1] = 0xCD;
			mem[2] = 0xAB;
			cpu->Step();
			Assert::IsTrue(cpu->PC == 0xABCD, L"PC wrong");
		}

		TEST_METHOD(Test0xC4)	// CALL NZ,NN		C4 NN NN
		{
			mem[0] = 0xC4;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC5)	// PUSH BC		C5
		{
			mem[0] = 0xC5;
			cpu->SP = 0x1FFF;
			cpu->BC = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int16>(0x1234, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(1, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC6)	// ADD A,N		C6 NN
		{
			mem[0] = 0xC6;
			mem[1] = 0xC8;
			cpu->AF = 0xC886;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xC7)	// RST #0		C7
		{
			mem[0] = 0xC7;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0000, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC8)	// RET Z		C8
		{
			mem[0] = 0xC8;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xC9)	// RET		C9
		{
			mem[0] = 0xC9;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xCA)	// JP Z,NN		CA NN NN
		{
			mem[0] = 0xCA;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xCC)	// CALL Z,NN		CC NN NN
		{
			mem[0] = 0xCC;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b01000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xCD)	// CALL NN		CD NN NN
		{
			mem[0] = 0xCD;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xCE)	// ADC A,N		CE NN
		{
			mem[0] = 0xCE;
			mem[1] = 0xC8;
			cpu->AF = 0xC886;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x90, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xC887;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x91, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(CF | HF | SF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xCF)	// RST #8		CF
		{
			mem[0] = 0xCF;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0008, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD0)	// RET NC		D0
		{
			mem[0] = 0xD0;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD1)	// POP DE		D1
		{
			mem[0] = 0xD1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->DE, L"DE wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xD2)	// JP NC,NN		D2 NN NN
		{
			mem[0] = 0xD2;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD4)	// CALL NC,NN		D4 NN NN
		{
			mem[0] = 0xD4;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD5)	// PUSH DE		D5
		{
			mem[0] = 0xD5;
			cpu->SP = 0x1FFF;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFD], 0x34, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFE], 0x12, L"Memory wrong");
			Assert::AreEqual<__int16>(1, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD6)	// SUB N		D6 NN
		{
			mem[0] = 0xD6;
			mem[1] = 0xFF;
			cpu->AF = 0xFF86;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF | VF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xD7)	// RST #10		D7
		{
			mem[0] = 0xD7;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0010, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD8)	// RET C		D8
		{
			mem[0] = 0xD8;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xD9)	// EXX D9
		{
			mem[0] = 0xD9;
			cpu->AF = 0x1111;
			cpu->BC = 0x2222;
			cpu->DE = 0x3333;
			cpu->HL = 0x4444;
			cpu->_AF = 0x5555;
			cpu->_BC = 0x6666;
			cpu->_DE = 0x7777;
			cpu->_HL = 0x8888;
			cpu->Step();
			Assert::IsTrue(cpu->_AF == 0x1111 && cpu->_BC == 0x2222 && cpu->_DE == 0x3333 && cpu->_HL == 0x4444);
			Assert::IsTrue(cpu->AF == 0x5555 && cpu->BC == 0x6666 && cpu->DE == 0x7777 && cpu->HL == 0x8888);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDA)	// JP C,NN		DA NN NN
		{
			mem[0] = 0xDA;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xDC)	// CALL C,NN		DC NN NN
		{
			mem[0] = 0xDC;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b00000001;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111110;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xDE)	// SBC A,N		DE NN
		{
			mem[0] = 0xDE;
			mem[1] = 0x01;
			cpu->AF = 0x0101;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | NF | VF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xDF)	// RST #18		DF
		{
			mem[0] = 0xDF;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0018, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE0)	// RET PO		E0
		{
			mem[0] = 0xE0;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE1)	// POP HL		E1
		{
			mem[0] = 0xE1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->HL, L"HL wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDE1)	// POP IX		DD E1
		{
			mem[0] = 0xDD;
			mem[1] = 0xE1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->IX, L"IX wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDE1)	// POP IY		FD E1
		{
			mem[0] = 0xFD;
			mem[1] = 0xE1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->IY, L"IY wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xE2)	// JP PO,NN		E2 NN NN
		{
			mem[0] = 0xE2;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE3)	// EX (SP),HL	E3
		{
			mem[0] = 0xE3;
			mem[0xA000] = 0x34;
			mem[0xA001] = 0x12;
			cpu->SP = 0xA000;
			cpu->HL = 0x5678;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x1234 && mem[0xA000] == 0x78 && mem[0xA001] == 0x56, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xDDE3)	// EX (SP),IX	DD E3
		{
			mem[0] = 0xDD;
			mem[1] = 0xE3;
			mem[0xA000] = 0x34;
			mem[0xA001] = 0x12;
			cpu->SP = 0xA000;
			cpu->IX = 0x5678;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x1234 && mem[0xA000] == 0x78 && mem[0xA001] == 0x56, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFDE3)	// EX (SP),IY	FD E3
		{
			mem[0] = 0xFD;
			mem[1] = 0xE3;
			mem[0xA000] = 0x34;
			mem[0xA001] = 0x12;
			cpu->SP = 0xA000;
			cpu->IY = 0x5678;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x1234 && mem[0xA000] == 0x78 && mem[0xA001] == 0x56, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xE4)	// CALL PO,NN		E4 NN NN
		{
			mem[0] = 0xE4;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE5)	// PUSH HL		E5
		{
			mem[0] = 0xE5;
			cpu->SP = 0x1FFF;
			cpu->HL = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFD], 0x34, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFE], 0x12, L"Memory wrong");
			Assert::AreEqual<__int16>(1, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xDDE5)	// PUSH IX		DD E5
		{
			mem[0] = 0xDD;
			mem[1] = 0xE5;
			cpu->SP = 0x1FFF;
			cpu->IX = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFD], 0x34, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFE], 0x12, L"Memory wrong");
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xFDE5)	// PUSH IY		FD E5
		{
			mem[0] = 0xFD;
			mem[1] = 0xE5;
			cpu->SP = 0x1FFF;
			cpu->IY = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFD], 0x34, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFE], 0x12, L"Memory wrong");
			Assert::AreEqual<__int16>(2, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE6)	// AND N	E6 NN
		{
			mem[0] = 0xE6;
			mem[1] = 0xFF;
			cpu->AF = 0xFF00;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0xFF, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | HF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0xF000;
			mem[1] = 0x0F;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | HF | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xE7)	// RST #20		E7
		{
			mem[0] = 0xE7;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0020, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE8)	// RET PE		E8
		{
			mem[0] = 0xE8;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xE9)	// JP (HL)	E9
		{
			mem[0] = 0xE9;
			cpu->HL = 0xA000;
			cpu->Step();
			Assert::IsTrue(cpu->PC == 0xA000, L"PC wrong");
		}

		TEST_METHOD(Test0xDDE9)	// JP (IX)	DD E9
		{
			mem[0] = 0xDD;
			mem[1] = 0xE9;
			cpu->IX = 0xA000;
			cpu->Step();
			Assert::IsTrue(cpu->PC == 0xA000, L"PC wrong");
		}

		TEST_METHOD(Test0xFDE9)	// JP (IY)	FD E9
		{
			mem[0] = 0xFD;
			mem[1] = 0xE9;
			cpu->IY = 0xA000;
			cpu->Step();
			Assert::IsTrue(cpu->PC == 0xA000, L"PC wrong");
		}

		TEST_METHOD(Test0xEA)	// JP PE,NN		EA NN NN
		{
			mem[0] = 0xEA;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xEB)	// EX DE,HL
		{
			mem[0] = 0xEB;
			cpu->DE = 0x1111;
			cpu->HL = 0x2222;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x2222 && cpu->HL == 0x1111);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xEC)	// CALL PE,NN		EC NN NN
		{
			mem[0] = 0xEC;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b00000100;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b11111011;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xEE)	// XOR N	EE NN
		{
			mem[0] = 0xEE;
			mem[1] = 0x0F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			mem[1] = 0x7F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x7F, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xEF)	// RST #28		EF
		{
			mem[0] = 0xEF;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0028, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF0)	// RET P		F0
		{
			mem[0] = 0xF0;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF1)	// POP AF		F1
		{
			mem[0] = 0xF1;
			cpu->SP = 0x1FFD;
			mem[0x1FFD] = 0x34;
			mem[0x1FFE] = 0x12;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->AF, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xF2)	// JP P,NN		F2 NN NN
		{
			mem[0] = 0xF2;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF4)	// CALL P,NN		F4 NN NN
		{
			mem[0] = 0xF4;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF5)	// PUSH AF		F5
		{
			mem[0] = 0xF5;
			cpu->SP = 0x1FFF;
			cpu->AF = 0x1234;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFD], 0x34, L"Memory wrong");
			Assert::AreEqual<__int8>(mem[0x1FFE], 0x12, L"Memory wrong");
			Assert::IsTrue(cpu->PC == 1, L"PC wrong");
		}

		TEST_METHOD(Test0xF6)	// OR N		F6 NN
		{
			mem[0] = 0xF6;
			mem[1] = 0x0F;
			cpu->AF = 0xF000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0xFF, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x0F, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | PF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			mem[1] = 0x8F;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x8F, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x8F, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F3, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xF7)	// RST #30		F7
		{
			mem[0] = 0xF7;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0030, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF8)	// RET M		F8
		{
			mem[0] = 0xF8;
			cpu->SP = 0x1FFD;
			MEM16(mem[cpu->SP]) = 0x1234;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFF, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xF9)	// LD SP,HL	F9
		{
			mem[0] = 0xF9;
			cpu->SP = 0x0000;
			cpu->HL = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xABCD);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0xDDF9)	// LD SP,IX	DD F9
		{
			mem[0] = 0xDD;
			mem[1] = 0xF9;
			cpu->SP = 0x0000;
			cpu->IX = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xABCD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFDF9)	// LD SP,IY	FD F9
		{
			mem[0] = 0xFD;
			mem[1] = 0xF9;
			cpu->SP = 0x0000;
			cpu->IY = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0xABCD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0xFA)	// JP M,NN		FA NN NN
		{
			mem[0] = 0xFA;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(3, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xFC)	// CALL M,NN		FC NN NN
		{
			mem[0] = 0xFC;
			MEM16(mem[1]) = 0x1234;
			cpu->SP = 0x1FFF;
			cpu->AF = 0b10000000;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, MEM16(mem[0x1FFD]), L"Stack wrong");
			Assert::AreEqual<__int16>(0x1234, cpu->PC, L"PC wrong");
			cpu->PC = 0;
			cpu->AF = 0b01111111;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0003, cpu->PC, L"PC wrong");
		}

		TEST_METHOD(Test0xFE)	// CP N		FE NN
		{
			mem[0] = 0xFE;
			mem[1] = 0x28;
			cpu->AF = 0x2800;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x28, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x28, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(ZF | NF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
			cpu->PC = 0;
			mem[1] = 0x01;
			cpu->AF = 0x0000;
			cpu->Step();
			Assert::AreEqual<unsigned __int8>(0x00, HIBYTE(cpu->AF), L"Result wrong");
			Assert::AreEqual<unsigned __int8>(0x01, mem[1], L"Result wrong");
			Assert::AreEqual<unsigned __int8>(SF | F5 | F3 | HF | VF | NF | CF, LOBYTE(cpu->AF), L"Flags wrong");
			Assert::IsTrue(cpu->PC == 2, L"PC wrong");
		}

		TEST_METHOD(Test0xFF)	// RST #38		FF
		{
			mem[0] = 0xFF;
			cpu->SP = 0x1FFF;
			cpu->Step();
			Assert::AreEqual<__int16>(0x1FFD, cpu->SP, L"SP wrong");
			Assert::AreEqual<__int16>(0x0001, MEM16(mem[0x1FFD]), L"Memory wrong");
			Assert::AreEqual<__int16>(0x0038, cpu->PC, L"PC wrong");
		}

	};
}