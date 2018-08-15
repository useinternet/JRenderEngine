#include"TextureShader.h"
using namespace std;
using namespace JRenderEngine;
TextureShader::TextureShader() : Shader()
{
	m_LayOut_Compiler = make_unique<Shader_Layout_Compile_Parts>();
	m_ConstantBuffer = make_unique<Shader_ConstantBuffer_Parts>();
	m_SampleState = make_unique<Shader_SampleState_Parts>();
}
TextureShader::~TextureShader()
{

}


bool TextureShader::ShaderInit(ID3D11Device* pDevice)
{
	bool result = true;
	// ���̴� ������ 
	m_LayOut_Compiler->CreateInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	m_LayOut_Compiler->CreateInputDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0);
	result = m_LayOut_Compiler->ShaderCompile(pDevice, L"../ShaderData/TextureShader_vs.hlsl", L"../ShaderData/TextureShader_ps.hlsl");
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
bool TextureShader::SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	bool result = true;

	TextureShaderDesc* desc = (TextureShaderDesc*)(ShaderMessage.pData);


	// ���̴��� �°� ��ķ� ��ȯ��Ų��.
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
		//����ó��
		return false;
	}
	return true;
}
bool TextureShader::InputTextureInShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	TextureShaderDesc* desc = (TextureShaderDesc*)(ShaderMessage.pData);
	if (desc->Texture)
	{
		Shader::InsertTexture(DeviceContext, 0, &(desc->Texture), 1);
	}
	return true;
}
bool TextureShader::RenderShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{

	TextureShaderDesc* desc = (TextureShaderDesc*)(ShaderMessage.pData);
	m_LayOut_Compiler->Render(DeviceContext);


	DeviceContext->PSSetSamplers(0, 1, m_SampleState->GetSampleStateAddress("SampleStateWrap"));
	DeviceContext->DrawIndexed(desc->indexSize, 0, 0);
	return true;
}