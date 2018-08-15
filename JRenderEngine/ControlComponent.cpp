#include"ControlComponent.h"

using namespace JRenderEngine;
using namespace JInputEngine;
using namespace std;
#ifdef _DEBUG
int ControlComponent::DebugActiveCount = 0;
#endif

// 좀더 편하게 하기 위해 typedef 를 이용
typedef tuple<const Key, const function<void(const float Value)>, const float> ControlFunc;
typedef pair<const std::function<void(const float Value)>, const float>        ControlAxisFunc;


ControlComponent::ControlComponent() : Component()
{
	bActive = false;
	SetName(ComponentName::ControlComponent);
}
ControlComponent::~ControlComponent()
{


}
bool ControlComponent::Update(const float deltaTime)
{
	Component::Update(deltaTime);
	if(bActive)
	{
		// 큐가 비워질때까지 루프를돈다.
		while (!m_ControlFuncQue.empty())
		{
			// 맨앞에있는 요소의 키가 눌러져있으면 해당 값을 매개변수로하여 함수를 실행
			ControlFunc iter = m_ControlFuncQue.front();
			if (PresseManager::IsPressed(get<0>(iter)))
			{
				get<1>(iter)(get<2>(iter));
			}
			m_ControlFuncQue.pop();
		}
		// 큐가 비워질떄까지 루프를 돈다.
		while (!m_ControlAxisFuncQue.empty())
		{
			ControlAxisFunc iter2 = m_ControlAxisFuncQue.front();
			// 함수 실행
			iter2.first(iter2.second);

			m_ControlAxisFuncQue.pop();
		}
	}
	// Active 와 DeActive 가 짝이 안맞으면 디버그모드에서 알림..
#ifdef _DEBUG
	if (DebugActiveCount > 2)
	{
		DebugLog(L"Error : ControlComponent's DebugActiveCount exceed 2!! \n");
	}
	if (DebugActiveCount < 0)
	{
		DebugLog(L"Error : ControlComponent's DebugActiveCount is negative !! \n");
	}
#endif
	return true;
}
void ControlComponent::Receive(ComponentMessage* msg)
{
	Component::Receive(msg);

	ControlComponentMessage* Msg = dynamic_cast<ControlComponentMessage*>(msg);
	if (Msg)
	{
		this->bActive = Msg->bActive;



		// 디버깅용 카운트
#ifdef _DEBUG
		if (Msg->bActive)
		{
			DebugActiveCount++;
		}
		else
		{
			DebugActiveCount--;
		}
#endif
	}
	// 메세지 처리
}
void ControlComponent::SetControlFunc(const JInputEngine::Key& key, const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlFuncQue.push(ControlFunc(key,func,ValueData));
}
void ControlComponent::SetControlAxisFunc(const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlAxisFuncQue.push(ControlAxisFunc(func, ValueData));
}