#include"Shader_Layout_Compile_Parts.h"
using namespace std;
using namespace Microsoft::WRL;
using namespace JRenderEngine;


Shader_Layout_Compile_Parts::Shader_Layout_Compile_Parts(const string& entryPoint, const string& Version)
	: m_entryPoint(entryPoint), m_Version_vs("vs_"+Version), m_Version_ps("ps_" + Version)
{
}
Shader_Layout_Compile_Parts::~Shader_Layout_Compile_Parts()
{

}
bool Shader_Layout_Compile_Parts::ShaderCompile(ID3D11Device* Device,const wstring& vsFileName, const wstring& psFileName)
{
	bool result = true;
	// 레이아웃 목록이 존재 하지 않는다면..
	if (m_InputDesc.size() == 0)
	{
		DebugLog(L"Error : InputLayoutDesc's size = 0 !! \n");
		DebugLog(L"Class : Shader_Layout_Compile_Parts \n");
		return false;
	}
	// 각 셰이더 컴파일
	result = VertexShaderCompile(Device,vsFileName);
	if (!result)
	{
		DebugLog(L"Error : Failed VertexShaderCompil!! \n");
		DebugLog(L"Class : Shader_Layout_Compile_Parts \n");
		return false;
	}
	result = PixelShaderCompile (Device,psFileName);
	if (!result)
	{
		DebugLog(L"Error : Failed PixelShaderCompile!! \n");
		DebugLog(L"Class : Shader_Layout_Compile_Parts \n");
		return false;
	}
	result = CreateLayOutBuffer(Device);
	if (!result)
	{
		DebugLog(L"Error : Failed CreateLayOutBuffer!! \n");
		DebugLog(L"Class : Shader_Layout_Compile_Parts \n");
		return false;
	}
	return true;
}
void Shader_Layout_Compile_Parts::CreateInputDesc(LPCSTR name, const int index, const DXGI_FORMAT format, const int inputslot,
	const UINT AlignedByteOffset, const D3D11_INPUT_CLASSIFICATION inputsoltclass, const UINT datasteprate)
{
	D3D11_INPUT_ELEMENT_DESC desc;

	desc.SemanticName         = name;
	desc.SemanticIndex        = index;
	desc.Format               = format;
	desc.InputSlot            = inputslot;
	desc.AlignedByteOffset    = AlignedByteOffset;
	desc.InputSlotClass       = inputsoltclass;
	desc.InstanceDataStepRate = datasteprate;
	
	m_InputDesc.push_back(desc);
}
void Shader_Layout_Compile_Parts::Render(ID3D11DeviceContext* DeviceContext)
{
	// 정점 입력 레이아웃 입력
	DeviceContext->IASetInputLayout(m_layout.Get());

	// 정점 셰이더 및 픽셀 셰이더 입력
	DeviceContext->VSSetShader(m_vertexShader.Get(), NULL, 0);
	DeviceContext->PSSetShader(m_pixelShader.Get(), NULL, 0);
}
ID3D11VertexShader* Shader_Layout_Compile_Parts::GetVertexShader() { return m_vertexShader.Get(); }
ID3D11PixelShader*  Shader_Layout_Compile_Parts::GetPixelShader()  { return m_pixelShader.Get(); }
ID3D11InputLayout*  Shader_Layout_Compile_Parts::GetLayOut()       { return m_layout.Get(); }
bool Shader_Layout_Compile_Parts::VertexShaderCompile(ID3D11Device* Device,const std::wstring& vsFileName)
{
	HRESULT result;

	// 정점 셰이더 컴파일 및 생성
	result = D3DX11CompileFromFile(vsFileName.c_str(), nullptr, nullptr, m_entryPoint.c_str(), m_Version_vs.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, m_vertexShaderBuffer.GetAddressOf(), nullptr, nullptr);
	if (FAILED(result))
	{
		return false;
	}
	result = Device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(),
		nullptr, m_vertexShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
bool Shader_Layout_Compile_Parts::PixelShaderCompile(ID3D11Device* Device, const std::wstring& psFileName)
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D10Blob> ErrorMessage;
	// 픽셀 셰이더 컴파일 및 생성
	result = D3DX11CompileFromFile(psFileName.c_str(), nullptr,nullptr, m_entryPoint.c_str(), m_Version_ps.c_str(),
		D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr, m_pixelShaderBuffer.GetAddressOf(), ErrorMessage.GetAddressOf(), nullptr);
	if (FAILED(result))
	{
		char* str = (char*)ErrorMessage->GetBufferPointer();
		return false;
	}
	result = Device->CreatePixelShader(m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(),
		nullptr, m_pixelShader.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
bool Shader_Layout_Compile_Parts::CreateLayOutBuffer(ID3D11Device* Device)
{
	HRESULT result = S_OK;

	result = Device->CreateInputLayout(&m_InputDesc[0], m_InputDesc.size(), m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(), m_layout.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	m_vertexShaderBuffer.Reset();
	m_pixelShaderBuffer.Reset();

	return true;
}