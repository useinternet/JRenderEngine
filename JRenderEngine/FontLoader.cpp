#include"FontLoader.h"
#include"DirectX.h"
#include<vector>
using namespace JRenderEngine;
using namespace std;

ID3D11Device * FontLoader::m_pDevice;
ID3D11DeviceContext* FontLoader::m_pDeviceContext;
std::map<const std::string, std::unique_ptr<Font>> FontLoader::m_mFonts;

FontLoader::FontLoader()
{
	DirectX* direct = DirectX::GetInstance();
	if (direct)
	{
		m_pDevice = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}
}

FontLoader::~FontLoader()
{

}

bool FontLoader::FontInit()
{
	LoadFonts("Godic", "../Font/Godic.fnt", L"../Font/Godic_0.png");

	return true;
}
Font* FontLoader::LoadFont(const std::string& name)
{
#ifdef _DEBUG
	bool result = false;
	for (auto& iter : m_mFonts)
	{
		if (iter.first == name)
		{
			result = true;
		}
	}
	if (!result)
	{
		DebugLog(L"Error : Failed LoadFont -> %s  not exist in FontLoader \n", name.c_str());
	}
#endif
	return m_mFonts[name].get();
}
void FontLoader::LoadFonts(const std::string& name, const std::string& FontName, const std::wstring& TextureName)
{
	unique_ptr<Font> font = make_unique<Font>();
	font->LoadFont(m_pDevice, FontName, TextureName);
	m_mFonts.insert(pair<const string, unique_ptr<Font>>(name, move(font)));
}