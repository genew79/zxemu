#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"

class EmuRender : public sf::Drawable, public sf::Transformable
{
public:
	static const int MULT_KOEF = 4;
	static const int FRAME_SIZE = EmuModel::VIDEO_WIDTH * MULT_KOEF * EmuModel::VIDEO_HEIGHT * MULT_KOEF;
protected:
	EmuModel *m_model;
	sf::RenderWindow m_window;
	sf::Vertex* m_pvertices;
public:
	EmuRender(EmuModel *model);
	~EmuRender();
	sf::RenderWindow& window() { return m_window; };
	bool Init();
	void Render();
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
