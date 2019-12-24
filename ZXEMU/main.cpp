#include "EmuModel.h"
#include "EmuRender.h"
#include "EmuController.h"

int main()
{
	EmuModel model;								// Создаем модель
	EmuRender render(&model);					// Создаем представление
	EmuController controller(&model, &render);	// Создаем контроллер
	controller.Run();							// Запускаем
	return 0;
}
