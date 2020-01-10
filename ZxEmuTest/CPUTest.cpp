#include "CppUnitTest.h"
#include "../ZxEmu/CPU.h"
#include "../ZxEmu/EmuModel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZxEmuTest
{		
	TEST_CLASS(CPUTest)
	{
	public:
		
		TEST_METHOD(TestLD16)
		{
			unsigned __int16 reg;
			unsigned __int16 val = 0xFFFF;
			CPU::LD16(reg, val);
			Assert::IsTrue(reg == 0xFFFF, L"LD16");
		}

		TEST_METHOD(TestLD8HI)
		{
			unsigned __int16 reg = 0xAAAA;
			unsigned __int8 val = 0xCC;
			CPU::LD8HI(reg, val);
			Assert::IsTrue(reg == 0xCCAA, L"LD8HI");
		}

		TEST_METHOD(TestLD8LO)
		{
			unsigned __int16 reg = 0xAAAA;
			unsigned __int8 val = 0xCC;
			CPU::LD8LO(reg, val);
			Assert::IsTrue(reg == 0xAACC, L"LD8LO");
		}
	};
}