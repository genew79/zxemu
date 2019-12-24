#pragma once
class RenderHelper
{
public:
	RenderHelper();
	~RenderHelper();
	static int getXCoord(int pos, int bit);
	static int getYCoord(int pos);
	static int getBlock(int y);
	static int getAddr(int x, int y);
};
