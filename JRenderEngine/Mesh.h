#pragma once
#include "JRenderEngine.h"
#include<vector>

namespace JRenderEngine
{
	// �޽����۸� ��������� �޼���
	class MeshMessage
	{
	public:
		unsigned Flags;
		size_t   VertexSize = 0;
		size_t   IndexSize = 0;
		size_t   InstanceSize = 0;

		void*    VertexType = nullptr;
		void*    IndexType = nullptr;
		void*    InstanceType = nullptr;

		size_t   VertexElementSize = 0;
		size_t   IndexElementSize = 0;
		size_t   InstanceElementSize = 0;
	};
	// �޽�Ÿ��
	enum MeshType
	{
		None = 0x001,
		Vertex_Dynamic = 0x002,
		Vertex_Static = 0x004,
		Index_Dynamic = 0x008,
		Index_Static = 0x010,
		Instance_Dynamic = 0x020,
		Instance_Static = 0x040
	};
	ENABLE_BITMASK_OPERATORS(MeshType)

		// �⺻ Ÿ�� - ���·ε��Ҷ� ���� Ÿ��
		typedef struct Vertex
	{
		D3DXVECTOR3 position = { 0.0f,0.0f,0.0f };
		D3DXVECTOR2 texture = { 0.0f,0.0f };
		D3DXVECTOR3 normal = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 tangent = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 binormal = { 0.0f,0.0f,0.0f };
	}Vertex;
	typedef unsigned int Index;
	class Mesh
	{
	public:
		std::vector<Vertex> Vertecies;
		std::vector<Index> Indecies;
	};

	// �޽� ����
	class MeshBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;
	public:
		MeshBuffer();
		~MeshBuffer();
		void Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage);
		void CreateBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage);

		ID3D11Buffer* GetVertexBuffer();
		ID3D11Buffer* GetIndexBuffer();
		ID3D11Buffer* GetInstanceBuffer();
	private:
		void Vertex_Index_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage);
		void Vertex_Instance_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage);
		void CreateVertexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage);
		void CreateIndexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage);
		void CreateInstanceBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage);
	};
}

