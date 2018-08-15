#pragma once
#include"Shader.h"
namespace JRenderEngine
{
	typedef struct AmbientLightShaderDesc
	{
		D3DXMATRIX worldMatrix;
		D3DXMATRIX viewMatrix;
		D3DXMATRIX projectionMatrix;
		unsigned int indexSize;
		ID3D11ShaderResourceView* Texture = nullptr;
		D3DXVECTOR3 AmbientDown = { 0.1f,0.1f,0.1f };
		D3DXVECTOR3 AmbientUp = { 0.3f,0.3f,0.3f };
		D3DXVECTOR3 DirToLight = { 0.0f,0.5f,-1.0f };
		D3DXVECTOR3 DirLightColor = { 1.0f,1.0f,1.0f };
		D3DXVECTOR3 EyePosition;
		float specExp = 18.0f;
	}AmbientLightShaderDesc;


	class AmbientLightShader : public Shader
	{
	protected:
		struct MatrixBuffer
		{
			D3DXMATRIX worldMatrix;
			D3DXMATRIX viewMatrix;
			D3DXMATRIX projectionMatrix;
		};
		struct LightBuffer
		{
			D3DXVECTOR3 AmbientDown;
			float padding;
			D3DXVECTOR3 AmbientUp;
			float padding2;
			D3DXVECTOR3 DirToLight;
			float padding3;
			D3DXVECTOR3 DirLightColor;
			float padding4;
		};
		struct ObjectBuffer
		{
			D3DXVECTOR3 EyePosition;
			float specExp;
		};
	protected:
		std::unique_ptr<Shader_Layout_Compile_Parts> m_LayOut_Compiler;
		std::unique_ptr<Shader_ConstantBuffer_Parts> m_ConstantBuffer;
		std::unique_ptr<Shader_SampleState_Parts>    m_SampleState;
	public:
		AmbientLightShader();
		virtual ~AmbientLightShader();
	public:
		virtual bool ShaderInit(ID3D11Device* pDevice) override;
	protected:
		virtual bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
		virtual bool InputTextureInShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
		virtual bool RenderShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
	};

}

