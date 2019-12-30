#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"

class FrameRender : public sf::Drawable, public sf::Transformable
{
protected:
	int m_magnify;
	EmuModel *m_model;
	sf::Uint8* m_pixels;
	sf::Texture m_texture;
	static int invert_counter;
	static bool invert_flag;
public:
	FrameRender(EmuModel *model, int magnify = 4);
	~FrameRender();
	void PrepareRender();
	void Invertor() const;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

