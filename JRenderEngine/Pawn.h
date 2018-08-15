#pragma once
#include"Actor.h"
#include"PresseManager.h"
#include<functional>

namespace JRenderEngine
{
	class ControlComponent;
	class Pawn : public Actor
	{
	private:
		std::unique_ptr<ControlComponent> m_ControlComponent;
	public:
		Pawn();
		virtual ~Pawn();

		virtual bool Update(const float deltaTime);
	protected:
		void BindControlFunc(const JInputEngine::Key& key, const float ValueData,const std::function<void(const float Value)>& func);
		void BindControlAxisFunc(const float ValueData, const std::function<void(const float Value)>& func);
	};
}
