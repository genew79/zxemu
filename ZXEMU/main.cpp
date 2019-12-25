#include <iostream>
#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main(int argc, char* argv[])
{
	EmuModel model;								// Создаем модель

	if (argc > 1)
	{
		FILE* file;
		fopen_s(&file, argv[1], "rb");
		fread(model.getVideo(), 1, 6912, file);
		fclose(file);
	}

	EmuRender render(&model);					// Создаем представление
	EmuController controller(&model, &render);	// Создаем контроллер
	controller.Run();							// Запускаем
	return 0;
}
