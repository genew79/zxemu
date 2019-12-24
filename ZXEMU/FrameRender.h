#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"

class FrameRender : public sf::Drawable, public sf::Transformable
{
public:
	static const int MULT_KOEF = 4;
	static const int FRAME_WIDTH = EmuModel::VIDEO_WIDTH * MULT_KOEF;
	static const int FRAME_HEIGHT = EmuModel::VIDEO_HEIGHT * MULT_KOEF;
	static const int FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT;
protected:
	EmuModel *m_model;
public:
	FrameRender(EmuModel *model);
	~FrameRender();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

