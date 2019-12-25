#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"

class FrameRender : public sf::Drawable, public sf::Transformable
{
protected:
	int m_magnify;
	EmuModel *m_model;
	static int invert_counter;
	static bool invert_flag;
public:
	FrameRender(EmuModel *model, int magnify = 4);
	~FrameRender();
	void Invertor() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

