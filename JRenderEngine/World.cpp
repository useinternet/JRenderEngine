#include"World.h"
#include"TextureLoader.h"
#include"DirectX.h"
#include"GameObject.h"
#include"ShaderManager.h"
#include"CameraComponent.h"
#include"Mesh.h"
using namespace std;
using namespace JRenderEngine;


World::World()
{
	DirectX* direct = DirectX::GetInstance();
	if (direct)
	{
		m_projectionMatrix = direct->GetProjectionMatrix();
	}
	m_ShaderManager = ShaderManager::GetInstance();
}
World::~World()
{

}

bool World::Update(const float deltaTime)
{
	for (auto& iter : m_vObjects)
	{
		iter->Update(deltaTime);
		if (m_CurrentShader == "TextureShader")
		{
			JMessage msg = m_ShaderManager->CreateTextureShaderDesc(
				iter->GetMatrix(), m_CurrentCameraView->GetViewMatrix(), m_projectionMatrix,
				iter->GetMesh()->Indecies.size(), TextureLoader::Load("Dirt_DM"));
			m_ShaderManager->Render(m_CurrentShader, msg);
		}
	}

	return true;
}
void World::InsertGameObject(unique_ptr<GameObject> Object)
{
	m_vObjects.push_back(move(Object));
}
std::vector<std::unique_ptr<GameObject>>* World::GetGameObjectArray_Origin()
{
	return &m_vObjects;
}
vector<GameObject*> World::GetGameObjectArray()
{
	vector<GameObject*> tmpVector;
	for (auto& iter : m_vObjects)
	{
		tmpVector.push_back(iter.get());
	}
	return tmpVector;
}
vector<GameObject*> World::GetGameObjectArray_if(const function<bool(const GameObject& Object)>& func)
{
	vector<GameObject*> tmpVector;
	for (auto& iter : m_vObjects)
	{
		if (func(*iter))
		{
			tmpVector.push_back(iter.get());
		}
	}
	return tmpVector;
}
void World::SetCurrentCameraView(CameraComponent* component)
{
	m_CurrentCameraView = component;
}

void World::SetCurrentShader(const std::string& ShaderName)
{
	m_CurrentShader = ShaderName;
}