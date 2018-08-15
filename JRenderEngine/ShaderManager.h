#pragma once
#include"JRenderEngine.h"
#include"Shader.h"
#include"TextureShader.h"
#include"TextShader.h"
#include"ColorShader.h"
#include"AmbientLightShader.h"
#include<map>

namespace JRenderEngine
{
	class ShaderManager
	{
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		std::map<const std::string, std::unique_ptr<Shader>> m_mShaders;

		// ºŒ¿Ã¥ı º≥¡§ ∫ØºˆµÈ
		std::unique_ptr<TextureShaderDesc> m_TextureShaderDesc;
		std::unique_ptr<AmbientLightShaderDesc> m_AmbientLightShaderDesc;
	public:
		ShaderManager();
		~ShaderManager();
		// ºŒ¿Ã¥ı √ ±‚»≠
		bool ShaderManagerInit();

		// ºŒ¿Ã¥ı ∑ª¥ı∏µ
		void Render(const std::string& ShaderName, const JMessage& msg);

		const JMessage CreateTextureShaderDesc(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix,
			unsigned int IndexSize, ID3D11ShaderResourceView* Texture);
		const JMessage CreateAmbientLightShaderDesc(const D3DXMATRIX& worldMatrix, const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projectionMatrix,
			unsigned int IndexSize, ID3D11ShaderResourceView* Texture,const D3DXVECTOR3& viewPosition);
		static ShaderManager* GetInstance();
	};

}


