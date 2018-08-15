#pragma once
#include"TextureShader.h"
namespace JRenderEngine
{
	typedef TextureShaderDesc ColorShaderDesc;
	class ColorShader : public TextureShader
	{
	public:
		ColorShader();
		virtual ~ColorShader();
	public:
		virtual bool ShaderInit(ID3D11Device* pDevice) override;
	};

}
