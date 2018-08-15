#include"RenderComponent.h"
#include"DirectX.h"
using namespace JRenderEngine;
using namespace std;

RenderComponent::RenderComponent() : Component()
{
	DirectX* direct = DirectX::GetInstance();
	if (direct)
	{
		m_pDevice        = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}

	// 이름 설정
	Component::SetName(ComponentName::RenderComponent);
}
RenderComponent::~RenderComponent()
{

}
bool RenderComponent::Update(const float deltaTime)
{
	Component::Update(deltaTime);

	bool bTrans = false;
	D3DXMATRIX  locationMatrix, rotationMatrix, scaleMatrix;
	D3DXMatrixIdentity(&locationMatrix);
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&scaleMatrix);
	D3DXVECTOR3 location = GetOwner()->GetLocation();
	D3DXVECTOR3 rotation = GetOwner()->GetRotation();
	D3DXVECTOR3 scale    = GetOwner()->GetScale();


	if (location != m_prevLocation)
	{
		bTrans = true;
		m_prevLocation = location;
		D3DXMatrixTranslation(&locationMatrix, location.x, location.y, location.z);
	}
	if (rotation != m_prevRotation)
	{
		bTrans = true;
		m_prevRotation = rotation;
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y, rotation.x, rotation.z);
	}
	if (scale != m_prevScale)
	{
		bTrans = true;
		m_prevScale = scale;
		D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	}
	if (bTrans)
	{
		GetOwner()->SetMatrix(scaleMatrix * rotationMatrix * locationMatrix);
		// 행렬 셋팅
	}

	// 렌더링
	if (m_MeshBuffer)
	{
		m_MeshBuffer->Render(m_pDeviceContext, m_Format, m_MeshMsg);
	}


	return true;
}
void RenderComponent::Receive(ComponentMessage* msg)
{
	Component::Receive(msg);
	RenderComponentMessage* Message = dynamic_cast<RenderComponentMessage*>(msg);
	if (Message)
	{
		if (Message->bDefault)
		{
			SetRenderSetting();
		}
		else
		{
			SetRenderSetting(Message->MeshMsg, Message->format);
		}
	}
}

void RenderComponent::SetRenderSetting(const MeshType& type, const D3D_PRIMITIVE_TOPOLOGY& format)
{

	if ((type == (Vertex_Dynamic | Instance_Dynamic)) ||
		(type == (Vertex_Dynamic | Instance_Static)) ||
		(type == (Vertex_Static | Instance_Dynamic)) ||
		(type == (Vertex_Static | Instance_Static)))
	{
		DebugLog(L"Error : This This function does not accept an instance type. Use other functions that support instance types. \n");
		DebugLog(L"Class : RenderComponent \n");
		DebugLog(L"Func  : SetRenderSetting(const D3D_PRIMITIVE_TOPOLOGY& format, const MeshType& type) \n");
		return;
	}
	if (m_MeshBuffer)
	{
		m_MeshBuffer.reset();
	}
	// 메쉬 버퍼 
	m_MeshBuffer = make_unique<MeshBuffer>();
	// 렌더링을 위한 설정.
	m_Format = format;
	m_MeshMsg.Flags = type;
	m_MeshMsg.VertexSize = sizeof(Vertex);
	m_MeshMsg.VertexType = &(GetOwner()->GetMesh()->Vertecies[0]);
	m_MeshMsg.VertexElementSize = GetOwner()->GetMesh()->Vertecies.size();

	m_MeshMsg.IndexSize = sizeof(Index);
	m_MeshMsg.IndexType = &(GetOwner()->GetMesh()->Indecies[0]);
	m_MeshMsg.IndexElementSize = GetOwner()->GetMesh()->Indecies.size();

	m_MeshBuffer->CreateBuffer(m_pDevice, m_MeshMsg);

	ObjectID ID = GetOwner()->GetID();

}
void RenderComponent::SetRenderSetting(const MeshMessage& msg, const D3D_PRIMITIVE_TOPOLOGY& format)
{
	if (m_MeshBuffer)
	{
		m_MeshBuffer.reset();
	}
	// 메쉬 버퍼 
	m_MeshBuffer = make_unique<MeshBuffer>();
	m_Format = format;
	m_MeshMsg = msg;

	m_MeshBuffer->CreateBuffer(m_pDevice, m_MeshMsg);
}