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
		// 오브젝트 아이디
		ObjectID m_ID;
		// 메쉬정보 / 위치 / 방향 / 스케일
		// - JLibrary 완성후 대체..
		Mesh* m_Mesh;
		std::unique_ptr<D3DXVECTOR3>  m_Location;
		std::unique_ptr<D3DXVECTOR3>  m_Rotation;
		std::unique_ptr<D3DXVECTOR3>  m_Scale;
		std::unique_ptr<D3DXMATRIX>   m_ObjectMatrix;
		// 컴포넌트 배열
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
		// 다이나믹 캐스트를 이용하여 Component를 부모로하는지 체크
		Component* result = dynamic_cast<Component*>(component);
		if (result == nullptr)
		{
			DebugLog(L"Error : Failed InsertComponent!! \n");
			DebugLog(L"Class : GameObject \n");
			//예외 처리
			return false;
		}
		// 오브젝트와 컴포넌트 연결
		component->CombineObject(this);
		// 삽입
		m_mComponent.insert(pair<ComponentName, Component*>(component->GetName(), move(component)));
		return true;
	}
}

