#pragma once
class RenderHelper
{
public:
	RenderHelper();
	~RenderHelper();
	static int getXCoord(int pos, int bit);
	static int getYCoord(int pos);
	static int getAddr(int x, int y);
	static int getAttrAddr(int x, int y);
	static int getAttrColor(unsigned char color, bool bright);
};
