#include"AmbientLightShader.h"
using namespace std;
using namespace JRenderEngine;
AmbientLightShader::AmbientLightShader() : Shader()
{
	m_LayOut_Compiler = make_unique<Shader_Layout_Compile_Parts>();
	m_ConstantBuffer = make_unique<Shader_ConstantBuffer_Parts>();
	m_SampleState = make_unique<Shader_SampleState_Parts>();
}
AmbientLightShader::~AmbientLightShader()
{

}


bool AmbientLightShader::ShaderInit(ID3D11Device* pDevice)
{
	bool result = true;
	// 셰이더 컴파일 
	m_LayOut_Compiler->CreateInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	m_LayOut_Compiler->CreateInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	m_LayOut_Compiler->CreateInputDesc("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	result = m_LayOut_Compiler->ShaderCompile(pDevice, L"../ShaderData/Light_vs.hlsl", L"../ShaderData/Light_ps.hlsl");
	if (!result)
	{
		// 예외 처리
		return false;
	}

	// 상수 버퍼 
	result = m_ConstantBuffer->InsertConstantBuffer(pDevice, "MatrixBuffer", sizeof(MatrixBuffer), ConstantType::Dynamic);
	if (!result)
	{
		// 예외 처리
		return false;
	}
	result = m_ConstantBuffer->InsertConstantBuffer(pDevice, "AmbientLightBuffer", sizeof(LightBuffer), ConstantType::Dynamic);
	if (!result)
	{
		// 예외 처리
		return false;
	}
	result = m_ConstantBuffer->InsertConstantBuffer(pDevice, "ObjectBuffer", sizeof(ObjectBuffer), ConstantType::Dynamic);
	if (!result)
	{
		// 예외 처리
		return false;
	}
	// 샘플러
	result = m_SampleState->InsertSampleState(pDevice, "SampleStateWrap", SampleStateType::Wrap);
	if (!result)
	{
		//예외처리
		return false;
	}
	return true;
}
bool AmbientLightShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	bool result = true;

	AmbientLightShaderDesc* desc = (AmbientLightShaderDesc*)(ShaderMessage.pData);


	// 셰이더에 맞게 행렬로 변환시킨다.
	D3DXMatrixTranspose(&desc->worldMatrix, &desc->worldMatrix);
	D3DXMatrixTranspose(&desc->viewMatrix, &desc->viewMatrix);
	D3DXMatrixTranspose(&desc->projectionMatrix, &desc->projectionMatrix);

	result = Shader::Lock_Setting_UnLock<MatrixBuffer>(
		DeviceContext, ShaderType::Vertex, m_ConstantBuffer->GetConstantBuffer("MatrixBuffer"), D3D11_MAP_WRITE_DISCARD,
		[desc](MatrixBuffer* dataPtr)
	{
		dataPtr->worldMatrix = desc->worldMatrix;
		dataPtr->viewMatrix = desc->viewMatrix;
		dataPtr->projectionMatrix = desc->projectionMatrix;
	});
	if (!result)
	{
		//예외처리
		return false;
	}

	result = Shader::Lock_Setting_UnLock<LightBuffer>(
		DeviceContext, ShaderType::Pixel, m_ConstantBuffer->GetConstantBuffer("AmbientLightBuffer"), D3D11_MAP_WRITE_DISCARD,
		[desc](LightBuffer* dataPtr)
	{
		dataPtr->AmbientDown = desc->AmbientDown;
		dataPtr->AmbientUp = desc->AmbientUp;
		dataPtr->DirLightColor = desc->DirLightColor;
		dataPtr->DirToLight = desc->DirToLight;

	});
	if (!result)
	{
		//예외처리
		return false;
	}

	result = Shader::Lock_Setting_UnLock<ObjectBuffer>(
		DeviceContext, ShaderType::Pixel, m_ConstantBuffer->GetConstantBuffer("ObjectBuffer"), D3D11_MAP_WRITE_DISCARD,
		[desc](ObjectBuffer* dataPtr)
	{
		dataPtr->EyePosition = desc->EyePosition;
		dataPtr->specExp = desc->specExp;
	});
	if (!result)
	{
		//예외처리
		return false;
	}
	return true;
}
bool AmbientLightShader::InputTextureInShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	AmbientLightShaderDesc* desc = (AmbientLightShaderDesc*)(ShaderMessage.pData);
	if (desc->Texture)
	{
		Shader::InsertTexture(DeviceContext, 0, &(desc->Texture), 1);
	}
	return true;
}
bool AmbientLightShader::RenderShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{

	AmbientLightShaderDesc* desc = (AmbientLightShaderDesc*)(ShaderMessage.pData);
	m_LayOut_Compiler->Render(DeviceContext);


	DeviceContext->PSSetSamplers(0, 1, m_SampleState->GetSampleStateAddress("SampleStateWrap"));
	DeviceContext->DrawIndexed(desc->indexSize, 0, 0);
	return true;
}