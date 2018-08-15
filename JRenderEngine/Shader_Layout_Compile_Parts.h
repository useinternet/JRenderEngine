#pragma once
#include"JRenderEngine.h"
#include<vector>

namespace JRenderEngine
{

	// 레이아웃
	class Shader_Layout_Compile_Parts
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_layout;

		Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexShaderBuffer;
		Microsoft::WRL::ComPtr<ID3D10Blob> m_pixelShaderBuffer;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputDesc;
		const std::string m_entryPoint;
		const std::string m_Version_vs;
		const std::string m_Version_ps;
	public:
		Shader_Layout_Compile_Parts(const std::string& entryPoint = "main", const std::string& Version = "5_0");
		~Shader_Layout_Compile_Parts();
		bool ShaderCompile(ID3D11Device* Device, const std::wstring& vsFileName, const std::wstring& psFileName);
		void CreateInputDesc(LPCSTR name, const int index, const DXGI_FORMAT format, const int inputslot,
			const UINT AlignedByteOffset, const D3D11_INPUT_CLASSIFICATION inputsoltclass, const UINT datasteprate);
		void Render(ID3D11DeviceContext* DeviceContext);

		// 반환
		ID3D11VertexShader* GetVertexShader();
		ID3D11PixelShader*  GetPixelShader();
		ID3D11InputLayout*  GetLayOut();
	private:
		bool VertexShaderCompile(ID3D11Device* Device, const std::wstring& vsFileName);
		bool PixelShaderCompile(ID3D11Device* Device, const std::wstring& psFileName);
		bool CreateLayOutBuffer(ID3D11Device* Device);

	};
}



