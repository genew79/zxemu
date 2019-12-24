#pragma once

class EmuModel
{
public:
	static const int ROM_SIZE = 16384;
	static const int VIDEO_WIDTH = 256;
	static const int VIDEO_HEIGHT = 192;
	static const int VIDEO_SIZE = VIDEO_WIDTH * VIDEO_HEIGHT / 8 + VIDEO_WIDTH / 8 * VIDEO_HEIGHT / 8;
	static const int RAM_SIZE = 48 * 1024 - ROM_SIZE - VIDEO_SIZE;
protected:
	unsigned char m_rom[ROM_SIZE];
	unsigned char m_video[VIDEO_SIZE];
	unsigned char m_ram[RAM_SIZE];
public:
	EmuModel();
	void Init();
};
