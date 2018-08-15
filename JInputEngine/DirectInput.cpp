#include "DirectInput.h"


using namespace JInputEngine;

float DirectInput::AxisX;
float DirectInput::AxisY;
float DirectInput::MouseX;
float DirectInput::MouseY;
bool DirectInput::m_bShowCursor = false;
void SetDirectXScreenTransform(HWND hWnd, POINT* pos)
{
	RECT windowClientRect;
	GetClientRect(hWnd, &windowClientRect);

	pos->x = pos->x - (windowClientRect.right / 2);
	pos->y = pos->y - (windowClientRect.bottom / 2);
}
DirectInput::DirectInput()
{
	m_DirectInput = nullptr;
	m_KeyBoard = nullptr;
	m_Mouse = nullptr;
	m_MousePos = { 0,0 };
	m_MousePrevPos = { 0,0 };
}


DirectInput::~DirectInput()
{
}
bool DirectInput::DirectInputInit(HINSTANCE hinst, HWND hWnd, int width, int height)
{
	HRESULT result = S_OK;

	// Ŭ���̾�Ʈ ũ�� �� �����Ѵ�.
	m_ScreenWidth = width;
	m_ScreenHeight = height;
	m_PressedManager = std::make_unique<PresseManager>(m_KeyBoardState);

	/** ���̷�Ʈ ��ġ */
	// ���̷�Ʈ ��ǲ�� �������̽��� �ʱ�ȭ
	result = DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	/* Ű���� ��ġ */
	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_KeyBoard, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	result = m_KeyBoard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}
	// �ٸ� ���α׷������ ���·����� �����Ѵ�. ( ���� ���·� ����)
	result = m_KeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	// acquire �Լ��� �̿��Ͽ� �� ������ ������ Ű���忡 ���� ������ ����Ѵ�.
	result = m_KeyBoard->Acquire();
	if (FAILED(result))
	{
		return false;
	}


	/**  ���콺 ��ġ */
	result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}
	// ���콺 ���»��� ���� (���� ���·�) <- ���콺 ��Ŀ���� �Ҿ����� ã�Ҵ��� �˱����ؼ�..
	result = m_Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}
	// acquire �Լ��� �̿��Ͽ� �������� ������ ���콺�� ���� ������ ����Ѵ�.
	result = m_Mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}


	return true;
}
bool DirectInput::Update(const float deltaTime, HWND hWnd, const int mousex, const int mousey)
{
	bool result = true;
	

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}
	result = ReadMouse();
	if (!result)
	{
		return false;
	}
	MouseLocationUpdate(hWnd, mousex, mousey);
	ProcessInput(deltaTime, hWnd);

	AxisX = (float)m_MouseState.lX;
	AxisY = (float)m_MouseState.lY;

	return true;
}
void DirectInput::ReSize(const int width, const int height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;
}
bool DirectInput::IsEscapePressed()
{
	if (m_KeyBoardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

POINT DirectInput::GetMouseLocation()
{
	return m_MousePos;
}
POINT DirectInput::GetMousePrevLocation()
{
	return m_MousePrevPos;
}
float DirectInput::GetMouseAxisX()
{
	if (!m_bShowCursor)
	{
		return AxisX;
	}
	return 0.0f;
}
float DirectInput::GetMouseAxisY()
{
	if (!m_bShowCursor)
	{
		return AxisY;
	}
	return 0.0f;
}
bool DirectInput::IsShowCursor()
{
	return m_bShowCursor;
}
void DirectInput::MouseLocationUpdate(HWND hWnd, const int mousex, const int mousey)
{
	m_MousePos.x = mousex;
	m_MousePos.y = mousey;


	SetDirectXScreenTransform(hWnd, &m_MousePos);
	if (m_MousePos.x != m_MousePrevPos.x || m_MousePos.y != m_MousePrevPos.y)
	{
		m_MousePrevPos = m_MousePos;
		MouseX = (float)m_MousePos.x;
		MouseY = (float)m_MousePos.y;
	}
}
void DirectInput::ProcessInput(const float deltaTime , HWND hWnd)
{
	static float accTime = 0;
	accTime += deltaTime;
	if (PresseManager::IsPressed(Key::ECTRL) && !m_bPressed)
	{
		m_bPressed = true;
		if (m_bShowCursor) m_bShowCursor = false;
		else m_bShowCursor = true;
	}

	if (accTime >= 0.5f)
	{
		accTime = 0;
		m_bPressed = false;
	}

	// ���콺 Ŀ�� �Ⱥ��̰��ϱ�
	if (m_bShowCursor != m_bPrevShowCursor && m_bShowCursor)
	{

		RECT rect;
		GetClientRect(hWnd, &rect);

		ShowCursor(true);
		m_bPrevShowCursor = m_bShowCursor;
	}
	else if(m_bShowCursor != m_bPrevShowCursor && !m_bShowCursor)
	{
		ShowCursor(false);
		m_bPrevShowCursor = m_bShowCursor;
	}
	// ���콺 Ŀ���� �Ⱥ��̸�
	if (!m_bShowCursor)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);

		POINT LT;
		POINT RB;
		LT.x = (int)(rect.right * 0.4f) ;     LT.y = (int)(rect.bottom * 0.4f);
		RB.x = (int)(rect.right * 0.6f);    RB.y = (int)(rect.bottom * 0.6f);

		ClientToScreen(hWnd, &LT); ClientToScreen(hWnd, &RB);
		rect.left = LT.x;  rect.top = LT.y;
		rect.right = RB.x; rect.bottom = RB.y;

		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(nullptr);
	}

}

bool DirectInput::ReadKeyboard()
{
	HRESULT result = S_OK;

	result = m_KeyBoard->GetDeviceState(sizeof(m_KeyBoardState), (LPVOID)&m_KeyBoardState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_KeyBoard->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}
bool DirectInput::ReadMouse()
{
	HRESULT result = S_OK;

	result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_Mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;

}

void DirectInput::Shutdown()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = nullptr;
	}
	if (m_KeyBoard)
	{
		m_KeyBoard->Unacquire();
		m_KeyBoard->Release();
		m_Mouse = nullptr;
	}
	if (m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput = nullptr;
	}


}