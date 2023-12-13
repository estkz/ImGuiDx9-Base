#include "menu/gui.h"

// Miscellaneous Related Includes
#include <thread>

// Main Entrypoint
int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{
	gui::CreateHWindow("YourCheatName");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isOpened)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	// Once EXIT_SUCCES gets returned it indicates that the cheat has executed successfully and is now terminating (shutting down).
	return EXIT_SUCCESS;
}