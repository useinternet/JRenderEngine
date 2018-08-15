#include"Shader_SampleState_Parts.h"
using namespace JRenderEngine;
using namespace Microsoft::WRL;
using namespace std;

Shader_SampleState_Parts::Shader_SampleState_Parts()
{

}
Shader_SampleState_Parts::~Shader_SampleState_Parts()
{

}

bool Shader_SampleState_Parts::InsertSampleState(ID3D11Device* pDevice, const string& name, const SampleStateType& type)
{
	HRESULT result = S_OK;
	ComPtr<ID3D11SamplerState> sampleState;
	D3D11_SAMPLER_DESC desc;

	switch (type)
	{
	case Wrap:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case Clamp:
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	default:
		DebugLog(L"Error : Failed InsertSampleState!! \n");
		DebugLog(L"Class : Shader_SampleState_Parts \n");
		return false;
	}
	desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MipLODBias     = 0.0f;
	desc.MaxAnisotropy  = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.BorderColor[0] = 0.0f;
	desc.BorderColor[1] = 0.0f;
	desc.BorderColor[2] = 0.0f;
	desc.BorderColor[3] = 0.0f;
	desc.MinLOD         = 0;
	desc.MaxLOD         = D3D11_FLOAT32_MAX;

	// 基敲矾 积己
	result = pDevice->CreateSamplerState(&desc, sampleState.GetAddressOf());
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed InsertSampleState!! \n");
		DebugLog(L"Class : Shader_SampleState_Parts \n");
		return false;
	}

	m_mSampleSate.insert(pair<const string, ComPtr<ID3D11SamplerState>>(name, move(sampleState)));

	return true;
	
}
bool Shader_SampleState_Parts::InsertSampleState(ID3D11Device* pDevice, const std::string& name, D3D11_SAMPLER_DESC& desc)
{
	HRESULT result = S_OK;
	ComPtr<ID3D11SamplerState> sampleState;

	// 基敲矾 积己
	result = pDevice->CreateSamplerState(&desc, sampleState.GetAddressOf());
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed InsertSampleState!! \n");
		DebugLog(L"Class : Shader_SampleState_Parts \n");
		return false;
	}
	m_mSampleSate.insert(pair<const string, ComPtr<ID3D11SamplerState>>(name, move(sampleState)));

	return true;
}
ID3D11SamplerState* Shader_SampleState_Parts::GetSampleState(const std::string& name)
{
#ifdef _DEBUG
	for (auto& iter : m_mSampleSate)
	{
		if (iter.first == name)
		{
			return iter.second.Get();
		}
	}
	DebugLog(L"Error : Failed GetSampleState!! \n");
	DebugLog(L"Class : Shader_SampleState_Parts \n");
	return nullptr;
#endif

#ifndef _DEBUG
	return 	m_mSampleSate[name].Get();
#endif
}
ID3D11SamplerState** Shader_SampleState_Parts::GetSampleStateAddress(const std::string& name)
{
#ifdef _DEBUG
	for (auto& iter : m_mSampleSate)
	{
		if (iter.first == name)
		{
			return iter.second.GetAddressOf();
		}
	}
	DebugLog(L"Error : Failed GetSampleStateAddress!! \n");
	DebugLog(L"Class : Shader_SampleState_Parts \n");
	return nullptr;
#endif
#ifndef _DEBUG
	return 	m_mSampleSate[name].GetAddressOf();
#endif
}