#include"MyPawn.h"
#include"CameraComponent.h"
#include"ControlComponent.h"
#include"DirectInput.h"
using namespace JInputEngine;
using namespace JRenderEngine;
using namespace std;

MyPawn::MyPawn() : Pawn()
{
	m_CameraComponent = make_unique<CameraComponent>();
	GameObject::InsertComponent<CameraComponent>(m_CameraComponent.get());

	ControlComponentMessage msg;
	msg.bActive = true;

	Send(&msg);
}
MyPawn::~MyPawn()
{

}
bool MyPawn::Update(const float deltaTime)
{
	Pawn::Update(deltaTime);

	// �� ������ ����

	Pawn::BindControlAxisFunc(DirectInput::GetMouseAxisX() * 0.05f,
		[this](const float Value)
	{
		D3DXVECTOR3 Rotation = GetRotation();
		Rotation.y += Value;
		if (Rotation.y < 0.0f)
		{
			Rotation.y += 360.0f;
		}
		if (Rotation.y > 360.0f)
		{
			Rotation.y -= 360.0f;
		}
		SetRotation(Rotation);
	});
	//  �� ���Ʒ�
	Pawn::BindControlAxisFunc(DirectInput::GetMouseAxisY() * 0.05f,
		[this](const float Value)
	{
		D3DXVECTOR3 Rotation = GetRotation();
		Rotation.x += Value;
		if (Rotation.x < -90.0f)
		{
			Rotation.x = -90.0f;
		}
		if (Rotation.x > 90.0f)
		{
			Rotation.x = 90.0f;
		}
		SetRotation(Rotation);
	});

	// ���� �̵�
	Pawn::BindControlFunc(Key::EA, deltaTime * 20.0f,
		[this](const float Value)
	{
		D3DXVECTOR3 RightVector = GetRightVector();
		RightVector.x *= Value;
		RightVector.z *= Value;

		SetLocation(GetLocation() - RightVector);
	});

	// ������ �̵�
	Pawn::BindControlFunc(Key::ED, deltaTime * 20.0f,
		[this](const float Value)
	{

		D3DXVECTOR3 RightVector = GetRightVector();
		RightVector.x *= Value;
		RightVector.z *= Value;

		SetLocation(GetLocation() + RightVector);
	});

	// ������ �̵�
	Pawn::BindControlFunc(Key::EW, deltaTime * 20.0f,
		[this](const float Value)
	{
		D3DXVECTOR3 ForwardVector = GetForwardVector();
		ForwardVector.x *= Value;
		ForwardVector.z *= Value;

		SetLocation(GetLocation() + ForwardVector);
	});
	// �ڷ� �̵�
	Pawn::BindControlFunc(Key::ES, deltaTime * 20.0f,
		[this](const float Value)
	{
		D3DXVECTOR3 ForwardVector = GetForwardVector();
		ForwardVector.x *= Value;
		ForwardVector.z *= Value;

		SetLocation(GetLocation() - ForwardVector);
	});



	// ���� �̵�
	Pawn::BindControlFunc(Key::EE, deltaTime * 20.0f,
		[this](const float Value)
	{
		D3DXVECTOR3 Location = GetLocation();
		Location.y += Value;
		SetLocation(Location);
	});
	// �Ʒ��� �̵�
	Pawn::BindControlFunc(Key::EQ, deltaTime * 20.0f,
		[this](const float Value)
	{
		D3DXVECTOR3 Location = GetLocation();
		Location.y -= Value;
		SetLocation(Location);
	});
	return true;
}


// ���� ����
D3DXMATRIX MyPawn::GetViewMatrix()
{
	return m_CameraComponent->GetViewMatrix();
}