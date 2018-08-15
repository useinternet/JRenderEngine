
#include <Windows.h> 

#include"WinMainLoop.h"

using namespace JRenderEngine;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	bool result = true;
	WinMainLoop* Engine = new WinMainLoop;
	if (!Engine)
	{
		return 0;
	}
	Engine->Initialization();
	Engine->Update();

	Engine->Shutdown();

	delete Engine;
	Engine = nullptr;
	return 0;
}
