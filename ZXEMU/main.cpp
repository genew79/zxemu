#include <iostream>
#include "Assets.h"
#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main(int argc, char* argv[])
{
	FILE* file;

	Assets::Instance().Load();

	EmuModel model;								// ������� ������

	fopen_s(&file, "rom48.rom", "rb");
	fread(model.getRam(), 1, 16384, file);
	fclose(file);

	if (argc > 1)
	{
		fopen_s(&file, argv[1], "rb");
		fread(model.getVideo(), 1, 6912, file);
		fclose(file);
	}

	EmuRender render(&model);					// ������� �������������
	EmuController controller(&model, &render);	// ������� ����������
	controller.Run();							// ���������
	return 0;
}
