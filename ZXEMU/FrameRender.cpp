#include "FrameRender.h"
#include "RenderHelper.h"

int FrameRender::invert_counter = 0;
bool FrameRender::invert_flag = false;

FrameRender::FrameRender(EmuModel *model, int magnify)
{
	m_model = model;
	m_magnify = magnify;
	m_pixels = new sf::Uint8[EmuModel::VIDEO_WIDTH * magnify * EmuModel::VIDEO_HEIGHT * magnify * 4];
	m_texture.create(EmuModel::VIDEO_WIDTH * magnify, EmuModel::VIDEO_HEIGHT * magnify);
}

FrameRender::~FrameRender()
{
	delete[] m_pixels;
}

void FrameRender::PrepareRender2()
{
	Invertor();

	sf::Color fgcolor, bgcolor, *pcolor;

	auto video = m_model->getVideo();
	auto attr = m_model->getVideoAttr();

	int video_index = 0;
	int attr_index = 0;
	int pixel_index = 0;
	int bpp = 4;
	int pixel_line_size = 256 * m_magnify * bpp;

	for (int block = 0; block < 3; block++)
	{
		for (int line = 0; line < 8; line++)
		{
			for (int row = 0; row < 8; row++)
			{
				for (int index = 0; index < 32; index++)
				{
					auto data = video[video_index];
					auto color = attr[attr_index];

					bool bright = (color & 0b01000000) > 0;
					fgcolor = sf::Color(RenderHelper::getAttrColor(color & 0b00000111, bright));
					bgcolor = sf::Color(RenderHelper::getAttrColor((color >> 3) & 0b00000111, bright));
					bool blink = (color & 0b10000000) > 0;
					if (blink && invert_flag) std::swap(fgcolor, bgcolor);

					for (int bit = 0; bit < 8; bit++)
					{
						pcolor = data & 0b10000000 ? &fgcolor : &bgcolor;
						for (int x = 0; x < m_magnify; x++)
						{
							for (int y = 0; y < m_magnify; y++)
							{
								int offset = y * pixel_line_size;
								m_pixels[pixel_index + offset + 0] = pcolor->r;
								m_pixels[pixel_index + offset + 1] = pcolor->g;
								m_pixels[pixel_index + offset + 2] = pcolor->b;
								m_pixels[pixel_index + offset + 3] = pcolor->a;
							}
							pixel_index += bpp;
						}
						data <<= 1;
					}
					video_index++;
					attr_index++;
				}
				pixel_index -= pixel_line_size;
				pixel_index += 8 * m_magnify * pixel_line_size;
			}
			pixel_index -= pixel_line_size * 64 * m_magnify;
			pixel_index += pixel_line_size * m_magnify;
			attr_index -= 32 * 8;
		}
		pixel_index -= pixel_line_size * m_magnify * 8;
		pixel_index += pixel_line_size * m_magnify * 64;
		attr_index += 32 * 8;
	}


	m_texture.update(m_pixels);
}

void FrameRender::PrepareRender()
{
	Invertor();

	sf::Color fgcolor, bgcolor;

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

			int index;
			for (int bit = 0; bit < 8; bit++)
			{
				for (int dy = 0; dy < m_magnify; dy++)
				{
					for (int dx = 0; dx < m_magnify; dx++)
					{
						index = 4 * RenderHelper::getFlatIndex(m_magnify * (x + bit) + dx, m_magnify * y + dy, m_magnify);
						m_pixels[index + 0] = data & 0b10000000 ? fgcolor.r : bgcolor.r;
						m_pixels[index + 1] = data & 0b10000000 ? fgcolor.g : bgcolor.g;
						m_pixels[index + 2] = data & 0b10000000 ? fgcolor.b : bgcolor.b;
						m_pixels[index + 3] = data & 0b10000000 ? fgcolor.a : bgcolor.a;
					}
				}
				data <<= 1;
			}
		}
	}

	m_texture.update(m_pixels);
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
/*
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
				shape.setFillColor(sf::Color::White);
				shape.setFillColor(data & 0b10000000 ? fgcolor : bgcolor);
				target.draw(shape, states);
				data <<= 1;
			}
		}
	}
*/
	sf::Sprite sprite(m_texture);
	target.draw(sprite);
}
