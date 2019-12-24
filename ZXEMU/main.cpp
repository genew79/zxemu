#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// ������� ������
	EmuRender render(&model);					// ������� �������������
	EmuController controller(&model, &render);	// ������� ����������
	controller.Run();							// ���������
	return 0;
}
