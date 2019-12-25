#include "EmuModel.h"

EmuModel::EmuModel()
{
	Init();
}

void EmuModel::Init()
{
	for (int i = 0; i < ROM_SIZE; i++) video[i] = 0;
	for (int i = 0; i < VIDEO_SIZE; i++) video[i] = 0b10101010;
	for (int i = 0; i < VIDEO_ATTR_SIZE; i++) attr[i] = 0b11111100;
}
