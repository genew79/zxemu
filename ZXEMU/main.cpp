#include <iostream>
#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// Создаем модель

	FILE* file;
	fopen_s(&file, "dizzy.scr", "rb");
	fread(model.getVideo(), 1, 6912, file);
	fclose(file);

	EmuRender render(&model);					// Создаем представление
	EmuController controller(&model, &render);	// Создаем контроллер
	controller.Run();							// Запускаем
	return 0;
}
