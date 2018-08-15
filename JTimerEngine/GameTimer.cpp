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
	// 멈춰 있다면
	if (m_StopTime)
	{
		// ( (정지됨 시점의 시간 - 일시정지된 시간) - 응용프로그램 시작 시간 ) * 초당 프레임
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime)*m_SecondsPerCount);
	}
	// 활성화 중이라면
	else
	{
		// ((현재 시점의 시간 - 일시정지된 시간) - 응용 프로그램 시작 시간) * 초당 프레임
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

	// 멈춰 있다면
	if (m_Stopped)
	{
		// 일시 정지 시간 = 일시 정지시간 + ( 시작 시간 - 정지 시간)
		m_PausedTime += (StartTime - m_StopTime);

		// 전시간, 정지 시점 시간초기화 및 정지유무를 false로 바꾼다.
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

		// 정지 시간을 현재시간으로..
		// 정지 유무를 true로 바꾼다.
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


	// 사이 시간 = (현재 시간 - 전 시간)*초당 프레임
	m_DeltaTime = (m_CurrentTime - m_PrevTime)*m_SecondsPerCount;


	// 다시 전시간에 현재시간을 저장
	m_PrevTime = m_CurrentTime;


	// 프레임 계산
	FPSCount++;
	accTime += m_DeltaTime;
	if (accTime >= 1.0f)
	{
		m_FPS = FPSCount;

		FPSCount = 0;
		accTime = 0.0f;
	}


	// 간헐적으로 deltatime이 음수가 되는 경우가있어 음수가 될경우 강제적으로 0으로 한다.
	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0f;
	}

}