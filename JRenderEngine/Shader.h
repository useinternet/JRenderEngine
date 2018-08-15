#pragma once
#include"JRenderEngine.h"
#include"Shader_Layout_Compile_Parts.h"
#include"Shader_ConstantBuffer_Parts.h"
#include"Shader_SampleState_Parts.h"
#include<functional>

// 메세지에담을 변수

namespace JRenderEngine
{
	class JMessage;

	class Shader
	{
	private:
		size_t m_vsBufferNumber = 0;
		size_t m_psBufferNumber = 0;
	protected:
		enum ShaderType
		{
			Vertex, Pixel
		};
	public:
		Shader() {}
		virtual ~Shader() {}
		void Render(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage);
	protected:
		template<class ConstantBufferType>
		bool Lock_Setting_UnLock(ID3D11DeviceContext* DeviceContext, const ShaderType& type, ID3D11Buffer* ConstantBuffer,
			const D3D11_MAP& mapFormat, const std::function<void(ConstantBufferType*)>& func);
		bool InsertTexture(ID3D11DeviceContext* DeviceContext, const size_t slotNumber, ID3D11ShaderResourceView** texture,
			const size_t textureSize, const ShaderType& type = Pixel);
	public:
		virtual bool ShaderInit(ID3D11Device* pDevice) = 0;
	protected:
		virtual bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) = 0;
		virtual bool InputTextureInShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) = 0;
		virtual bool RenderShader(ID3D11DeviceContext* DeviceContext, const JMessage& ShaderMessage) = 0;
	};

	template<class ConstantBufferType>
	bool Shader::Lock_Setting_UnLock(ID3D11DeviceContext* DeviceContext, const ShaderType& type, ID3D11Buffer* ConstantBuffer,
		const D3D11_MAP& mapFormat, const std::function<void(ConstantBufferType*)>& func)
	{
		HRESULT result = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		// Lock
		result = DeviceContext->Map(ConstantBuffer, 0, mapFormat, 0, &mappedResource);
		if (FAILED(result)) return false;

		// 지정 함수 실행
		func((ConstantBufferType*)mappedResource.pData);

		// UnLock
		DeviceContext->Unmap(ConstantBuffer, 0);


		// 타입에따라 셰이더에 버퍼 등록
		switch (type)
		{
		case Vertex:
			DeviceContext->VSSetConstantBuffers(m_vsBufferNumber, 1, &ConstantBuffer);
			m_vsBufferNumber++;
			break;
		case Pixel:
			DeviceContext->PSSetConstantBuffers(m_psBufferNumber, 1, &ConstantBuffer);
			m_psBufferNumber++;
			break;
		default:
			return false;
		}
		return true;
	}
}


