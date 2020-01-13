#include "CppUnitTest.h"
#include "../ZxEmu/CPU.h"
#include "../ZxEmu/EmuModel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZxEmuTest
{		
	unsigned __int8 mem[10];
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

		TEST_METHOD(Test0x01)	// LD BC,NN 01 NN NN
		{
			mem[0] = 0x01;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x06)	// LD B,N 06 NN
		{
			mem[0] = 0x06;
			mem[1] = 0x12;
			cpu->BC = 0x0034;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x08)	// EX AF, A'F' 08
		{
			mem[0] = 0x08;
			cpu->AF = 0x1234;
			cpu->_AF = 0x5678;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x5678);
			Assert::IsTrue(cpu->_AF == 0x1234);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x0A)	// LD A,(BC)
		{
			mem[0] = 0x0A;
			mem[1] = 0x00;
			mem[2] = 0xDD;
			cpu->AF = 0x00EE;
			cpu->BC = 0x0002;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x0E)	// LD C,N	0E NN
		{
			mem[0] = 0x0E;
			mem[1] = 0xDD;
			cpu->BC = 0xCC00;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCCDD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x11)	// LD DE,NN		11 NN NN
		{
			mem[0] = 0x11;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->DE = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x16)	// LD D,N	16 NN
		{
			mem[0] = 0x16;
			mem[1] = 0x12;
			cpu->DE = 0x0034;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x1234);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x1E)	// LD E,N	1E NN
		{
			mem[0] = 0x1E;
			mem[1] = 0xDD;
			cpu->DE = 0xCC00;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCCDD);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x21)	// LD HL, NN	21 NN NN
		{
			mem[0] = 0x21;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->HL = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->HL == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0xDD21)	// LD IX, NN	DD 21 NN NN
		{
			mem[0] = 0xDD;
			mem[1] = 0x21;
			mem[2] = 0x34;
			mem[3] = 0x12;
			cpu->IX = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->IX == 0x1234);
			Assert::IsTrue(cpu->PC == 4);
		}

		TEST_METHOD(Test0xFD21)	// LD IY, NN	FD 21 NN NN
		{
			mem[0] = 0xFD;
			mem[1] = 0x21;
			mem[2] = 0x34;
			mem[3] = 0x12;
			cpu->IY = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->IY == 0x1234);
			Assert::IsTrue(cpu->PC == 4);
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

		TEST_METHOD(Test0x31)	// LD SP,NN	31 NN NN
		{
			mem[0] = 0x31;
			mem[1] = 0x34;
			mem[2] = 0x12;
			cpu->Step();
			Assert::IsTrue(cpu->SP == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x3E)	// LD A,N	3E NN
		{
			mem[0] = 0x3E;
			mem[1] = 0xCC;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xCC00);
			Assert::IsTrue(cpu->PC == 2);
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

		TEST_METHOD(Test0x43)	// LD B,E	43
		{
			mem[0] = 0x43;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCD00);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0x47)	// LD B,A	47
		{
			mem[0] = 0x47;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xAB00);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0x4B)	// LD C,E	4B
		{
			mem[0] = 0x4B;
			cpu->DE = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00CD);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0x4F)	// LD C,A	4F
		{
			mem[0] = 0x4F;
			cpu->AF = 0xABCD;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x00AB);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0x53)	// LD D,E	53
		{
			mem[0] = 0x53;
			cpu->DE = 0x00CD;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0xCDCD);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0x7B)	// LD A,E	7B
		{
			mem[0] = 0x7B;
			cpu->DE = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400);
			Assert::IsTrue(cpu->PC == 1);
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

		TEST_METHOD(Test0xDD7D)	// LD A,XL	DD 7D
		{
			mem[0] = 0xDD;
			mem[1] = 0x7D;
			cpu->IX = 0x1234;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0x3400);
			Assert::IsTrue(cpu->PC == 2);
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

		TEST_METHOD(Test0xEB)	// EX DE,HL
		{
			mem[0] = 0xEB;
			cpu->DE = 0x1111;
			cpu->HL = 0x2222;
			cpu->Step();
			Assert::IsTrue(cpu->DE == 0x2222 && cpu->HL == 0x1111);
			Assert::IsTrue(cpu->PC == 1);
		}



	};
}