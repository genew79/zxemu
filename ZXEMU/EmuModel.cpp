#include "EmuModel.h"

EmuModel::EmuModel()
{
	Init();
}

void EmuModel::Init()
{
	for (int i = 0; i < VIDEO_SIZE; i++) m_video[i] = 0b10101010;
	for (int i = 0; i < VIDEO_ATTR_SIZE; i++) m_attr[i] = 0b11111100;
}
