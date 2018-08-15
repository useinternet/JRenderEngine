#pragma once
#include"GameObject.h"


namespace JRenderEngine
{
	class RenderComponent;
	class MeshMessage;
	class Actor : public GameObject
	{
	private:
		std::unique_ptr<RenderComponent> m_RenderComponent;
	public:
		Actor();
		virtual ~Actor();

		bool Update(const float deltaTime) override;
		virtual void SetMesh(Mesh* mesh) override;
		void SetCustomMesh(const MeshMessage& mesh, const D3D_PRIMITIVE_TOPOLOGY& format = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	};
}
