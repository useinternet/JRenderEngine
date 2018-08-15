#pragma once
#include"JRenderEngine.h"
#include<map>

namespace JRenderEngine
{
	enum SampleStateType
	{
		Wrap, Clamp
	};

	class Shader_SampleState_Parts
	{
	private:
		std::map<const std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_mSampleSate;
	public:
		Shader_SampleState_Parts();
		~Shader_SampleState_Parts();

		bool InsertSampleState(ID3D11Device* pDevice, const std::string& name, const SampleStateType& type);
		bool InsertSampleState(ID3D11Device* pDevice, const std::string& name, D3D11_SAMPLER_DESC& desc);
		ID3D11SamplerState* GetSampleState(const std::string& name);
		ID3D11SamplerState** GetSampleStateAddress(const std::string& name);
	};
}
