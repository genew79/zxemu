#include "CppUnitTest.h"
#include "../ZxEmu/RenderHelper.h"
#include "../ZxEmu/EmuModel.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ZxEmuTest
{		
	TEST_CLASS(RenderHelperTest)
	{
	public:
		
		TEST_METHOD(TestXCoord)
		{
			// First line of screen
			Assert::AreEqual(0, RenderHelper::getXCoord(0, 0), L"wrong X coordinate for address = 0, bit = 0");
			Assert::AreEqual(7, RenderHelper::getXCoord(0, 7), L"wrong X coordinate for address = 0, bit = 7");
			Assert::AreEqual(8, RenderHelper::getXCoord(1, 0), L"wrong X coordinate for address = 1, bit = 0");
			Assert::AreEqual(15, RenderHelper::getXCoord(1, 7), L"wrong X coordinate for address = 1, bit = 7");
			Assert::AreEqual(248, RenderHelper::getXCoord(31, 0), L"wrong X coordinate for address = 31, bit = 0");
			Assert::AreEqual(255, RenderHelper::getXCoord(31, 7), L"wrong X coordinate for address = 31, bit = 7");

			// 8-th line of screen
			// address = 256 / 8 = 32
			Assert::AreEqual(0, RenderHelper::getXCoord(32, 0), L"wrong X coordinate for address = 32, bit = 0");
			Assert::AreEqual(7, RenderHelper::getXCoord(32, 7), L"wrong X coordinate for address = 32, bit = 7");
			Assert::AreEqual(248, RenderHelper::getXCoord(63, 0), L"wrong X coordinate for address = 63, bit = 0");
			Assert::AreEqual(255, RenderHelper::getXCoord(63, 7), L"wrong X coordinate for address = 63, bit = 7");
		}

		TEST_METHOD(TestYCoord)
		{
			// First block of screen

			// First line of screen, address = 0
			Assert::AreEqual(0, RenderHelper::getYCoord(0), L"wrong Y coordinate for address = 0");
			// Second line of screen, address = 8 * 32 = 256
			Assert::AreEqual(1, RenderHelper::getYCoord(256), L"wrong Y coordinate for address = 256");

			// 8-th line of screen, address = 1 * 32 = 32
			Assert::AreEqual(8, RenderHelper::getYCoord(32), L"wrong Y coordinate for address = 32");
			// 16-th line of screen, address = 2 * 32 = 64
			Assert::AreEqual(16, RenderHelper::getYCoord(64), L"wrong Y coordinate for address = 64");

			// Second block of screen

			// 64-th line of screen, address = 64 * 32 = 2048
			Assert::AreEqual(64, RenderHelper::getYCoord(2048), L"wrong Y coordinate for address = 2048");
		}

		TEST_METHOD(TestAddr)
		{
			// 1 line of screen
			Assert::AreEqual(0, RenderHelper::getAddr(0, 0), L"Wrong address for coordinate (0, 0)");
			Assert::AreEqual(0, RenderHelper::getAddr(7, 0), L"Wrong address for coordinate (7, 0)");
			Assert::AreEqual(1, RenderHelper::getAddr(8, 0), L"Wrong address for coordinate (8, 0)");
			Assert::AreEqual(1, RenderHelper::getAddr(15, 0), L"Wrong address for coordinate (15, 0)");
			Assert::AreEqual(31, RenderHelper::getAddr(248, 0), L"Wrong address for coordinate (248, 0)");
			Assert::AreEqual(31, RenderHelper::getAddr(255, 0), L"Wrong address for coordinate (255, 0)");
			// 8 line of screen
			Assert::AreEqual(32, RenderHelper::getAddr(0, 8), L"Wrong address for coordinate (0, 8)");
			// 2 line of screen
			Assert::AreEqual(256, RenderHelper::getAddr(0, 1), L"Wrong address for coordinate (0, 1)");
		}

		TEST_METHOD(TestAttrAddr)
		{
			Assert::AreEqual(0, RenderHelper::getAttrAddr(0, 0), L"Wrong attribute address for coordinate (0, 0)");
			Assert::AreEqual(0, RenderHelper::getAttrAddr(0, 1), L"Wrong attribute address for coordinate (0, 1)");
			Assert::AreEqual(1, RenderHelper::getAttrAddr(8, 0), L"Wrong attribute address for coordinate (8, 0)");
			Assert::AreEqual(1, RenderHelper::getAttrAddr(8, 1), L"Wrong attribute address for coordinate (8, 1)");
			Assert::AreEqual(31, RenderHelper::getAttrAddr(255, 0), L"Wrong attribute address for coordinate (255, 0)");
			Assert::AreEqual(32, RenderHelper::getAttrAddr(0, 8), L"Wrong attribute address for coordinate (0, 8)");
			Assert::AreEqual(33, RenderHelper::getAttrAddr(8, 8), L"Wrong attribute address for coordinate (8, 8)");
			Assert::AreEqual(767, RenderHelper::getAttrAddr(255, 191), L"Wrong attribute address for coordinate (255, 191)");
		}

		TEST_METHOD(TestFlatIndex)
		{
			Assert::AreEqual(0, RenderHelper::getFlatIndex(0, 0), L"Wrong index for coordinate (0, 0)");
			Assert::AreEqual(1, RenderHelper::getFlatIndex(1, 0), L"Wrong index for coordinate (1, 0)");
			Assert::AreEqual(255, RenderHelper::getFlatIndex(255, 0), L"Wrong index for coordinate (255, 0)");
			Assert::AreEqual(256, RenderHelper::getFlatIndex(0, 1), L"Wrong index for coordinate (0, 1)");
			Assert::AreEqual(257, RenderHelper::getFlatIndex(1, 1), L"Wrong index for coordinate (0, 0)");
			Assert::AreEqual(49151, RenderHelper::getFlatIndex(255, 191), L"Wrong index for coordinate (255, 191)");
		}

	};
}