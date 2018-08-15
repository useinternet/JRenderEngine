#pragma once
#include "TextureShader.h"

namespace JRenderEngine
{
	struct TextShaderDesc
	{
		D3DXVECTOR4 Color;
	};


	class TextShader : public TextureShader
	{
		struct ColorBuffer
		{
			D3DXVECTOR4 Color;
		};
	public:
		TextShader();
		~TextShader();
	protected:
		virtual bool ShaderInit(ID3D11Device* pDevice) override;
		virtual bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) override;
	};


}