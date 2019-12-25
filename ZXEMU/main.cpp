#include <iostream>
#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// ������� ������

	FILE* file;
	fopen_s(&file, "dizzy.scr", "rb");
	fread(model.getVideo(), 1, 6912, file);
	fclose(file);

	EmuRender render(&model);					// ������� �������������
	EmuController controller(&model, &render);	// ������� ����������
	controller.Run();							// ���������
	return 0;
}
