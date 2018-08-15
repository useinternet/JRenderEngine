#pragma once






class GameTimer
{
private:
	double accTime;
	int FPSCount;
private:
	/**  �ð� ����
	@param m_BaseTime   ���� ���α׷� ���� �ð�
	@param m_PausedTime Ÿ�̸Ӱ� �Ͻ������� �ð�
	@param m_StopTime   Ÿ�̸Ӱ� ������ ������ �ð�
	@param m_PrevTime   �� Ÿ�̸� �ð� 
	@param m_CurrentTime ���� Ÿ�̸� �ð�
	@param m_SecondsPerCount �ʴ� ������ 
	@param m_DeltaTime  ���� �ð�
	@param m_FPS       ������
	@param m_Stopped   ���� �Ǿ��ִ°�? */
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
	@function TotalTime     ���� �ð� ��ȯ
	@function DeltaTime     ���� �ð� ��ȯ
	@function Reset         �ð� ����
	@function Start         Ÿ�̸� ����
	@function Stop          Ÿ�̸� ����
	@function Tick          Ÿ�̸� ƽ   */
	float TotalTime() const;
	float DeltaTime() const;
	static int FPS();
	void Reset();
	void Start();
	void Stop();
	void Tick();
};

