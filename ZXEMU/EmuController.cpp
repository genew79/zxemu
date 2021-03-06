#include "EmuController.h"

EmuController::EmuController(EmuModel *model, EmuRender *render)
{
	m_model = model;
	m_render = render;
}

EmuController::~EmuController()
{
}

void EmuController::Run()
{
	sf::Event event;

	while (m_render->window().isOpen())
	{
		while (m_render->window().pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_render->window().close();
			if (event.type == sf::Event::KeyPressed)
			{
				// �������� ������� ������� - ��������� ��������������� ��������
				if (event.key.code == sf::Keyboard::Escape) m_render->window().close();
			}
		}

		for(int i = 0; i < 1000; i++) m_model->Step();
		// ��������� ����������� �������� �� ���������
		m_render->Render();
	}
}
