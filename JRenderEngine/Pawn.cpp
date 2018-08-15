#include"Pawn.h"
#include"ControlComponent.h"
using namespace JRenderEngine;
using namespace std;

Pawn::Pawn()
{
	SetName(ObjectID::Pawn);
	m_ControlComponent = make_unique<ControlComponent>();

	GameObject::InsertComponent<ControlComponent>(m_ControlComponent.get());
}
Pawn::~Pawn()
{

}
bool Pawn::Update(const float deltaTime)
{
	Actor::Update(deltaTime);

	return true;
}
void Pawn::BindControlFunc(const JInputEngine::Key& key, const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlComponent->SetControlFunc(key, ValueData, func);
}
void Pawn::BindControlAxisFunc(const float ValueData, const std::function<void(const float Value)>& func)
{
	m_ControlComponent->SetControlAxisFunc(ValueData, func);
}

