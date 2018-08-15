
#include "RenderEngine.h"
#include "DirectX.h"
// ����// �ؽ��� //���� //��Ʈ �δ�..
#include "AssetLoader.h"
#include "TextureLoader.h"
#include "FontLoader.h"
#include "TerrainMapLoader.h"

// �ӽ�
#include "ShaderManager.h"
#include "GameTimer.h" 
#include "Terrain.h"
using namespace std;
using namespace JRenderEngine;
RenderEngine::RenderEngine()
{
	
	
}
RenderEngine::~RenderEngine()
{
	
}
bool RenderEngine::Initialization(HWND hWnd, int nScreenWidth, int nScreenHeight)
{
	bool result = true;
	// ���̷�Ʈ x
	m_DirectX = make_unique<DirectX>();
	result = m_DirectX->Initialization(nScreenWidth, nScreenHeight, hWnd, FULLSCREEN, FARZ, NEARZ);
	if (!result)
	{
		MessageBox(hWnd, L"DirectXŬ���� �ʱ�ȭ�� �����߽��ϴ�!!", L"Error", MB_OK);
		return false;
	}
	// ���� �δ�
	m_Asset = make_unique<AssetLoader>();
	result = m_Asset->AssetInit(hWnd);
	if (!result)
	{
		DebugLog(L"Error : Failed Asset Initialization! \n");
	}
	//�ؽ��� �δ�
	m_TextureLoader = make_unique<TextureLoader>();
	result = m_TextureLoader->TextureInit(); 
	if (!result)
	{
		DebugLog(L"Error : Failed TextureLoader Initialization!! \n");
	}
	// ��Ʈ �δ� 
	m_FontLoader = make_unique<FontLoader>();
	result = m_FontLoader->FontInit();
	if (!result)
	{
		DebugLog(L"Error : Failed FontLoader Initialization! \n");
	}
	// ���� �� �δ� ( ���̸� Raw �� ���..)
	m_TerrainMapLoader = make_unique <TerrainMapLoader>();
	m_TerrainMapLoader->TerrainMapInit();
	// ���̴� ������ 
	m_ShaderManager = make_unique<ShaderManager>();
	result = m_ShaderManager->ShaderManagerInit();
	if (!result)
	{
		DebugLog(L"Error : Failed m_ShaderManager Initialization!! \n");
	}


	// �ӽ�

	m_Wall = make_unique<Actor>();
	m_Wall->SetMesh(AssetLoader::Load("Wall"));
	m_Wall->SetLocation(0.0f, 6.0f, 8.0f);

	m_Bath = make_unique<Actor>();
	m_Bath->SetMesh(AssetLoader::Load("Bath"));
	m_Bath->SetLocation(0.0f, 2.0f, 0.0f);

	m_Ground = make_unique<Actor>();
	m_Ground->SetMesh(AssetLoader::Load("Ground"));
	m_Ground->SetLocation(0.0f, 1.0f, 0.0f);

	m_Pawn = make_unique<MyPawn>();
	m_Pawn->SetLocation(0.0f, 5.0f, -10.0f);
	m_Pawn->Update(0.0f);
	m_2DviewMatrix = m_Pawn->GetViewMatrix();

	// �ؽ�Ʈ
	m_FpsText = make_unique<Text>();
	m_FpsText->SetLocation(-900.0f, 500.0f,0.0f);

	// �ؽ�Ʈ
	m_CharacterPositionText = make_unique<Text>();
	m_CharacterPositionText->SetLocation(-900.0f, 450.0f, 0.0f);

	// �ؽ�Ʈ
	m_CharacterRotationText = make_unique<Text>();
	m_CharacterRotationText->SetLocation(-900.0f, 400.0f, 0.0f);
	
	// ����
	/*m_Terrain = make_unique<Terrain>();
	m_Terrain->SetGrid(100, 100);
	m_Terrain->SetLocation(-50, 0.0,-50);*/
	return true;
}
bool RenderEngine::Update(const float deltaTime)
{
	static float AccTime = 0.0f;
	AccTime += deltaTime;
	if (AccTime > 0.1f)
	{
		AccTime = 0.0f;
		DebugLogOnScreen();
	}

	D3DXMATRIX projectionMatrix = m_DirectX->GetProjectionMatrix();


	m_DirectX->BenginDraw(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	// ����� �� ������Ʈ
	m_Pawn->Update(deltaTime);


	// ��
	m_Wall->Update(deltaTime);
	JMessage msg = m_ShaderManager->CreateAmbientLightShaderDesc(
		m_Wall->GetMatrix(), m_Pawn->GetViewMatrix(), projectionMatrix,
		m_Wall->GetMesh()->Indecies.size(), TextureLoader::Load("Wall_DM"), m_Pawn->GetLocation());
	m_ShaderManager->Render("AmbientLightShader", msg);


	// ����
	/*m_Terrain->Update(deltaTime);
	JMessage msg2 = m_ShaderManager->CreateTextureShaderDesc(
		m_Terrain->GetMatrix(), m_Pawn->GetViewMatrix(), projectionMatrix,
		m_Terrain->GetIndexSize(), TextureLoader::Load("Fieldstone_DM"));
	m_ShaderManager->Render("TextureShader", msg2);*/
	m_Ground->Update(deltaTime);
	JMessage msg2 = m_ShaderManager->CreateAmbientLightShaderDesc(
		m_Ground->GetMatrix(), m_Pawn->GetViewMatrix(), projectionMatrix,
		m_Ground->GetMesh()->Indecies.size(), TextureLoader::Load("Dirt_DM"), m_Pawn->GetLocation());
	m_ShaderManager->Render("AmbientLightShader", msg2);

	// ����
	m_Bath->Update(deltaTime);
	JMessage msg3 = m_ShaderManager->CreateAmbientLightShaderDesc(
		m_Bath->GetMatrix(), m_Pawn->GetViewMatrix(), projectionMatrix,
		m_Bath->GetMesh()->Indecies.size(), TextureLoader::Load("Marble_DM"), m_Pawn->GetLocation());
	m_ShaderManager->Render("AmbientLightShader", msg3);


	// �ؽ�Ʈ ������
	TextRender(deltaTime);

	m_DirectX->EndDraw();


	Frame();
	Render();
	return true;
}
void RenderEngine::Frame()
{

}
void RenderEngine::Render()
{

}
void RenderEngine::TextRender(const float deltaTime)
{
	D3DXMATRIX OrthoMatrix = m_DirectX->GetOrthoMatrix();
	m_DirectX->TurnZBufferOff();
	m_DirectX->TurnOnAlphaBlending();

	// ���� ������
	m_FpsText->Update(deltaTime);

	JMessage msg = m_ShaderManager->CreateTextureShaderDesc(
		m_FpsText->GetMatrix(), m_2DviewMatrix, OrthoMatrix,
		m_FpsText->GetTextIndexSize(), m_FpsText->GetFontTexture());
	m_ShaderManager->Render("TextShader", msg);

	// ���� ������
	m_CharacterPositionText->Update(deltaTime);

	JMessage msg2 = m_ShaderManager->CreateTextureShaderDesc(
		m_CharacterPositionText->GetMatrix(), m_2DviewMatrix, OrthoMatrix,
		m_CharacterPositionText->GetTextIndexSize(), m_CharacterPositionText->GetFontTexture());
	m_ShaderManager->Render("TextShader", msg2);

	// ���� ������
	m_CharacterRotationText->Update(deltaTime);

	JMessage msg3 = m_ShaderManager->CreateTextureShaderDesc(
		m_CharacterRotationText->GetMatrix(), m_2DviewMatrix, OrthoMatrix,
		m_CharacterRotationText->GetTextIndexSize(), m_CharacterRotationText->GetFontTexture());
	m_ShaderManager->Render("TextShader", msg3);

	m_DirectX->TurnOffAlphaBlending();
	m_DirectX->TurnZBufferOn();
}
void RenderEngine::DebugLogOnScreen()
{
	m_FpsText->InputText(L"FPS : %d", GameTimer::FPS());
	m_CharacterPositionText->InputText(L"X : %d    Y : %d    Z : %d",
		(int)m_Pawn->GetLocation().x, (int)m_Pawn->GetLocation().y, (int)m_Pawn->GetLocation().z);
	m_CharacterRotationText->InputText(L"RX : %d   RY : %d   RZ : %d",
		(int)m_Pawn->GetRotation().x, (int)m_Pawn->GetRotation().y, (int)m_Pawn->GetRotation().z);
}