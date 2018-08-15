#include"Shader.h"

using namespace JRenderEngine;

void Shader::Render(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage)
{
	bool result = true;
	// 파라미터 셋팅
	result = SetShaderParameters(DeviceContext, ShaderMessage);
	if (!result)
	{
		DebugLog(L"Error : Failed SetShaderParameters \n");
		DebugLog(L"Class : Shader \n");
		return;
	}
	// 텍스쳐 셰이더에 삽입
	result = InputTextureInShader(DeviceContext, ShaderMessage);
	if (!result)
	{
		DebugLog(L"Error : Failed InputTextureInShader \n");
		DebugLog(L"Class : Shader \n");
		return;
	}
	result = RenderShader(DeviceContext, ShaderMessage);
	if (!result)
	{
		DebugLog(L"Error : Failed RenderShader \n");
		DebugLog(L"Class : Shader \n");
		return;
	}
	// 각 픽셀 넘버수 초기화
	m_vsBufferNumber = 0;
	m_psBufferNumber = 0;
}

bool Shader::InsertTexture(ID3D11DeviceContext* DeviceContext, const size_t slotNumber, ID3D11ShaderResourceView** texture,
	const size_t textureSize, const ShaderType& type)
{
	switch (type)
	{
	case Vertex:
		DeviceContext->VSSetShaderResources(slotNumber, textureSize, texture);
		break;
	case Pixel:
		DeviceContext->PSSetShaderResources(slotNumber, textureSize, texture);
		break;
	default:
		return false;
	}
	return true;

}