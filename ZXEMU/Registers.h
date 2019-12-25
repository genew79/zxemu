#pragma once

class Registers
{
protected:
	union
	{
		unsigned __int8 F;
		unsigned __int16 AF;
	};
	union
	{
		unsigned __int8 C;
		unsigned __int16 BC;
	};
	union
	{
		unsigned __int8 E;
		unsigned __int16 DE;
	};
	union
	{
		unsigned __int8 L;
		unsigned __int16 HL;
	};
public:
	Registers();
	~Registers();
};
