#pragma once
#include"Shader.h"
namespace JRenderEngine
{
	typedef struct TextureShaderDesc
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
		unsigned int indexSize;
		ID3D11ShaderResourceView* Texture = nullptr;
	}TextureShaderDesc;


	class TextureShader : public Shader
	{
	protected:
		struct MatrixBuffer
		{
			D3DXMATRIX worldMatrix;
			D3DXMATRIX viewMatrix;
			D3DXMATRIX projectionMatrix;
		};
	protected:
		std::unique_ptr<Shader_Layout_Compile_Parts> m_LayOut_Compiler;
		std::unique_ptr<Shader_ConstantBuffer_Parts> m_ConstantBuffer;
		std::unique_ptr<Shader_SampleState_Parts>    m_SampleState;
	public:
		TextureShader();
		virtual ~TextureShader();
	public:
		virtual bool ShaderInit(ID3D11Device* pDevice) override;
	protected:
		virtual bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
		virtual bool InputTextureInShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
		virtual bool RenderShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
	};

}

