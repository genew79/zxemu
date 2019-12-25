#include "RenderHelper.h"

RenderHelper::RenderHelper()
{
}

RenderHelper::~RenderHelper()
{
}

int RenderHelper::getXCoord(int address, int bit)
{
	return (address * 8) % 256 + bit;
}

int RenderHelper::getYCoord(int address)
{
	int block = address / 2048;
	int line = (address % 2048) / 256;
	int row = ((address % 2048) % 256) / 32;

	return block * 64 + row * 8 + line;
}

int RenderHelper::getAddr(int x, int y)
{
	int block = y / 64;
	int row = (y % 64) / 8;
	int line = y % 8;
	int addr = block * 2048 + row * 32 + line * 256 + x / 8;
	return addr;
}

int RenderHelper::getAttrAddr(int x, int y)
{
	return y / 8 * 32 + x / 8;
}

int RenderHelper::getAttrColor(unsigned char color)
{
	switch (color)
	{
	case 0:
		return 0x000000FF;
	case 1:
		return 0xFFFFFFFF;
	case 2:
		return 0xFFFFFFFF;
	case 3:
		return 0xFFFFFFFF;
	case 4:
		return 0xFFFFFFFF;
	case 5:
		return 0xFFFFFFFF;
	case 6:
		return 0xFFFFFFFF;
	case 7:
		return 0xFFFFFFFF;
	default:
		return 0;
	}
}
