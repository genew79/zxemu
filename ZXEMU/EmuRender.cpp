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

	m_textFramerate = sf::Text();
	m_textFramerate.setFont(Assets::Instance().font);
	m_textFramerate.setFillColor(sf::Color::Cyan);
	m_textFramerate.setPosition(5.f, 5.f);

	m_textPC = sf::Text();
	m_textPC.setFont(Assets::Instance().font);
	m_textPC.setFillColor(sf::Color::Yellow);
	m_textPC.setPosition(950.f, 5.f);
	return true;
}

void EmuRender::Render()
{
	unsigned __int16* video = (unsigned __int16*)m_model->getVideo();
	video[0] = m_model->getCpu()->PC;
	m_frame->PrepareRender2();
	m_window.clear();
	m_window.draw(*this);
	m_window.display();
	int Framerate = (int)(1.f / m_clock.getElapsedTime().asSeconds());
	m_textFramerate.setString(std::to_string(Framerate));
	m_clock.restart();

	m_textPC.setString(std::to_string(m_model->getCpu()->PC));
}

void EmuRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(*m_frame, states);
	target.draw(m_textFramerate, states);
	target.draw(m_textPC, states);
}
