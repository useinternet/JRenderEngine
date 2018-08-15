
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

// 여기 부분 다르고
void MeshBuffer::Vertex_Index_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage)
{
	unsigned int stride;      // 스트라이드 : 해당 정점 버퍼에서 사용될 요소의 크기
	unsigned int offset;      // 오프셋  :  정점 버퍼의 첫번째 요소와 사용될 첫번째 요소 사이의 바이트 수

	stride = meshMessage.VertexSize;  // 오프셋과 스프라이드 값을 초기화 한다
	offset = 0;
	// 첫정점배열 / 정점버퍼의수 / 정점버퍼 / 스트라이드 / 오프셋
	// 렌더링에 필요한 변수들을 입력한다.(정점)
	DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	// 인덱스버퍼와 데이터형식, 오프셋
	// 렌더링에 필요한 변수들을 입력합니다.(인덱스)
	DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	DeviceContext->IASetPrimitiveTopology(format);
}
void MeshBuffer::Vertex_Instance_Render(ID3D11DeviceContext* DeviceContext, D3D_PRIMITIVE_TOPOLOGY format, const MeshMessage& meshMessage)
{
	unsigned int stride[2] = { meshMessage.VertexSize, meshMessage.InstanceSize };
	unsigned int offset[2] = { 0,0 };

	// 포인터 배열을 정점 버퍼와 인스턴스 버퍼로 설정
	ID3D11Buffer* bufferPointer[2] = { m_VertexBuffer.Get(), m_InstanceBuffer.Get() };

	// 렌더링 할수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정
	DeviceContext->IASetVertexBuffers(0, 2, bufferPointer, stride, offset);

	// 이꼭지점 버퍼에서 렌더링 해야하는 프리미티브 유형을 설정
	DeviceContext->IASetPrimitiveTopology(format);
}
void MeshBuffer::CreateVertexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	// 정점 버퍼 목록 셋팅
	// 타입에 따라 다이나믹 / 디폴트 설정
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


	// 정점 데이터 저장
	vertexData.pSysMem = meshMessage.VertexType;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 버퍼 생성
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf());

}
void MeshBuffer::CreateIndexBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;


	// 인덱스 버퍼 셋팅
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


	// 인덱스 데이터 저장
	indexData.pSysMem = meshMessage.IndexType;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 버퍼 생성
	pDevice->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.GetAddressOf());
}
void MeshBuffer::CreateInstanceBuffer(ID3D11Device* pDevice, const MeshMessage& meshMessage)
{
	MeshType type = static_cast<MeshType>(meshMessage.Flags);
	D3D11_BUFFER_DESC InstanceBufferDesc;
	D3D11_SUBRESOURCE_DATA InstanceData;


	// 인스턴스 버퍼 목록 셋팅
	// 타입에 따라 다이나믹 / 디폴트 설정
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


	// 정점 데이터 저장
	InstanceData.pSysMem = meshMessage.InstanceType;
	InstanceData.SysMemPitch = 0;
	InstanceData.SysMemSlicePitch = 0;

	// 버퍼 생성
	pDevice->CreateBuffer(&InstanceBufferDesc, &InstanceData, m_InstanceBuffer.GetAddressOf());
}