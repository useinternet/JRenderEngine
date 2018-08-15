#include"GameObject.h"
#include"DirectX.h"
#include"Mesh.h"
using namespace std;
using namespace JRenderEngine;
GameObject::GameObject()
{
	m_ID = ObjectID::NONE;

	DirectX* direct = DirectX::GetInstance();
	if (direct)
	{
		m_pDevice = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}
	m_Location     = make_unique<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);
	m_Rotation     = make_unique<D3DXVECTOR3>(0.0f, 0.0f, 0.0f);
	m_Scale        = make_unique<D3DXVECTOR3>(1.0f, 1.0f, 1.0f);
	m_ObjectMatrix = make_unique<D3DXMATRIX>();
	*m_ObjectMatrix = direct->GetWorldMatrix();
}
GameObject::~GameObject()
{

}
bool GameObject::Update(const float deltaTime)
{
	for (auto& iter : m_mComponent)
	{
		iter.second->Update(deltaTime);
	}
	return true;
}
const ObjectID& GameObject::GetID()    { return m_ID; }
Component*   GameObject::GetComponent(const ComponentName& name) { return m_mComponent[name]; }
Mesh*        GameObject::GetMesh()     { return m_Mesh; }
D3DXVECTOR3& GameObject::GetLocation() { return *m_Location; }
D3DXVECTOR3& GameObject::GetRotation() { return *m_Rotation; }
D3DXVECTOR3& GameObject::GetScale()    { return *m_Scale; }
D3DXMATRIX&  GameObject::GetMatrix()   { return *m_ObjectMatrix; }
const ObjectID&   GameObject::GetObjectID() { return m_ID; }
D3DXVECTOR3       GameObject::GetForwardVector() {
	return D3DXVECTOR3(sinf(TransRadian(m_Rotation->y)), 0, cosf(TransRadian(m_Rotation->y)));
}
D3DXVECTOR3       GameObject::GetRightVector() {
	return D3DXVECTOR3(cosf(TransRadian(m_Rotation->y)), 0,-sinf(TransRadian(m_Rotation->y)));
}
void GameObject::SetMesh(Mesh* mesh) { m_Mesh = mesh; }

void GameObject::SetLocation(const float x, const float y, const float z)
{
	m_Location->x = x;
	m_Location->y = y;
	m_Location->z = z;
}
void GameObject::SetRotation(const float x, const float y, const float z)
{
	m_Rotation->x = x;
	m_Rotation->y = y;
	m_Rotation->z = z;
}
void GameObject::SetScale(const float x, const float y, const float z)
{
	m_Scale->x = x;
	m_Scale->y = y;
	m_Scale->z = z;
}
void GameObject::SetLocation(const D3DXVECTOR3& vector)
{
	*m_Location = vector;
}
void GameObject::SetRotation(const D3DXVECTOR3& rotation)
{
	*m_Rotation = rotation;
}
void GameObject::SetScale(const D3DXVECTOR3& scale)
{
	*m_Scale = scale;
}
void GameObject::SetMatrix(const D3DXMATRIX& matrix)
{
	*m_ObjectMatrix = matrix;
}
void GameObject::SetName(const ObjectID& ID)
{
	m_ID = ID;
}
void GameObject::Send(ComponentMessage* msg)
{
	for (auto& iter : m_mComponent)
	{
		iter.second->Receive(msg);
	}
}