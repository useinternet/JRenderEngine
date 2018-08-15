#pragma once
#include<Windows.h>
#include<dinput.h>
#include<memory>
#include "PresseManager.h"
//#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace JInputEngine
{
	class DirectInput
	{
	private:
		static float AxisX;
		static float AxisY;
		static float MouseX;
		static float MouseY;

	private:
		IDirectInput8 * m_DirectInput;
		IDirectInputDevice8* m_KeyBoard;
		IDirectInputDevice8* m_Mouse;
		std::unique_ptr<class PresseManager> m_PressedManager;
		unsigned char m_KeyBoardState[256];
		DIMOUSESTATE m_MouseState;

		int m_ScreenWidth, m_ScreenHeight;
		POINT m_MousePrevPos;
		POINT m_MousePos;

		bool m_bPressed = false;
		static bool m_bShowCursor;
		bool m_bPrevShowCursor = true;
	public:
		DirectInput();
		~DirectInput();


		bool DirectInputInit(HINSTANCE hinst, HWND hWnd, int width, int height);
		bool Update(const float deltaTime, HWND hWnd, const int mousex, const int mousey);
		void ReSize(const int width, const int height);
		// Commandinput  클래스는 따로 만들것 !!
		bool IsEscapePressed();
		//
		POINT GetMouseLocation();
		POINT GetMousePrevLocation();
		static float GetMouseAxisX();
		static float GetMouseAxisY();
		bool IsShowCursor();
		//
	private:
		void MouseLocationUpdate(HWND hWnd, const int mousex, const int mousey);
		void ProcessInput(const float deltaTime, HWND hWnd);
		bool ReadKeyboard();
		bool ReadMouse();
		void Shutdown();
	};
}



