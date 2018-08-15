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


		// 메세지 받기...
		virtual void Receive(ComponentMessage* msg);

		// 게임 오브젝트와 연결 및 반환
		void CombineObject(GameObject* Object);
		void SetName(const ComponentName& name);
		GameObject* GetOwner();
		ComponentName GetName();

	};
}

