#pragma once
#include"Component.h"
#include"PresseManager.h"
#include<functional>
#include <map>
#include <queue>


namespace JRenderEngine
{
	class ControlComponentMessage : public ComponentMessage
	{
	public:
		ControlComponentMessage() {}
		virtual ~ControlComponentMessage() {}

		bool bActive = false;
	};
	class ControlComponent : public Component
	{
	private:
#ifdef _DEBUG
		static int DebugActiveCount;
#endif
		bool bActive;

		std::queue<
			std::tuple<const JInputEngine::Key, const std::function<void(const float Value)>,const float>
		    > m_ControlFuncQue;
		std::queue <
			std::pair<const std::function<void(const float Value)>, const float>>
			m_ControlAxisFuncQue;
		
	public:
		ControlComponent();
		virtual ~ControlComponent();

		
		virtual bool Update(const float deltaTime) override;
		virtual void Receive(ComponentMessage* msg) override;
		void SetControlFunc(const JInputEngine::Key& key, const float ValueData ,const std::function<void(const float Value)>& func);
		void SetControlAxisFunc(const float ValueData, const std::function<void(const float Value)>& func);
	};

}
