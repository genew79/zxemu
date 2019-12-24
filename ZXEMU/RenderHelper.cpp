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
	int block_size = (256 / 8 * 192) / 3;		// 2048
	int block = address / block_size;
	int blockaddr = address % block_size;

	return block * 192 / 3 + blockaddr / 32 * 8;
}

int RenderHelper::getBlock(int y)
{
	return y / 64;
}

int RenderHelper::getRow(int y)
{
	return (y % 64) / 8;
}

int RenderHelper::getLine(int y)
{
	return y % 8;
}

int RenderHelper::getAddr(int x, int y)
{
	int block = RenderHelper::getBlock(y);
	int row = RenderHelper::getRow(y);
	int line = RenderHelper::getLine(y);
	int addr = block * 2048 + row * 32 + line * 256 + x / 8;
	return addr;
}
