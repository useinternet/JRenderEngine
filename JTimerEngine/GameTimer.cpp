#include<Windows.h>
#include "GameTimer.h"

int GameTimer::m_FPS;

GameTimer::GameTimer()
{
	accTime = 0.0;
	FPSCount = 0;


	m_BaseTime = 0;
	m_PausedTime = 0;
	m_StopTime = 0;
	m_PrevTime = 0;
	m_CurrentTime = 0;
	m_SecondsPerCount = 0;
	m_DeltaTime = 0;
	m_FPS = 0;
	m_Stopped = false;


	__int64 CountsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CountsPerSec);
	m_SecondsPerCount = 1.0 / (double)CountsPerSec;
}


GameTimer::~GameTimer()
{


}


float GameTimer::TotalTime() const
{
	// ���� �ִٸ�
	if (m_StopTime)
	{
		// ( (������ ������ �ð� - �Ͻ������� �ð�) - �������α׷� ���� �ð� ) * �ʴ� ������
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime)*m_SecondsPerCount);
	}
	// Ȱ��ȭ ���̶��
	else
	{
		// ((���� ������ �ð� - �Ͻ������� �ð�) - ���� ���α׷� ���� �ð�) * �ʴ� ������
		return (float)(((m_CurrentTime - m_PausedTime) - m_BaseTime)*m_SecondsPerCount);
	}


}
float GameTimer::DeltaTime() const
{
	return (float)m_DeltaTime;
}
int GameTimer::FPS()
{
	return m_FPS;
}
void GameTimer::Reset()
{
	__int64 CurrentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

	m_BaseTime = CurrentTime;
	m_PrevTime = CurrentTime;
	m_StopTime = 0;
	m_Stopped = false;
}
void GameTimer::Start()
{
	__int64 StartTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	// ���� �ִٸ�
	if (m_Stopped)
	{
		// �Ͻ� ���� �ð� = �Ͻ� �����ð� + ( ���� �ð� - ���� �ð�)
		m_PausedTime += (StartTime - m_StopTime);

		// ���ð�, ���� ���� �ð��ʱ�ȭ �� ���������� false�� �ٲ۴�.
		m_PrevTime = StartTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
}
void GameTimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 CurrentTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

		// ���� �ð��� ����ð�����..
		// ���� ������ true�� �ٲ۴�.
		m_StopTime = CurrentTime;
		m_Stopped = true;
	}
}
void GameTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0f;
		return;
	}

	

	__int64 CurrentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

	m_CurrentTime = CurrentTime;


	// ���� �ð� = (���� �ð� - �� �ð�)*�ʴ� ������
	m_DeltaTime = (m_CurrentTime - m_PrevTime)*m_SecondsPerCount;


	// �ٽ� ���ð��� ����ð��� ����
	m_PrevTime = m_CurrentTime;


	// ������ ���
	FPSCount++;
	accTime += m_DeltaTime;
	if (accTime >= 1.0f)
	{
		m_FPS = FPSCount;

		FPSCount = 0;
		accTime = 0.0f;
	}


	// ���������� deltatime�� ������ �Ǵ� ��찡�־� ������ �ɰ�� ���������� 0���� �Ѵ�.
	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0f;
	}

}