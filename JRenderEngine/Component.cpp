#include"Component.h"
using namespace JRenderEngine;

Component::Component()
{
	name = ComponentName::Component;
}
Component::~Component()
{

}



bool Component::Update(const float deltaTime)
{
	if (!GetOwner())
	{
		DebugLog(L"Error : Component's Owner is nullptr!! \n");
		DebugLog(L"Class : Component \n");
		DebugLog(L"Func  : Update() \n");
		// 예외처리
		return false;
	}
	return true;
}

void Component::Receive(ComponentMessage* msg)
{
	// 메시지 처리
	return;
}
// 게임 오브젝트와 연결 및 반환
void Component::CombineObject(GameObject* Object)  { m_Owner = Object; }
void Component::SetName(const ComponentName& name) { this->name = name; }
GameObject* Component::GetOwner()  { return m_Owner; }
ComponentName Component::GetName() { return name; }