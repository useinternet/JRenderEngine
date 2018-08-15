#pragma once
#include <string>

namespace JInputEngine
{

	enum class Key
	{
		// ���� Ű
		E0, E1, E2, E3, E4, E5, E6, E7, E8, E9,
		// F���� Ű
		EF1, EF2, EF3, EF4, EF5, EF6, EF7, EF8, EF9, EF10, EF11, EF12,
		// NumPad Ű
		ENUMLOCK, ENUMPAD0, ENUMPAD1, ENUMPAD2, ENUMPAD3, ENUMPAD4, ENUMPAD5, ENUMPAD6, ENUMPAD7,
		ENUMPAD8, ENUMPAD9, ENUMPADENTER,

		// ��� Ű
		EESCAPE, EDELETE, EEND, EHOME, EINSERT, EPGDN, EPGUP,

		ESHIFT, ECTRL, EALT, ECAPSLOCK, ETAB,

		// ���ĺ� Ű
		EA, EB, EC, ED, EE, EF, EG, EH, EI, EJ, EK, EL, EN, EM, EO, EP, EQ, ER, ES, ET, EU, EV, EW, EX, EY, EZ,
	};
	class PresseManager
	{
		// �ӽ� ����ƽ
		static unsigned char* m_pKeyBoardState;
	public:
		PresseManager(unsigned char* KeyBoardState);
		~PresseManager();

		// �ӽ�.. ���߿� ���� ����
		// Ű �Է� �ޱ�..
		static bool IsPressed(Key key);



	};


}


