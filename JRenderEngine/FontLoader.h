#pragma once
#include"JRenderEngine.h"
#include"Font.h"
#include<map>


namespace JRenderEngine
{
	class FontLoader
	{
		static ID3D11Device * m_pDevice;
		static ID3D11DeviceContext* m_pDeviceContext;
		static std::map<const std::string, std::unique_ptr<Font>> m_mFonts;
	public:
		FontLoader();
		~FontLoader();


		bool FontInit();
		static Font* LoadFont(const std::string& name);
	private:
		void LoadFonts(const std::string& name, const std::string& FontName, const std::wstring& TextureName);
	};
}