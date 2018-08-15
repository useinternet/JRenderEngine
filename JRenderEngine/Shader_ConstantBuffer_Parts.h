#pragma once
#include"JRenderEngine.h"
#include<map>


namespace JRenderEngine
{
	enum ConstantType
	{
		Dynamic, Static
	};

	class Shader_ConstantBuffer_Parts
	{
	private:
		std::map<const std::string, Microsoft::WRL::ComPtr<ID3D11Buffer>> m_mConstantBuffer;
	public:
		Shader_ConstantBuffer_Parts();
		~Shader_ConstantBuffer_Parts();
		bool InsertConstantBuffer(ID3D11Device* pDevice, const std::string& name, const UINT TypeSize, const ConstantType& type = Dynamic);
		bool InsertConstantBuffer(ID3D11Device* pDevice, const std::string& name, const D3D11_BUFFER_DESC& desc);
		ID3D11Buffer* GetConstantBuffer(const std::string& name);

	};

}
