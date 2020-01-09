#pragma once
#include "CPU.h"

class EmuModel
{
public:
	static const int MEM_SIZE = 65536;
	static const int ROM_SIZE = 16384;
	static const int VIDEO_WIDTH = 256;
	static const int VIDEO_HEIGHT = 192;
	static const int VIDEO_SIZE = VIDEO_WIDTH / 8 * VIDEO_HEIGHT;
	static const int VIDEO_ATTR_SIZE = VIDEO_WIDTH / 8 * VIDEO_HEIGHT / 8;
	static const int RAM_SIZE = MEM_SIZE - ROM_SIZE - (VIDEO_SIZE + VIDEO_ATTR_SIZE);
protected:
	CPU *cpu;
	unsigned __int8 ram[MEM_SIZE];
public:
	EmuModel();
	~EmuModel();
	void Init();
	void Step();
	CPU* getCpu() { return cpu; }
	unsigned __int8* getRam() { return ram; }
	unsigned __int8* getVideo() { return ram + ROM_SIZE; }
	unsigned __int8* getVideoAttr() { return ram + ROM_SIZE + VIDEO_SIZE; }
};
