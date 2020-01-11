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
			cpu->PC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234);
			Assert::IsTrue(cpu->PC == 3);
		}

		TEST_METHOD(Test0x06)	// LD B,N 06 NN
		{
			mem[0] = 0x06;
			mem[1] = 0x12;
			cpu->BC = 0x0034;
			cpu->PC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0x1234);
			Assert::IsTrue(cpu->PC == 2);
		}

		TEST_METHOD(Test0x08)	// EX AF, A'F' 08
		{
			mem[0] = 0x08;
			cpu->AF = 0x1234;
			cpu->_AF = 0x5678;
			cpu->PC = 0x0000;
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
			cpu->PC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->AF == 0xDDEE);
			Assert::IsTrue(cpu->PC == 1);
		}

		TEST_METHOD(Test0x0E)	// LD C,N	0E NN
		{
			mem[0] = 0x0E;
			mem[1] = 0xDD;
			cpu->BC = 0xCC00;
			cpu->PC = 0x0000;
			cpu->Step();
			Assert::IsTrue(cpu->BC == 0xCCDD);
			Assert::IsTrue(cpu->PC == 2);
		}



	};
}