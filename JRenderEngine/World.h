#pragma once
#include"JRenderEngine.h"
#include<vector>
#include<functional>

/*
���̴��� ���� Ŭ������ ���� ����
������Ʈ�� ī�޶� ���̴� ���� ������.. �׸��� ���� �ϸ鼭 ��� ���� ��������
�ϴ� �Է� ���� ����Ÿ�̸� ���̺귯�� �����ͼ� �����ؼ� ī�޶� �̵��ϴ°ű��� ��������


// ����

*/


namespace JRenderEngine
{
	class GameObject;
	class CameraComponent;
	class ShaderManager;

	class World
	{
	private:
		std::vector<std::unique_ptr<GameObject>> m_vObjects;
		CameraComponent* m_CurrentCameraView;
		ShaderManager*   m_ShaderManager;
		D3DXMATRIX       m_projectionMatrix;
		std::string m_CurrentShader;
	public:
		World();
		~World();

		bool Update(const float deltaTime);
		void InsertGameObject(std::unique_ptr<GameObject> Object);
		std::vector<std::unique_ptr<GameObject>>* GetGameObjectArray_Origin();
		std::vector<GameObject*> GetGameObjectArray();
		std::vector<GameObject*> GetGameObjectArray_if(const std::function<bool(const GameObject& Object)>& func);
		void SetCurrentCameraView(CameraComponent* component);
		void SetCurrentShader(const std::string& ShaderName);
	private:
	
	};

}
