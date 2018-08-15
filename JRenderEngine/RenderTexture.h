#pragma once
#include"JRenderEngine.h"


namespace JRenderEngine
{
	class RenderTexture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D>            m_RenderTexture;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>     m_RenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_ResourceView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>            m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>     m_DepthStencilView;
		D3D11_VIEWPORT                                     m_Viewport;

		D3DXMATRIX                                         m_projectionMatrix;
		D3DXMATRIX                                         m_orthoMatrix;


		int m_TextureWidth;
		int m_TextureHeight;

	public:
		RenderTexture();
		~RenderTexture();

		bool InitRenderTexture(ID3D11Device* Device, const int width, const int height, const float screenDepth = 1000.0f, const float screenNear = 0.1f);

		void SetRenderTarget(ID3D11DeviceContext* DeviceContext);
		void ClearRenderTarget(ID3D11DeviceContext* DeviceContext);


		ID3D11ShaderResourceView* GetTexture();
		const D3DXMATRIX& GetProjectionMatrix();
		const D3DXMATRIX& GetOrthoMatrix();
		const int GetWidth();
		const int GetHeight();
	private:
		bool CreateRenderTargetView_Texture(ID3D11Device* Device, const int width, const int height);
		bool CreateDepthStencilBuffer_View(ID3D11Device* Device, const int width, const int height);
		bool CreateViewPort_Matrix(const int width, const int height, const float screenDepth, const float screenNear);
	};


}