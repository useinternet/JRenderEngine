#pragma once
#include"Pawn.h"

namespace JRenderEngine
{
	class CameraComponent;
	class MyPawn : public Pawn
	{
	private:
		std::unique_ptr<CameraComponent> m_CameraComponent;
	public:
		MyPawn();
		virtual ~MyPawn();

		virtual bool Update(const float deltaTime);


		// 임시 추후 수정
		D3DXMATRIX GetViewMatrix();
	};
}