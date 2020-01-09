#include "EmuRender.h"
#include "Assets.h"

EmuRender::EmuRender(EmuModel *model)
{
	m_model = model;
	m_frame = new FrameRender(model, 4);
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
//	m_window.setFramerateLimit(60);
	m_frame->setPosition(0.f, 0.f);
	m_text = sf::Text();
	m_text.setFont(Assets::Instance().font);
	m_text.setFillColor(sf::Color::Cyan);
	m_text.setPosition(5.f, 5.f);
	return true;
}

void EmuRender::Render()
{
	m_frame->PrepareRender2();
	m_window.clear();
	m_window.draw(*this);
	m_window.display();
	int Framerate = (int)(1.f / m_clock.getElapsedTime().asSeconds());
	m_text.setString(std::to_string(Framerate));
	m_clock.restart();
}

void EmuRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(*m_frame, states);
	target.draw(m_text, states);
}
