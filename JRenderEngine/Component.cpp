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
		// ����ó��
		return false;
	}
	return true;
}

void Component::Receive(ComponentMessage* msg)
{
	// �޽��� ó��
	return;
}
// ���� ������Ʈ�� ���� �� ��ȯ
void Component::CombineObject(GameObject* Object)  { m_Owner = Object; }
void Component::SetName(const ComponentName& name) { this->name = name; }
GameObject* Component::GetOwner()  { return m_Owner; }
ComponentName Component::GetName() { return name; }