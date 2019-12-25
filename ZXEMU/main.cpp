#include <iostream>
#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// ������� ������

	try
	{
		FILE* file;
		fopen_s(&file, "ufo.scr", "rb");
		fread(model.getVideo(), 1, 6912, file);
		fclose(file);
	}
	catch(...)
	{
		return -1;
	}

	EmuRender render(&model);					// ������� �������������
	EmuController controller(&model, &render);	// ������� ����������
	controller.Run();							// ���������
	return 0;
}
