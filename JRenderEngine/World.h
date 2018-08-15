#pragma once
#include"JRenderEngine.h"
#include<vector>
#include<functional>

/*
셰이더와 월드 클래스의 관계 문제
컴포넌트와 카메라 셰이더 변수 설정은.. 그림자 맵핑 하면서 어떻게 할지 결정하자
일단 입력 모듈과 게임타이머 라이브러리 가져와서 장착해서 카메라 이동하는거까지 구현하자


// 보류

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
