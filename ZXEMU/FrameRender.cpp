#include "FrameRender.h"
#include "RenderHelper.h"

int FrameRender::invert_counter = 0;
bool FrameRender::invert_flag = false;

FrameRender::FrameRender(EmuModel *model, int magnify)
{
	m_model = model;
	m_magnify = magnify;
}

FrameRender::~FrameRender()
{
}

void FrameRender::Invertor() const
{
	if (invert_counter++ > 0)
	{
		invert_flag = !invert_flag;
		invert_counter = 0;
	}
}

void FrameRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	Invertor();

	sf::Color fgcolor, bgcolor;
	sf::RectangleShape shape(sf::Vector2f(1.f * m_magnify, 1.f * m_magnify));
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

			bool bright = (color & 0b01000000) > 0;
			fgcolor = sf::Color(RenderHelper::getAttrColor(color & 0b00000111, bright));
			bgcolor = sf::Color(RenderHelper::getAttrColor((color >> 3) & 0b00000111, bright));

			bool blink = (color & 0b10000000) > 0;
			if (blink && invert_flag) std::swap(fgcolor, bgcolor);

			for (int bit = 0; bit < 8; bit++)
			{
				shape.setPosition(1.f * (x + bit) * m_magnify, 1.f * y * m_magnify);
				shape.setFillColor(data & 0b10000000 ? fgcolor : bgcolor);
				target.draw(shape, states);
				data <<= 1;
			}
		}
	}
}
