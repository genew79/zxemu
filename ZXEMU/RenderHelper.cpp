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

int RenderHelper::getAttrColor(unsigned char color, bool bright)
{
	switch (color)
	{
	case 0:
		return 0x000000FF;
	case 1:
		return bright ? 0x0000FFFF : 0x0000C0FF;
	case 2:
		return bright ? 0xFF0000FF : 0xC00000FF;
	case 3:
		return bright ? 0xFF00FFFF : 0xC000C0FF;
	case 4:
		return bright ? 0x00FF00FF : 0x00C000FF;
	case 5:
		return bright ? 0x00FFFFFF : 0x00C0C0FF;
	case 6:
		return bright ? 0xFFFF00FF : 0xC0C000FF;
	case 7:
		return bright ? 0xFFFFFFFF : 0xC0C0C0FF;
	default:
		return 0;
	}
}

int RenderHelper::getFlatIndex(int x, int y, int magnify)
{
	return y * 256 * magnify + x;
}