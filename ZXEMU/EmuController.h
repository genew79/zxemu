#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"
#include "EmuRender.h"

class EmuController
{
	EmuModel *m_model;
	EmuRender *m_render;
public:
	EmuController(EmuModel *model, EmuRender *render);
	~EmuController();
	void Run();
};
