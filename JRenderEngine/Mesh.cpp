
#include"Mesh.h"
using namespace JRenderEngine;

MeshBuffer::MeshBuffer()
{

}
MeshBuffer::~MeshBuffer()
{

}
void MeshBuffer::Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);

	if ((type == (Vertex_Dynamic | Index_Dynamic)) ||
		(type == (Vertex_Dynamic | Index_Static))  ||
		(type == (Vertex_Static  | Index_Dynamic)) ||
		(type == (Vertex_Static  | Index_Static)))
	{
		Vertex_Index_Render(DeviceContext, format, meshMessage);
		return;
	}
	if ((type == (Vertex_Dynamic | Instance_Dynamic)) ||
		(type == (Vertex_Dynamic | Instance_Static))  ||
		(type == (Vertex_Static  | Instance_Dynamic)) ||
		(type == (Vertex_Static  | Instance_Static)))
	{
		Vertex_Instance_Render(DeviceContext, format, meshMessage);
		return;
	}
	DebugLog(L"Error : This This Class does not accept an instance type \n");
	DebugLog(L"Class : MeshBuffer");
}
void MeshBuffer::CreateBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);

	if ((type == (Vertex_Dynamic | Index_Dynamic)) ||
		(type == (Vertex_Dynamic | Index_Static))  ||
		(type == (Vertex_Static  | Index_Dynamic)) ||
		(type == (Vertex_Static  | Index_Static)))
	{
		CreateVertexBuffer(pDevice, meshMessage);
		CreateIndexBuffer(pDevice, meshMessage);
		return;
	}
	if ((type == (Vertex_Dynamic | Instance_Dynamic)) ||
		(type == (Vertex_Dynamic | Instance_Static )) ||
		(type == (Vertex_Static  | Instance_Dynamic)) ||
		(type == (Vertex_Static  | Instance_Static)  ))
	{
		CreateVertexBuffer(pDevice, meshMessage);
		CreateInstanceBuffer(pDevice, meshMessage);
		return;
	}
	DebugLog(L"Error : This This Class does not accept an instance type \n");
	DebugLog(L"Class : MeshBuffer");
}

ID3D11Buffer* MeshBuffer::GetVertexBuffer()   { return m_VertexBuffer.Get(); }
ID3D11Buffer* MeshBuffer::GetIndexBuffer()    { return m_IndexBuffer.Get();  }
ID3D11Buffer* MeshBuffer::GetInstanceBuffer() { return m_InstanceBuffer.Get(); }

// ���� �κ� �ٸ���
void MeshBuffer::Vertex_Index_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage)
{
	unsigned int stride;      // ��Ʈ���̵� : �ش� ���� ���ۿ��� ���� ����� ũ��
	unsigned int offset;      // ������  :  ���� ������ ù��° ��ҿ� ���� ù��° ��� ������ ����Ʈ ��

	stride = meshMessage.VertexSize;  // �����°� �������̵� ���� �ʱ�ȭ �Ѵ�
	offset = 0;
	// ù�����迭 / ���������Ǽ� / �������� / ��Ʈ���̵� / ������
	// �������� �ʿ��� �������� �Է��Ѵ�.(����)
	DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// �ε������ۿ� ����������, ������
	// �������� �ʿ��� �������� �Է��մϴ�.(�ε���)
	DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	DeviceContext->IASetPrimitiveTopology(format);
}
void MeshBuffer::Vertex_Instance_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage)
{
	unsigned int stride[2] = { meshMessage.VertexSize, meshMessage.InstanceSize };
	unsigned int offset[2] = { 0,0 };

	// ������ �迭�� ���� ���ۿ� �ν��Ͻ� ���۷� ����
	ID3D11Buffer* bufferPointer[2] = { m_VertexBuffer.Get(), m_InstanceBuffer.Get() };

	// ������ �Ҽ� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	DeviceContext->IASetVertexBuffers(0, 2, bufferPointer, stride, offset);

	// �̲����� ���ۿ��� ������ �ؾ��ϴ� ������Ƽ�� ������ ����
	DeviceContext->IASetPrimitiveTopology(format);
}
void MeshBuffer::CreateVertexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	// ���� ���� ��� ����
	// Ÿ�Կ� ���� ���̳��� / ����Ʈ ����
	if ((type == (Vertex_Dynamic | Index_Dynamic))    ||
		(type == (Vertex_Dynamic | Index_Static))     ||
		(type == (Vertex_Dynamic | Instance_Dynamic)) ||
		(type == (Vertex_Dynamic | Instance_Static)))
	{
		vertexBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	if ((type == (Vertex_Static | Index_Dynamic))    ||
		(type == (Vertex_Static | Index_Static))     ||
		(type == (Vertex_Static | Instance_Dynamic)) ||
		(type == (Vertex_Static | Instance_Static)))
	{
		vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.CPUAccessFlags = 0;
	}
	vertexBufferDesc.ByteWidth = meshMessage.VertexSize * meshMessage.VertexElementSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// ���� ������ ����
	vertexData.pSysMem = meshMessage.VertexType;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ����
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf());

}
void MeshBuffer::CreateIndexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;


	// �ε��� ���� ����
	if ((type == (Vertex_Dynamic | Index_Dynamic)) ||
		(type == (Vertex_Static | Index_Dynamic)))
	{
		indexBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	if ((type == (Vertex_Dynamic | Index_Static)) ||
		(type == (Vertex_Static | Index_Static)))
	{
		indexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		indexBufferDesc.CPUAccessFlags = 0;
	}
	indexBufferDesc.ByteWidth = meshMessage.IndexSize * meshMessage.IndexElementSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;


	// �ε��� ������ ����
	indexData.pSysMem = meshMessage.IndexType;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// ���� ����
	pDevice->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.GetAddressOf());
}
void MeshBuffer::CreateInstanceBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC InstanceBufferDesc;
	D3D11_SUBRESOURCE_DATA InstanceData;


	// �ν��Ͻ� ���� ��� ����
	// Ÿ�Կ� ���� ���̳��� / ����Ʈ ����
	if ((type == (Vertex_Dynamic | Instance_Dynamic)) ||
		(type == (Vertex_Static  | Instance_Dynamic)))
	{
		InstanceBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
		InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	if ((type == (Vertex_Dynamic | Instance_Static)) ||
		(type == (Vertex_Static  | Instance_Static)))
	{
		InstanceBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		InstanceBufferDesc.CPUAccessFlags = 0;
	}
	InstanceBufferDesc.ByteWidth = meshMessage.InstanceSize * meshMessage.InstanceElementSize;
	InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	InstanceBufferDesc.MiscFlags = 0;
	InstanceBufferDesc.StructureByteStride = 0;


	// ���� ������ ����
	InstanceData.pSysMem = meshMessage.InstanceType;
	InstanceData.SysMemPitch = 0;
	InstanceData.SysMemSlicePitch = 0;

	// ���� ����
	pDevice->CreateBuffer(&InstanceBufferDesc, &InstanceData, m_InstanceBuffer.GetAddressOf());
}