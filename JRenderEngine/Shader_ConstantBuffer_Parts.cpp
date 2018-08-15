#include"Shader_ConstantBuffer_Parts.h"
using namespace JRenderEngine;
using namespace Microsoft::WRL;
using namespace std;

Shader_ConstantBuffer_Parts::Shader_ConstantBuffer_Parts()
{

}
Shader_ConstantBuffer_Parts::~Shader_ConstantBuffer_Parts()
{

}
bool Shader_ConstantBuffer_Parts::InsertConstantBuffer(ID3D11Device* pDevice, const std::string& name, const UINT TypeSize, const ConstantType& type)
{
	HRESULT result = S_OK;
	D3D11_BUFFER_DESC desc;
	ComPtr<ID3D11Buffer> ConstantBuffer;
	ID3D11Buffer* buffer = nullptr;
	// 상수버퍼 목록작성
	if (type == Dynamic)
	{
		desc.Usage          = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	if (type == Static)
	{
		desc.Usage          = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	desc.ByteWidth           = TypeSize;
	desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	desc.MiscFlags           = 0;
	desc.StructureByteStride = 0;



	// 상수버퍼 생성 및 파츠에 삽입
	result = pDevice->CreateBuffer(&desc, nullptr, ConstantBuffer.GetAddressOf());
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed InsertConstantBuffer!! \n");
		DebugLog(L"Class : Shader_ConstantBuffer_Parts \n");
		return false;
	}

	m_mConstantBuffer.insert(pair<const string, ComPtr<ID3D11Buffer>>(name, move(ConstantBuffer)));

	return true;
}
bool Shader_ConstantBuffer_Parts::InsertConstantBuffer(ID3D11Device* pDevice, const std::string& name, const D3D11_BUFFER_DESC& desc)
{
	HRESULT result = S_OK;
	ComPtr<ID3D11Buffer> ConstantBuffer;
	// 상수버퍼 생성 및 파츠에 삽입
	result = pDevice->CreateBuffer(&desc, nullptr, ConstantBuffer.GetAddressOf());
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed InsertConstantBuffer!! \n");
		DebugLog(L"Class : Shader_ConstantBuffer_Parts \n");
		return false;
	}
	m_mConstantBuffer.insert(pair<const string, ComPtr<ID3D11Buffer>>(name, move(ConstantBuffer)));

	return true;
}
ID3D11Buffer* Shader_ConstantBuffer_Parts::GetConstantBuffer(const std::string& name)
{
#ifdef _DEBUG
	// 혹시모르니 해당 상수버퍼가 존재하는지 확인후 있으면 리턴 없으면 nullptr을 리턴한다.
	for (auto& iter : m_mConstantBuffer)
	{
		if (iter.first == name)
		{
			return iter.second.Get();
		}
	}

	DebugLog(L"Error : ConstantBuffer is nullptr \n");
	return nullptr;
#endif

#ifndef _DEBUG
	return m_mConstantBuffer[name].Get();
#endif
}