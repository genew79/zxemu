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
	static int invert_counter;
	static bool invert_flag;
public:
	FrameRender(EmuModel *model);
	~FrameRender();
	void Invertor() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

