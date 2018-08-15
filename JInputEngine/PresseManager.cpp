
#include<dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "PresseManager.h"
using namespace JInputEngine;
using namespace std;
// 임시 스태틱 변수
unsigned char* PresseManager::m_pKeyBoardState;

PresseManager::PresseManager(unsigned char* KeyBoardState)
{
	m_pKeyBoardState = KeyBoardState;
}


PresseManager::~PresseManager()
{

}
bool PresseManager::IsPressed(Key key)
{
	switch (key)
	{
		// 일단 카메라 키구현만
	case Key::EW:
		if (m_pKeyBoardState[DIK_W] & 0x80) return true;
		break;
	case Key::ES:
		if (m_pKeyBoardState[DIK_S] & 0x80) return true;
		break;
	case Key::EA:
		if (m_pKeyBoardState[DIK_A] & 0x80) return true;
		break;
	case Key::ED:
		if (m_pKeyBoardState[DIK_D] & 0x80) return true;
		break;
	case Key::EE:
		if (m_pKeyBoardState[DIK_E] & 0x80) return true;
		break;
	case Key::EQ:
		if (m_pKeyBoardState[DIK_Q] & 0x80) return true;
		break;
	case Key::ECTRL:
		if ((m_pKeyBoardState[DIK_LCONTROL] & 0x80) || (m_pKeyBoardState[DIK_RCONTROL] & 0x80)) return true;
		break;
	default:
		return false;
	}
	return false;
}