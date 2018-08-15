#include"RenderTexture.h"

using namespace JRenderEngine;


RenderTexture::RenderTexture()
{

}
RenderTexture::~RenderTexture()
{

}

bool RenderTexture::InitRenderTexture(ID3D11Device* Device, const int width, const int height, const float screenDepth, const float screenNear)
{
	m_TextureWidth = width;
	m_TextureHeight = height;
	CreateRenderTargetView_Texture(Device, width, height);
	CreateDepthStencilBuffer_View(Device, width, height);
	CreateViewPort_Matrix(width, height, screenDepth, screenNear);
	return true;
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* DeviceContext)
{
	DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	DeviceContext->RSSetViewports(1, &m_Viewport);
}
void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* DeviceContext)
{
	float color[4] = { 0.0f,0.0f,0.0f,1.0f };
	DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), color);
	DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* RenderTexture::GetTexture()
{
	return m_ResourceView.Get();
}
const D3DXMATRIX& RenderTexture::GetProjectionMatrix()
{
	return m_projectionMatrix;
}
const D3DXMATRIX& RenderTexture::GetOrthoMatrix()
{
	return m_orthoMatrix;
}
const int RenderTexture::GetWidth()
{
	return m_TextureWidth;
}
const int RenderTexture::GetHeight()
{
	return m_TextureHeight;
}
bool RenderTexture::CreateRenderTargetView_Texture(ID3D11Device* Device, const int width, const int height)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC            TextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC   RenderTargetDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;


	// ·»´õÅ¸±ê¿ë ÅØ½ºÃÄ ¸ñ·Ï ÀÛ¼º
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
	TextureDesc.Width = width;
	TextureDesc.Height = height;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	// ÅØ½ºÃÄ »ý¼º
	result = Device->CreateTexture2D(&TextureDesc, nullptr, m_RenderTexture.GetAddressOf());
	if (FAILED(result)) return false;

	// ·»´õ Å¸°Ù ºä ¸ñ·ÏÀ» ÀÛ¼º
	RenderTargetDesc.Format = TextureDesc.Format;
	RenderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetDesc.Texture2D.MipSlice = 0;

	// ·»´õ Å¸°Ù ºä »ý¼º
	result = Device->CreateRenderTargetView(m_RenderTexture.Get(), &RenderTargetDesc, m_RenderTargetView.GetAddressOf());
	if (FAILED(result)) return false;

	// ¼ÎÀÌ´õ ¸®¼Ò½º ºä ¸ñ·ÏÀ» ÀÛ¼º

	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ¸®¼Ò½º ºä »ý¼º
	result = Device->CreateShaderResourceView(m_RenderTexture.Get(), &ShaderResourceViewDesc, m_ResourceView.GetAddressOf());
	if (FAILED(result)) return false;


	return true;
}
bool RenderTexture::CreateDepthStencilBuffer_View(ID3D11Device* Device, const int width, const int height)
{
	HRESULT                       result = S_OK;
	D3D11_TEXTURE2D_DESC          DepthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;

	ZeroMemory(&DepthBufferDesc, sizeof(DepthBufferDesc));
	// ±íÀÌ ¹öÆÛ ¸ñ·ÏÀ» ÀÛ¼ºÇÑ´Ù.
	DepthBufferDesc.Width = width;
	DepthBufferDesc.Height = height;
	DepthBufferDesc.MipLevels = 1;
	DepthBufferDesc.ArraySize = 1;
	DepthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthBufferDesc.SampleDesc.Count = 1;
	DepthBufferDesc.SampleDesc.Quality = 0;
	DepthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthBufferDesc.CPUAccessFlags = 0;
	DepthBufferDesc.MiscFlags = 0;

	// ±íÀÌ ¹öÆÛ¸¦ »ý¼º
	result = Device->CreateTexture2D(&DepthBufferDesc, nullptr, m_DepthStencilBuffer.GetAddressOf());
	if (FAILED(result)) return false;


	// ±íÀÌ ½ºÅÙ½Ç ºä ¸ñ·ÏÀ» ÀÛ¼º
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	result = Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &DepthStencilViewDesc, m_DepthStencilView.GetAddressOf());
	if (FAILED(result)) return false;



	return true;
}

bool RenderTexture::CreateViewPort_Matrix(const int width, const int height, const float screenDepth, const float screenNear)
{
	m_Viewport.Width = (float)width;
	m_Viewport.Height = (float)height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;

	// Çà·Ä ¸¸µé±â
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)width / (float)height), screenNear, screenDepth);
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)width, (float)height, screenNear, screenDepth);
	return true;
}