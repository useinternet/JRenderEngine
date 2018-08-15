#pragma once






class GameTimer
{
private:
	double accTime;
	int FPSCount;
private:
	/**  시간 변수
	@param m_BaseTime   응용 프로그램 시작 시간
	@param m_PausedTime 타이머가 일시정지된 시간
	@param m_StopTime   타이머가 정지된 시점의 시간
	@param m_PrevTime   전 타이머 시간 
	@param m_CurrentTime 현재 타이머 시간
	@param m_SecondsPerCount 초당 프레임 
	@param m_DeltaTime  사이 시간
	@param m_FPS       프레임
	@param m_Stopped   정지 되어있는가? */
	__int64 m_BaseTime;
	__int64 m_PausedTime;
	__int64 m_StopTime;
	__int64 m_PrevTime;
	__int64 m_CurrentTime;

	double m_SecondsPerCount;
	double m_DeltaTime;
	static int m_FPS;
	bool m_Stopped;
public:
	GameTimer();
	~GameTimer();

	/** 
	@function TotalTime     현재 시간 반환
	@function DeltaTime     사이 시간 반환
	@function Reset         시간 리셋
	@function Start         타이머 시작
	@function Stop          타이머 정지
	@function Tick          타이머 틱   */
	float TotalTime() const;
	float DeltaTime() const;
	static int FPS();
	void Reset();
	void Start();
	void Stop();
	void Tick();
};

