
#include "WinMainLoop.h"
#include "RenderEngine.h"
using namespace std;
using namespace JRenderEngine;
using namespace JInputEngine;
const int WinMainLoop::m_nScreenWidth = 1920;
const int WinMainLoop::m_nScreenHeight = 1080;

/**  ������ �� �Ҹ���  */
WinMainLoop::WinMainLoop()
{
	// WinMainLoop Ŭ���� ����
	m_wTitle = L"GameFrameWork";
	m_wWindowClass = L"GameFrameWork";
	m_bPaused = false;
	m_bMouseCursorinClient = false;
}
WinMainLoop::~WinMainLoop()
{

}
bool WinMainLoop::Update()
{
	MSG msg;
	bool result = true;

	while (!m_bPaused)
	{
		m_Timer->Tick();
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			m_bPaused = true;
		}
		if (m_Timer->DeltaTime() < 0.5f)
		{
			m_SampleRenderEngine->Update(m_Timer->DeltaTime());
			m_SampleInputEngine->Update(m_Timer->DeltaTime(), m_hWnd, m_nMousePosX, m_nMousePosY);
		}
	}
	return true;
}
void WinMainLoop::Shutdown()
{
	ShutdownWindows();
}
bool WinMainLoop::Initialization()
{
	bool result = true;
	globalGameEngine = this;

	/* ������ �ʱ�ȭ */
	result = WindowsInitialization();
	if (!result)
	{
		return false;
	}


	m_SampleRenderEngine = std::make_unique<RenderEngine>();
	result = m_SampleRenderEngine->Initialization(m_hWnd, m_nScreenWidth, m_nScreenHeight);
	if (!result)
	{
		return false;
	}
	m_SampleInputEngine = std::make_unique<DirectInput>();
	result = m_SampleInputEngine->DirectInputInit(m_hInst, m_hWnd, m_nScreenWidth, m_nScreenHeight);
	if (!result)
	{
		return false;
	}
	m_Timer = std::make_unique<GameTimer>();
	m_Timer->Start();

	return true;
}
LRESULT CALLBACK WinMainLoop::WindowMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_SIZE:
		break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		MouseMessage(hWnd, Message, wParam, lParam);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, Message, wParam, lParam);
	
}
void WinMainLoop::MouseMessage(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	m_nMousePosX = (int)LOWORD(lParam);
	m_nMousePosY = (int)HIWORD(lParam);
	// �Է� ����� ���� ������ ���� ~!
	switch (Message)
	{
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_RBUTTONUP:
		break;
	}
}

/** ������ �ʱ�ȭ */
bool WinMainLoop::WindowsInitialization()
{
	// �ν��Ͻ� �ڵ��� ��� �´�.
	m_hInst = GetModuleHandle(NULL);

	/* ������ ���� ���� */
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, 0);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = m_wWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, 0);


	// ������ Ȱ��ȭ
	RegisterClassEx(&wcex);
	
	// ������ �ڵ� ����
	m_hWnd = CreateWindowEx(NULL, m_wWindowClass, m_wTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, m_nScreenWidth, m_nScreenHeight, NULL, NULL, m_hInst, NULL);


	if (!m_hWnd) // ������ �ڵ� ���� ���н�.
	{
		return false;
	}


	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}
/** ������ �˴ٿ� */
void WinMainLoop::ShutdownWindows()
{
	// ������ �ı�
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// ������ Ŭ���� ��Ȱ��ȭ
	UnregisterClass(m_wWindowClass, m_hInst);
	m_hInst = NULL;
}

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default: /* */
		return globalGameEngine->WindowMessage(hWnd, message, wParam, lParam);
	}
}