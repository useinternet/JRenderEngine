#include "ShaderManager.h"
#include"DirectX.h"

using namespace std;
using namespace JRenderEngine;

static ShaderManager* gShaderManager = nullptr;

ShaderManager::ShaderManager()
{
	gShaderManager = this;
	DirectX* direct = DirectX::GetInstance();
	if (direct)
	{
		m_pDevice = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}
	m_TextureShaderDesc = make_unique<TextureShaderDesc>();
	m_AmbientLightShaderDesc = make_unique<AmbientLightShaderDesc>();
}
ShaderManager::~ShaderManager()
{
}

bool ShaderManager::ShaderManagerInit()
{
	bool result = true;
	// 텍스쳐 셰이더 
	m_mShaders.insert(pair<const string, unique_ptr<Shader>>("TextureShader", make_unique<TextureShader>()));
	// 텍스트 셰이더
	m_mShaders.insert(pair<const string, unique_ptr<Shader>>("TextShader", make_unique<TextShader>()));
	// 컬러 셰이더
	m_mShaders.insert(pair<const string, unique_ptr<Shader>>("ColorShader", make_unique<ColorShader>()));
	// AmbientLight
	m_mShaders.insert(pair<const string, unique_ptr<Shader>>("AmbientLightShader", make_unique<AmbientLightShader>()));
	for (auto& iter : m_mShaders)
	{
		iter.second->ShaderInit(m_pDevice);
	}

	return true;
}
void ShaderManager::Render(const string& ShaderName, const JMessage& msg)
{
	// 디버그 모드일때만 검사..
#ifdef _DEBUG
	for (auto& iter : m_mShaders)
	{
		if (iter.first == ShaderName)
		{
			iter.second->Render(m_pDeviceContext, msg);
			return;
		}
	}
	DebugLog(L"Error : The name of the shader to render does not exist.!! \n");
	DebugLog(L"Class : ShaderManager \n");
#endif
#ifndef _DEBUG
	m_mShaders[ShaderName]->Render(m_pDeviceContext, msg);
#endif
}
const JMessage ShaderManager::CreateTextureShaderDesc(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix,
	unsigned int IndexSize, ID3D11ShaderResourceView* Texture)
{
	JMessage msg;
	m_TextureShaderDesc->worldMatrix      = worldMatrix;
	m_TextureShaderDesc->viewMatrix       = viewMatrix;
	m_TextureShaderDesc->projectionMatrix = projectionMatrix;
	m_TextureShaderDesc->indexSize        = IndexSize;
	m_TextureShaderDesc->Texture          = Texture;

	msg.Flags = 0x000;
	msg.pData = m_TextureShaderDesc.get();
	msg.Size = sizeof(TextureShaderDesc);

	return msg;
}
const JMessage ShaderManager::CreateAmbientLightShaderDesc(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix,
	unsigned int IndexSize, ID3D11ShaderResourceView* Texture, const D3DXVECTOR3& viewPosition)
{
	JMessage msg;
	m_AmbientLightShaderDesc->worldMatrix = worldMatrix;
	m_AmbientLightShaderDesc->viewMatrix = viewMatrix;
	m_AmbientLightShaderDesc->projectionMatrix = projectionMatrix;
	m_AmbientLightShaderDesc->indexSize = IndexSize;
	m_AmbientLightShaderDesc->Texture = Texture;
	m_AmbientLightShaderDesc->EyePosition = viewPosition;

	msg.Flags = 0x000;
	msg.pData = m_AmbientLightShaderDesc.get();
	msg.Size = sizeof(AmbientLightShaderDesc);

	return msg;
}
ShaderManager* ShaderManager::GetInstance() { return gShaderManager; }
