#include"Camera.h"
#include"CameraComponent.h"
using namespace std;
using namespace JRenderEngine;

Camera::Camera() : GameObject()
{
	SetName(ObjectID::Camera);
	m_CameraComponent = make_unique<CameraComponent>();
	GameObject::InsertComponent<CameraComponent>(m_CameraComponent.get());
}
Camera::~Camera()
{

}

bool Camera::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);

	return true;
}

const D3DXMATRIX& Camera::GetViewMatrix()
{
	return m_CameraComponent->GetViewMatrix();
}