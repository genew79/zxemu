#include "FrameRender.h"
#include "RenderHelper.h"

FrameRender::FrameRender(EmuModel *model)
{
	m_model = model;
}

FrameRender::~FrameRender()
{
}

void FrameRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	sf::Color fgcolor = sf::Color(255, 255, 255);
	sf::Color bgcolor = sf::Color(0, 0, 0);
	sf::RectangleShape shape(sf::Vector2f(MULT_KOEF, MULT_KOEF));
	shape.setOutlineThickness(0.f);

	auto video = m_model->getVideo();
	auto attr = m_model->getVideoAttr();

	for (int y = 0; y < EmuModel::VIDEO_HEIGHT; y++)
	{
		for (int x = 0; x < EmuModel::VIDEO_WIDTH; x += 8)
		{
			int addr = RenderHelper::getAddr(x, y);
			int attr_addr = RenderHelper::getAttrAddr(x, y);
			auto data = video[addr];
			auto color = attr[attr_addr];

			fgcolor = sf::Color(RenderHelper::getAttrColor(color & 0b00000111));
			bgcolor = sf::Color(RenderHelper::getAttrColor((color >> 3) & 0b00000111));

			for (int bit = 0; bit < 8; bit++)
			{
				shape.setPosition(1.f * (x + bit) * MULT_KOEF, 1.f * y * MULT_KOEF);
				shape.setFillColor(data & 0b10000000 ? fgcolor : bgcolor);
				target.draw(shape, states);
				data <<= 1;
			}
		}
	}
}
