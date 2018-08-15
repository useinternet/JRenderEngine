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
		// 차피 임시니 렌더링 엔진 실험
		std::unique_ptr<RenderEngine> m_SampleRenderEngine;
		std::unique_ptr<JInputEngine::DirectInput> m_SampleInputEngine;
		std::unique_ptr<GameTimer> m_Timer;
	public:
		/**  생성자 및 소멸자  */
		WinMainLoop();
		~WinMainLoop();

		/** 게임 엔진 초기화 */
		bool Initialization();
		bool Update();
		void Shutdown();

		LRESULT CALLBACK WindowMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
		void MouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	private:

		/** 윈도우 초기화 */
		bool WindowsInitialization();
		/** 윈도우 셧다운 */
		void ShutdownWindows();

	};
	static WinMainLoop* globalGameEngine = nullptr;
}


/** 윈도우 프로시저 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);




