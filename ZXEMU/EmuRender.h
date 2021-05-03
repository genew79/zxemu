#pragma once
#include <SFML/Graphics.hpp>
#include "EmuModel.h"
#include "FrameRender.h"

class EmuRender : public sf::Drawable, public sf::Transformable
{
public:
	static const int FRAME_WIDTH = EmuModel::VIDEO_WIDTH * 4;
	static const int FRAME_HEIGHT = EmuModel::VIDEO_HEIGHT * 4;
	static const int FRAME_SIZE = FRAME_WIDTH * FRAME_HEIGHT;
protected:
	EmuModel *m_model;
	FrameRender* m_frame;
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Text m_textFramerate;
	sf::Text m_textPC;
public:
	EmuRender(EmuModel *model);
	~EmuRender();
	sf::RenderWindow& window() { return m_window; };
	bool Init();
	void Render();
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
