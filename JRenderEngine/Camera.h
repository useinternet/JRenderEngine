#pragma once
#include"GameObject.h"

namespace JRenderEngine
{
	class CameraComponent;

	class Camera : public GameObject
	{
	private:
		std::unique_ptr<CameraComponent> m_CameraComponent;

	public:
		Camera();
		virtual ~Camera();

		virtual bool Update(const float deltaTime) override;
		const D3DXMATRIX& GetViewMatrix();
	};
}
