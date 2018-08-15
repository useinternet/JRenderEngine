#pragma once
#include"JRenderEngine.h"
#include"GameObject.h"


namespace JRenderEngine
{
	class GameObject;

	enum class ComponentName
	{
		Component,
		RenderComponent,
		CameraComponent,
		ControlComponent,
		InputComponent,
		TextComponent
	};
	class ComponentMessage : public Message
	{
	public:
		ComponentMessage() {}
		virtual ~ComponentMessage() {}
	};
	class Component
	{
	private:
		GameObject * m_Owner;
		ComponentName name;

	public:
		Component();
		virtual ~Component();

		virtual bool Update(const float deltaTime);


		// �޼��� �ޱ�...
		virtual void Receive(ComponentMessage* msg);

		// ���� ������Ʈ�� ���� �� ��ȯ
		void CombineObject(GameObject* Object);
		void SetName(const ComponentName& name);
		GameObject* GetOwner();
		ComponentName GetName();

	};
}

