#include "EmuModel.h"

EmuModel::EmuModel()
{
	cpu = new CPU(ram);
	Init();
}

EmuModel::~EmuModel()
{
	delete cpu;
}

void EmuModel::Init()
{
	for (int i = 0; i < MEM_SIZE; i++) ram[i] = 0;
	for (int i = ROM_SIZE + VIDEO_SIZE; i < ROM_SIZE + VIDEO_SIZE + VIDEO_ATTR_SIZE; i++) ram[i] = 0b00111000;
}

void EmuModel::Step()
{
	cpu->Step();
}
