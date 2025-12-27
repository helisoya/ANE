#include "ANESystem.h"
#include "shellapi.h"
#include <string>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	ANESystem* system;
	bool result;

	int argsCount;

	LPWSTR* argList = CommandLineToArgvW(GetCommandLine(), &argsCount);

	if (argsCount != 2) {
		MessageBox(NULL, L"Wrong number of launch arguments", L"Error", MB_OK);
		return 0;
	}

	// Create the system object.
	system = new ANESystem;

	ApplicationMode mode = GAME;

	if (wcscmp(argList[1], L"Game") == 0) mode = GAME;
	else if (wcscmp(argList[1], L"Debug") == 0) mode = GAME_DEBUG;
	else if (wcscmp(argList[1], L"SceneEditor") == 0) mode = SCENE_EDITOR;


	// Initialize and run the system object.
	result = system->Initialize(mode);

	LocalFree(argList);

	if (result)
	{
		system->Run();
	}

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}