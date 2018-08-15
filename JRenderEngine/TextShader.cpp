#include"TextShader.h"
using namespace JRenderEngine;

TextShader::TextShader()
{

}
TextShader::~TextShader()
{

}
bool TextShader::ShaderInit(ID3D11Device* pDevice)
{
	bool result = true;
	// 셰이더 컴파일 
	m_LayOut_Compiler->CreateInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	m_LayOut_Compiler->CreateInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	result = m_LayOut_Compiler->ShaderCompile(pDevice, L"../ShaderData/TextShader_vs.hlsl", L"../ShaderData/TextShader_ps.hlsl");
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
	result = m_ConstantBuffer->InsertConstantBuffer(pDevice, "ColorBuffer", sizeof(ColorBuffer), ConstantType::Dynamic);
	if (!result)
	{
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
bool TextShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	TextureShader::SetShaderParameters(DeviceContext, ShaderMessage);

	
	bool result = true;
	// 임시
	result = Shader::Lock_Setting_UnLock<ColorBuffer>(
		DeviceContext,ShaderType::Pixel,m_ConstantBuffer->GetConstantBuffer("ColorBuffer"),D3D11_MAP_WRITE_DISCARD,
		[](ColorBuffer* dataPtr)
	{
		dataPtr->Color = { 1.0f,1.0f,1.0f,1.0f };
	});
	if (!result)
	{
		//예외처리
		return false;
	}

	return true;
}