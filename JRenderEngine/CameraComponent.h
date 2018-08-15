#pragma once
#include"Component.h"


namespace JRenderEngine
{
	class CameraComponentMessage : ComponentMessage
	{
	public:
		CameraComponentMessage() {}
		virtual ~CameraComponentMessage() {}

		float ReflectionHeight;
	};
	class CameraComponent : public Component
	{
		const float radians = 0.0174532925f;

		D3DXVECTOR3 m_prevRotation;

		std::unique_ptr<D3DXMATRIX> m_ViewMatrix;
		std::unique_ptr<D3DXMATRIX> m_ReflectionMatrix;

		float m_ReflectionHeight;
	public:
		CameraComponent();
		virtual ~CameraComponent();


		virtual bool Update(const float deltaTime);
		virtual void Receive(ComponentMessage* msg);
		D3DXMATRIX& GetViewMatrix();
		D3DXMATRIX& GetReflectionMatrix();
	private:
		void Calculation_ViewMatrix();
		void Calculation_ReflectionMatrix();

	};







}
