#include"ControlComponent.h"

using namespace JRenderEngine;
using namespace JInputEngine;
using namespace std;
#ifdef _DEBUG
int ControlComponent::DebugActiveCount = 0;
#endif

// ���� ���ϰ� �ϱ� ���� typedef �� �̿�
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
		// ť�� ����������� ����������.
		while (!m_ControlFuncQue.empty())
		{
			// �Ǿտ��ִ� ����� Ű�� ������������ �ش� ���� �Ű��������Ͽ� �Լ��� ����
			ControlFunc iter = m_ControlFuncQue.front();
			if (PresseManager::IsPressed(get<0>(iter)))
			{
				get<1>(iter)(get<2>(iter));
			}
			m_ControlFuncQue.pop();
		}
		// ť�� ����������� ������ ����.
		while (!m_ControlAxisFuncQue.empty())
		{
			ControlAxisFunc iter2 = m_ControlAxisFuncQue.front();
			// �Լ� ����
			iter2.first(iter2.second);

			m_ControlAxisFuncQue.pop();
		}
	}
	// Active �� DeActive �� ¦�� �ȸ����� ����׸�忡�� �˸�..
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



		// ������ ī��Ʈ
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
	// �޼��� ó��
}
void ControlComponent::SetControlFunc(const JInputEngine::Key& key, const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlFuncQue.push(ControlFunc(key,func,ValueData));
}
void ControlComponent::SetControlAxisFunc(const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlAxisFuncQue.push(ControlAxisFunc(func, ValueData));
}