#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// ������� ������

	try
	{
		auto file = fopen("ufo.scr", "r");
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
