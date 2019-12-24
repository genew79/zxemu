#include "EmuRender.h"

EmuRender::EmuRender(EmuModel *model)
{
	m_model = model;
	m_frame = new FrameRender(model);
	Init();
}

EmuRender::~EmuRender()
{
	delete m_frame;
}

bool EmuRender::Init()
{
	setPosition(0.f, 0.f);
	m_window.create(sf::VideoMode(FRAME_WIDTH, FRAME_HEIGHT), "ZXEmu");
	m_window.setFramerateLimit(60);
	m_frame->setPosition(0.f, 0.f);
	return true;
}

void EmuRender::Render()
{
	m_window.clear();
	m_window.draw(*this);
	m_window.display();
}

void EmuRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(*m_frame, states);
}
