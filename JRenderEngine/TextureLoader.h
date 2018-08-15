#pragma once
#include"JRenderEngine.h"
#include<map>
#include<string>

namespace JRenderEngine
{
	class TextureLoader
	{
		static std::map<const std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_mTexture;
		static std::map<const std::string, D3DX11_IMAGE_INFO> m_mTextureInfo;
		static ID3D11Device* m_pDevice;
		static ID3D11DeviceContext* m_pDeviceContext;
	public:
		TextureLoader();
		~TextureLoader();

		bool TextureInit();

		static ID3D11ShaderResourceView* Load(const std::string name);
		static D3DX11_IMAGE_INFO& LoadInfomation(const std::string name);
	private:
		bool TextureLoad(const std::string name, const std::wstring TextureDirectoty);
	};

}


