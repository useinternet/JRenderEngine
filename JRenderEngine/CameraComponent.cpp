#include"CameraComponent.h"
using namespace JRenderEngine;
using namespace std;

CameraComponent::CameraComponent()
{
	m_prevRotation = { 9999.0f,9999.0f,9999.0f };
	m_ViewMatrix = make_unique<D3DXMATRIX>();
	m_ReflectionMatrix = make_unique<D3DXMATRIX>();
	SetName(ComponentName::CameraComponent);
}
CameraComponent::~CameraComponent()
{

}
bool CameraComponent::Update(const float deltaTime)
{
	Component::Update(deltaTime);

	D3DXVECTOR3 rotation = GetOwner()->GetRotation();


	Calculation_ViewMatrix();
	Calculation_ReflectionMatrix();

	return true;
}
void CameraComponent::Receive(ComponentMessage* msg)
{
	Component::Receive(msg);

	CameraComponentMessage* Msg = dynamic_cast<CameraComponentMessage*>(msg);
	if (Msg)
	{
		m_ReflectionHeight = Msg->ReflectionHeight;
	}
}
D3DXMATRIX& CameraComponent::GetViewMatrix()
{
	return *m_ViewMatrix;
}
D3DXMATRIX& CameraComponent::GetReflectionMatrix()
{
	return *m_ReflectionMatrix;
}

void CameraComponent::Calculation_ViewMatrix()
{
	D3DXVECTOR3 lookAt = { 0.0f,0.0f,1.0f };
	D3DXVECTOR3 up = { 0.0f,1.0f,0.0f };
	D3DXVECTOR3 position = GetOwner()->GetLocation();//m_Location->GetD3DVector();
	D3DXVECTOR3 rotation = GetOwner()->GetRotation();
	D3DXMATRIX rotationMatrix;

	// ī�޶� �������ִ� ȸ�������� ȸ�� ����� �����.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation.y * radians, rotation.x * radians,
		rotation.z * radians);
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// �ٶ󺸴� ������ ��ġ�� ���߾� ��ȯ��Ų��.
	lookAt = position + lookAt;

	// �� ����� �����.
	D3DXMatrixLookAtLH(m_ViewMatrix.get(), &position, &lookAt, &up);
}
void CameraComponent::Calculation_ReflectionMatrix()
{
	D3DXVECTOR3 lookAt = { 0.0f,0.0f,1.0f };
	D3DXVECTOR3 up = { 0.0f,1.0f,0.0f };
	D3DXVECTOR3 position = GetOwner()->GetLocation();
	position.y = -position.y + (m_ReflectionHeight * 2.0f);
	D3DXVECTOR3 rotation = GetOwner()->GetRotation();
	D3DXMATRIX rotationMatrix;

	// ī�޶� �������ִ� ȸ�������� ȸ�� ����� �����.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, (rotation.y + 180)  * radians, (rotation.x + 180)  * radians,
		(rotation.z + 180) * radians);
	// ī�޶� �������ִ� ȸ�������� ȸ�� ����� �����.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	// �ٶ󺸴� ������ ��ġ�� ���߾� ��ȯ��Ų��.

	lookAt = position + lookAt;

	D3DXMatrixLookAtLH(m_ReflectionMatrix.get(), &position, &lookAt, &up);
}
