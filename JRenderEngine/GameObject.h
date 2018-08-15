#pragma once
#include"JRenderEngine.h"

#include"Component.h"
#include<map>

namespace JRenderEngine
{
	enum class ObjectID
	{
		NONE,
		Actor, Text, Pawn, Terrain,
		Camera, 
		Light
	};
	enum class ComponentName;
	class ComponentMessage;
	class Component;
	class Mesh;


	class GameObject
	{
		// ������Ʈ ���̵�
		ObjectID m_ID;
		// �޽����� / ��ġ / ���� / ������
		// - JLibrary �ϼ��� ��ü..
		Mesh* m_Mesh;
		std::unique_ptr<D3DXVECTOR3>  m_Location;
		std::unique_ptr<D3DXVECTOR3>  m_Rotation;
		std::unique_ptr<D3DXVECTOR3>  m_Scale;
		std::unique_ptr<D3DXMATRIX>   m_ObjectMatrix;
		// ������Ʈ �迭
		std::map<const ComponentName, Component*> m_mComponent;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
	public:
		GameObject();
		virtual ~GameObject();
		virtual bool Update(const float deltaTime);
		const ObjectID& GetID();
		Component*   GetComponent(const ComponentName& name);
		Mesh* GetMesh();
		D3DXVECTOR3& GetLocation();
		D3DXVECTOR3& GetRotation();
		D3DXVECTOR3& GetScale();
		D3DXMATRIX&  GetMatrix();
		const ObjectID&    GetObjectID();
		D3DXVECTOR3  GetForwardVector();
		D3DXVECTOR3  GetRightVector();

		virtual void SetMesh(Mesh* mesh);
		void SetLocation(const float x, const float y, const float z);
		void SetRotation(const float x, const float y, const float z);
		void SetScale(const float x, const float y, const float z);
		void SetLocation(const D3DXVECTOR3& vector);
		void SetRotation(const D3DXVECTOR3& rotation);
		void SetScale(const D3DXVECTOR3& scale);
		void SetMatrix(const D3DXMATRIX& matrix);
	protected:
		template<class ComponentType>
		bool InsertComponent(ComponentType* component);
		void SetName(const ObjectID& ID);
		void Send(ComponentMessage* msg);

	};

	template<class ComponentType>
	bool GameObject::InsertComponent(ComponentType* component)
	{
		// ���̳��� ĳ��Ʈ�� �̿��Ͽ� Component�� �θ���ϴ��� üũ
		Component* result = dynamic_cast<Component*>(component);
		if (result == nullptr)
		{
			DebugLog(L"Error : Failed InsertComponent!! \n");
			DebugLog(L"Class : GameObject \n");
			//���� ó��
			return false;
		}
		// ������Ʈ�� ������Ʈ ����
		component->CombineObject(this);
		// ����
		m_mComponent.insert(pair<ComponentName, Component*>(component->GetName(), move(component)));
		return true;
	}
}

