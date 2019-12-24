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

	unsigned char* video = m_model->getVideo();
	unsigned char* attr = m_model->getVideoAttr();

/*
	for (int i = 0; i < EmuModel::VIDEO_SIZE; i++)
	{
		unsigned char data = video[i];
		for (int bit = 0; bit < 8; bit++)
		{
			int x = RenderHelper::getXCoord(i, bit);
			int y = RenderHelper::getYCoord(i);
			shape.setPosition(1.f * x * MULT_KOEF, 1.f * y * MULT_KOEF);
			shape.setFillColor(data & 0b10000000 ? fgcolor : bgcolor);
			target.draw(shape, states);
			data <<= 1;
		}
	}
*/

	for (int y = 0; y < EmuModel::VIDEO_HEIGHT; y++)
	{
		for (int x = 0; x < EmuModel::VIDEO_WIDTH / 8; x++)
		{
			int address = RenderHelper::getAddr(x, y);
			unsigned char data = video[address];
			for (int i = 0; i < 8; i++)
			{
				int x_coord = (x * 8 + i) * MULT_KOEF;
				int y_coord = y * MULT_KOEF;
				shape.setPosition(1.f * x_coord, 1.f * y_coord);
				shape.setFillColor(data & 0b10000000 ? fgcolor : bgcolor);
				target.draw(shape, states);
				data <<= 1;
			}
		}
	}

}
