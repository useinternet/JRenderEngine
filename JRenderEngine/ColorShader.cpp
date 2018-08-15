#include"ColorShader.h"
using namespace std;
using namespace JRenderEngine;


ColorShader::ColorShader() : TextureShader()
{

}
ColorShader::~ColorShader()
{

}


bool ColorShader::ShaderInit(ID3D11Device* pDevice)
{
	bool result = true;
	// ���̴� ������ 
	m_LayOut_Compiler->CreateInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	m_LayOut_Compiler->CreateInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	result = m_LayOut_Compiler->ShaderCompile(pDevice, L"../ShaderData/ColorShader_vs.hlsl", L"../ShaderData/ColorShader_ps.hlsl");
	if (!result)
	{
		// ���� ó��
		return false;
	}

	// ��� ���� 
	result = m_ConstantBuffer->InsertConstantBuffer(pDevice, "MatrixBuffer", sizeof(MatrixBuffer), ConstantType::Dynamic);
	if (!result)
	{
		// ���� ó��
		return false;
	}

	// ���÷�
	result = m_SampleState->InsertSampleState(pDevice, "SampleStateWrap", SampleStateType::Wrap);
	if (!result)
	{
		//����ó��
		return false;
	}
	return true;
}
