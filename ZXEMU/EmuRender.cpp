#include "EmuRender.h"

EmuRender::EmuRender(EmuModel *model)
{
	m_model = model;
	Init();
}

EmuRender::~EmuRender()
{
	delete[] m_pvertices;
}

bool EmuRender::Init()
{
	if (m_pvertices) delete[] m_pvertices;
	m_pvertices = new sf::Vertex[FRAME_SIZE];
	setPosition(0.f, 0.f);
	m_window.create(sf::VideoMode(EmuModel::VIDEO_WIDTH * MULT_KOEF, EmuModel::VIDEO_HEIGHT * MULT_KOEF), "ZXEmu");
	m_window.setFramerateLimit(60);
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

	for (int y = 0; y < EmuModel::VIDEO_HEIGHT; y++)
	{
		for (int x = 0; x < EmuModel::VIDEO_WIDTH; x++)
		{
			int i = y * EmuModel::VIDEO_WIDTH + x;
			m_pvertices[i].position = sf::Vector2f(x * MULT_KOEF * 1.f, y * MULT_KOEF * 1.f);
			m_pvertices[i].color = sf::Color::White;
		}
	}

	target.draw(m_pvertices, FRAME_SIZE, sf::Points);
}
