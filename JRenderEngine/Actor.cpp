#include"Actor.h"
#include"RenderComponent.h"
using namespace std;
using namespace JRenderEngine;
Actor::Actor() : GameObject()
{
	SetName(ObjectID::Actor);
	m_RenderComponent = make_unique<RenderComponent>();
	GameObject::InsertComponent<RenderComponent>(m_RenderComponent.get());
}
Actor::~Actor()
{

}

bool Actor::Update(const float deltaTime)
{
	GameObject::Update(deltaTime);

	return true;
}
void Actor::SetMesh(Mesh* mesh)
{
	GameObject::SetMesh(mesh);
	RenderComponentMessage msg;
	Send(&msg);
}
void Actor::SetCustomMesh(const MeshMessage& mesh, const D3D_PRIMITIVE_TOPOLOGY& format)
{
	RenderComponentMessage msg;
	msg.bDefault = false;
	msg.MeshMsg = mesh;
	msg.format = format;
	Send(&msg);
}