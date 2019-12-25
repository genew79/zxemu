#pragma once
#include "Registers.h"

class EmuModel
{
public:
	static const int ROM_SIZE = 16384;
	static const int VIDEO_WIDTH = 256;
	static const int VIDEO_HEIGHT = 192;
	static const int VIDEO_SIZE = VIDEO_WIDTH / 8 * VIDEO_HEIGHT;
	static const int VIDEO_ATTR_SIZE = VIDEO_WIDTH / 8 * VIDEO_HEIGHT / 8;
	static const int RAM_SIZE = 48 * 1024 - ROM_SIZE - (VIDEO_SIZE + VIDEO_ATTR_SIZE);
protected:
	Registers reg;
	unsigned char rom[ROM_SIZE];
	unsigned char video[VIDEO_SIZE];
	unsigned char attr[VIDEO_ATTR_SIZE];
	unsigned char ram[RAM_SIZE];
public:
	EmuModel();
	void Init();
	unsigned char* getRom() { return rom; }
	unsigned char* getVideo() { return video; }
	unsigned char* getVideoAttr() { return attr; }
};
