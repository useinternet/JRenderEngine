#include "TextureLoader.h"
#include "DirectX.h"
using namespace JRenderEngine;
using namespace std;
using namespace Microsoft::WRL;

std::map<const std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> TextureLoader::m_mTexture;
std::map<const std::string, D3DX11_IMAGE_INFO> TextureLoader::m_mTextureInfo;
ID3D11Device* TextureLoader::m_pDevice;
ID3D11DeviceContext* TextureLoader::m_pDeviceContext;

TextureLoader::TextureLoader()
{
	DirectX* direct = DirectX::GetInstance();
	if (direct != nullptr)
	{
		m_pDevice = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}
}
TextureLoader::~TextureLoader()
{


}
bool TextureLoader::TextureInit()
{
	TextureLoad("Dirt_DM", L"../Merterial/Dirt_DM.dds");
	TextureLoad("Fire_AM", L"../Merterial/Fire_AM.dds");
	TextureLoad("Fire_DM", L"../Merterial/Fire_DM.dds");
	TextureLoad("Fire_NS", L"../Merterial/Fire_NS.dds");
	TextureLoad("Ground_DM", L"../Merterial/Ground_DM.dds");
	TextureLoad("Marble_DM", L"../Merterial/Marble_DM.dds");
	TextureLoad("Stone01_DM", L"../Merterial/Stone01_DM.dds");
	TextureLoad("Stone02_DM", L"../Merterial/Stone02_DM.dds");
	TextureLoad("Stone02_NM", L"../Merterial/Stone02_NM.dds");
	TextureLoad("Stone02_SM", L"../Merterial/Stone02_SM.dds");
	TextureLoad("Wall_DM", L"../Merterial/Wall_DM.dds");
	TextureLoad("Water_NM", L"../Merterial/Water_NM.dds");
	return true;
}

ID3D11ShaderResourceView* TextureLoader::Load(const string name)
{
#ifdef _DEBUG
	for (auto& iter : m_mTexture)
	{
		if (iter.first == name)
		{
			return iter.second.Get();
		}
	}
	DebugLog(L"Error : The name of the texture to load does not exist.!! \n");
	DebugLog(L"Class : TextureLoader \n");
	return nullptr;
#endif

#ifndef _DEBUG
	return m_mTexture[name].Get();
#endif
}
D3DX11_IMAGE_INFO& TextureLoader::LoadInfomation(const string name)
{
	return m_mTextureInfo[name];
}
bool TextureLoader::TextureLoad(const string name,const wstring TextureDirectoty)
{
	HRESULT result;
	D3DX11_IMAGE_INFO TextureInfo;
	ComPtr<ID3D11ShaderResourceView> texture;

	// 이미지를 로드한다.
	result = D3DX11CreateShaderResourceViewFromFile(m_pDevice, TextureDirectoty.c_str(), nullptr, nullptr,
		texture.GetAddressOf(), nullptr);
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed TextureLoad!! \n");
		return false;
	}
	m_mTexture.insert(pair < const string, ComPtr<ID3D11ShaderResourceView>>(name, move(texture)));
	// 이미지의 정보를 불러온다.
	result = D3DX11GetImageInfoFromFile(TextureDirectoty.c_str(), nullptr, &TextureInfo, nullptr);
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed TextureInfomationLoad!! \n");
		return false;
	}
	m_mTextureInfo.insert(pair<const string, D3DX11_IMAGE_INFO>(name, move(TextureInfo)));

	return true;
}
