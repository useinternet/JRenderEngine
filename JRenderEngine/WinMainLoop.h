#pragma once

#include"JRenderEngine.h"
#include"DirectInput.h"
#include"GameTimer.h"
#pragma comment(lib, "JInputEngine.lib")
#pragma comment(lib, "JTimerEngine.lib")
namespace JRenderEngine
{
	class RenderEngine;

	class WinMainLoop
	{
	private:
		HINSTANCE m_hInst;
		HWND m_hWnd;
		const WCHAR* m_wTitle;
		const WCHAR* m_wWindowClass;
		bool m_bPaused;
		bool m_bMouseCursorinClient;
		static const int m_nScreenWidth;
		static const int m_nScreenHeight;
		int m_nMousePosX;
		int m_nMousePosY;
		// ���� �ӽô� ������ ���� ����
		std::unique_ptr<RenderEngine> m_SampleRenderEngine;
		std::unique_ptr<JInputEngine::DirectInput> m_SampleInputEngine;
		std::unique_ptr<GameTimer> m_Timer;
	public:
		/**  ������ �� �Ҹ���  */
		WinMainLoop();
		~WinMainLoop();

		/** ���� ���� �ʱ�ȭ */
		bool Initialization();
		bool Update();
		void Shutdown();

		LRESULT CALLBACK WindowMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		void MouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	private:

		/** ������ �ʱ�ȭ */
		bool WindowsInitialization();
		/** ������ �˴ٿ� */
		void ShutdownWindows();

	};
	static WinMainLoop* globalGameEngine = nullptr;
}


/** ������ ���ν��� */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




