#pragma once
#include"JRenderEngine.h"
#include"Component.h"
#include"Mesh.h"

namespace JRenderEngine
{
	class RenderComponentMessage : public ComponentMessage
	{
	public:
		RenderComponentMessage() {}
		virtual ~RenderComponentMessage() {}

		bool bDefault = true;
		MeshMessage MeshMsg;
		D3D_PRIMITIVE_TOPOLOGY format = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	};
	class RenderComponent : public Component
	{
	private:
		ID3D11Device * m_pDevice;
		ID3D11DeviceContext*   m_pDeviceContext;
		std::unique_ptr<MeshBuffer> m_MeshBuffer;
		D3D_PRIMITIVE_TOPOLOGY m_Format;
		MeshMessage m_MeshMsg;

		D3DXVECTOR3 m_prevLocation;
		D3DXVECTOR3 m_prevRotation;
		D3DXVECTOR3 m_prevScale;
	public:
		RenderComponent();
		virtual ~RenderComponent();
		virtual bool Update(const float deltaTime) override;
		virtual void Receive(ComponentMessage* msg)override;

	private:
		void SetRenderSetting(const MeshType& type = (Vertex_Dynamic | Index_Static), const D3D_PRIMITIVE_TOPOLOGY& format = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void SetRenderSetting(const MeshMessage& msg, const D3D_PRIMITIVE_TOPOLOGY& format = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	};

}

