
#include "DirectX.h"
using namespace JRenderEngine;
using namespace std;
using namespace Microsoft::WRL;

static DirectX * g_D3D = nullptr;

DirectX::DirectX()
{
	g_D3D = this;

	int m_ScreenWidth  = 0;
	int m_ScreenHeight = 0;

	m_fFov   = (float)D3DX_PI / 4.0f;
	m_fFarZ  = 0.0f;
	m_fNearZ = 0.0f;

}
DirectX::~DirectX()
{
	
}

/** �⺻ Ŭ���� �޼ҵ� ( ��Ʈ ���� )
�ʱ�ȭ , ������Ʈ, ������, �˴ٿ� �Լ� */
bool DirectX::Initialization(const int ScreenWidth,const int ScreenHeight, const HWND hWnd, const bool FullScreen, const float FarZ, const float NearZ)
{
	bool result;
	m_ScreenWidth  = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_fFarZ = FarZ;
	m_fNearZ = NearZ;

	// ����ü�� �� ����̽�, ����̽� ���ؽ�Ʈ �ʱ�ȭ
	result = CreateDirect3D(hWnd, FullScreen);
	if (!result)
	{
		DebugLog(L"Error : Failed CreateDirect3D!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	//  ��������� ���� Ÿ�ٺ� �ʱ�ȭ �� ���� ���ٽǺ並 ���� Ÿ�ٿ� ���ε�
	result = CreateRenderTargetView();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateRenderTargetView!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// ���ٽ� ���¸� ����
	result = CreateStencilState();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateStencilState!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// ���� ��� ���¸� �����մϴ�.
	result = CreateBlendingState();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateBlendingState!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// ������ȭ�� ���
	result = CreateRaster();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateRaster!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// ����Ʈ �� �� ����,����,���翵 ��� ����
	result = CreateViewPortAndMatrix();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateViewPortAndMatrix!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	result = CreateDirectText();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateDirectText!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	result = CreateTextTargetBitmap();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateTextTargetBitmap!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	return true;
}
void DirectX::BenginDraw(D3DXCOLOR color)
{
	float Color[4] = { color.r, color.g, color.b, color.a };

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), Color);

	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);


}
void DirectX::EndDraw()
{
	m_SwapChain->Present(0, 0);
}
// z���� Ű�� && ����
void DirectX::TurnZBufferOn()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
	return;
}
void DirectX::TurnZBufferOff()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState.Get(), 1);

}
// ���� Ű�� && ����
void DirectX::TurnOnAlphaBlending()
{
	float BlendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	m_DeviceContext->OMSetBlendState(m_AlphaEnableBlendingState.Get(), BlendFactor, 0xffffffff);
}
void DirectX::TurnOffAlphaBlending()
{
	float BlendFactor[4] = { 0.0f,0.0f,0.0f,0.0f };

	m_DeviceContext->OMSetBlendState(m_AlphaDisableBlendingState.Get(), BlendFactor, 0xffffffff);
}
// ������ �Լ�
ID3D11Device* DirectX::GetDevice()
{
	return m_Device.Get();
}
ID3D11DeviceContext* DirectX::GetDeviceContext()
{
	return m_DeviceContext.Get();
}
IDWriteFactory* DirectX::GetWriteFactory()
{
	return m_DWriteFactory.Get();
}
ID2D1Device*  DirectX::Get2DDevice()
{
	return m_D2DDevice.Get();
}
ID2D1DeviceContext* DirectX::Get2DDeviceContext()
{
	return m_D2DContext.Get();
}
ID3D11DepthStencilView* DirectX::GetDepthStencilView()
{
	return m_DepthStencilView.Get();
}
const D3DXMATRIX& DirectX::GetWorldMatrix()
{
	return m_worldMatrix;
}
const D3DXMATRIX& DirectX::GetProjectionMatrix()
{
	return m_projectionMatrix;
}
const D3DXMATRIX& DirectX::GetOrthoMatrix()
{
	return m_orthoMatrix;
}

void DirectX::SetRenderTarget()
{
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
}
void DirectX::SetViewPort()
{
	D3D11_VIEWPORT ViewPort;
	ViewPort.Width = (float)m_ScreenWidth;
	ViewPort.Height = (float)m_ScreenHeight;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &ViewPort);
}
void DirectX::Shutdown()
{

}
/** ��ġ ������ �ҷ��´� */
bool DirectX::CreateDirect3D(const HWND hWnd, const bool FullScreen)
{
	HRESULT result = S_OK;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel[] = 
	{ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
	  D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
	};
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	// ���� ü�� ����
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 2;

	SwapChainDesc.BufferDesc.Width = m_ScreenWidth;
	SwapChainDesc.BufferDesc.Height = m_ScreenHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = hWnd;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	if (FullScreen)
	{
		SwapChainDesc.Windowed = false;
	}
	else
	{
		SwapChainDesc.Windowed = true;
	}
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = 0;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, FeatureLevel, ARRAYSIZE(FeatureLevel),
		D3D11_SDK_VERSION, &SwapChainDesc, m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	return true;
}
bool DirectX::CreateRenderTargetView()
{
	HRESULT result = S_OK;
	ComPtr<ID3D11Texture2D> BackBufferPtr;
	D3D11_TEXTURE2D_DESC BufferDesc;


	/** ���� Ÿ�� �� ����  */
	// ������� �����͸� �����ɴϴ�.
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBufferPtr.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// ������� �����ͷ� ����Ÿ�� �並 �����մϴ�.
	result = m_Device->CreateRenderTargetView(BackBufferPtr.Get(), NULL, m_RenderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	/** ���� ���� ���� */
	// ���� ������ ������ �ۼ�
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.Width = m_ScreenWidth;
	BufferDesc.Height = m_ScreenHeight;
	BufferDesc.MipLevels = 1;
	BufferDesc.ArraySize = 1;
	BufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	BufferDesc.SampleDesc.Count = 1;
	BufferDesc.SampleDesc.Quality = 0;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	// ���� ���� ����
	result = m_Device->CreateTexture2D(&BufferDesc, NULL, m_DepthStencilBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool DirectX::CreateStencilState()
{
	HRESULT result = S_OK; 
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;            // z���� ���ٽ� ����
	D3D11_DEPTH_STENCIL_DESC DepthDisalbedStencilDesc;    // z����(x) ���ٽ� ����(2d������ ��)
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;

	// ���ٽ� ������ ������ �ۼ�
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// ���ٽ� ���� ����
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, m_DepthStencilState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// ����-���ٽ� ���¸� ����
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);



	// ����-���ٽ� ���� ������ �ۼ�
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// ����-���ٽ� �並 ����. 
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &DepthStencilViewDesc, m_DepthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// ����Ÿ�� ��� ����-���ٽ� ���۸� ���� ��� ���������ο� ���ε��մϴ�.
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	
	// 2D �������� ���� ����- ���ٽ� ���¸� �����. (zbuffer �����)
	ZeroMemory(&DepthDisalbedStencilDesc, sizeof(DepthDisalbedStencilDesc));
	DepthDisalbedStencilDesc.DepthEnable = false;
	DepthDisalbedStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthDisalbedStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DepthDisalbedStencilDesc.StencilEnable = true;
	DepthDisalbedStencilDesc.StencilReadMask = 0xFF;
	DepthDisalbedStencilDesc.StencilWriteMask = 0xFF;
	DepthDisalbedStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthDisalbedStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthDisalbedStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthDisalbedStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	DepthDisalbedStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthDisalbedStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthDisalbedStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthDisalbedStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// 2D �������� ���� - ���ٽ� ���¸� �����.
	result = m_Device->CreateDepthStencilState(&DepthDisalbedStencilDesc, m_DepthDisabledStencilState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool DirectX::CreateBlendingState()
{
	HRESULT result = S_OK;
	D3D11_BLEND_DESC BlendStateDesc;

	ZeroMemory(&BlendStateDesc, sizeof(D3D11_BLEND_DESC));

	// ���� On ��� ������ �ۼ� 
	/** ���� ��� ���� 
	SrcBlend : �ȼ� ���̴��� ����ϴ� RGB ���� ���� ������ ������ ����
	 - D3D11_BLEND_SRC_ALPHA : ���� �μ�(A,A,A,A) �ȼ� ���̴� ���ĵ�����(A) (�� ��� RGB���� ���İ��� �����Ѵٴ� ���ε�..)
	DestBlend : ������ ����� ���� RGB���� ���� ������ �۾��� ���� 
	 - D3D11_BLEND_INV_SRC_ALPHA : ���� ���ʹ� �ȼ� ���̴��� ���� ������ (RGB)��( 1-R,1-G,1-B,1-A) �̴�.
	BlendOP : SrcBlend �� DestBlend �۾��� �����ϴ� ����� ���� 
	 - D3D11_BLEND_OP_ADD : �� ���� �۾��� ���Ѵ�.
	SrcBlendAlpha : �ȼ����̴��� ����ϴ� ���İ��� ���� ������ ������ ����
	 - D3D11_BLEND_ZERO : ���� �μ��� (0,0,0,0) �̴�
	DestBlendAlpha : ������ ����� �ն󰪿� ���� ������ ������ ����
	 - D3D11_BLEND_ONE : ���� �μ��� (1,1,1,1)
	BlendOpAlpha : �� �ΰ��� ���� �۾��� �����ϴ� ��� �� ����*/

	BlendStateDesc.RenderTarget[0].BlendEnable = true;
	BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL & ~D3D11_COLOR_WRITE_ENABLE_ALPHA;//0x0f;

	// ���� ���� On ��� �����ڷ� ���� ���� ����
	result = m_Device->CreateBlendState(&BlendStateDesc, m_AlphaEnableBlendingState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// �̹����� ���� ��� Off ���� �ٲ� �����ڷ� ���� ���¸� �����Ѵ�.
	BlendStateDesc.RenderTarget[0].BlendEnable = false;

	result = m_Device->CreateBlendState(&BlendStateDesc, m_AlphaDisableBlendingState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	return true;

}
bool DirectX::CreateRaster()
{
	HRESULT result = S_OK;
	D3D11_RASTERIZER_DESC RasterDesc;
	// � ������ ��� �׸� ������ �����ϴ� ������ȭ�� description�� �ۼ��մϴ�. 
	RasterDesc.AntialiasedLineEnable = false;
	RasterDesc.CullMode = D3D11_CULL_BACK;
	RasterDesc.DepthBias = 0;
	RasterDesc.DepthBiasClamp = 0.0f;
	RasterDesc.DepthClipEnable = true;
	RasterDesc.FillMode = D3D11_FILL_SOLID;
	RasterDesc.FrontCounterClockwise = false;
	RasterDesc.MultisampleEnable = false;
	RasterDesc.ScissorEnable = false;
	RasterDesc.SlopeScaledDepthBias = 0.0f;

	//D3D11_CLIP_OR_CULL_DISTANCE_COUNT
	// �ۼ��� description���κ��� ������ȭ�� ���¸� �����մϴ�.
	result = m_Device->CreateRasterizerState(&RasterDesc, m_RasterState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// ������ȭ�� ���¸� �����մϴ�. 
	m_DeviceContext->RSSetState(m_RasterState.Get());
	return true;
}
bool DirectX::CreateViewPortAndMatrix()
{
	D3D11_VIEWPORT ViewPort;
	float ScreenAspect = 0;
	// �������� ���� ����Ʈ�� �����մϴ�. 
	ViewPort.Width = (float)m_ScreenWidth;
	ViewPort.Height = (float)m_ScreenHeight;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;


	// ����Ʈ�� �����մϴ�. 
	m_DeviceContext->RSSetViewports(1, &ViewPort);

	// ȭ�� ������ ���Ѵ�.
	ScreenAspect = (float)m_ScreenWidth / (float)m_ScreenHeight;
	// 3D �������� ���� ���� ����� �����մϴ�. 
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, m_fFov, ScreenAspect, m_fNearZ, m_fFarZ);
	// ���� ����� ���� ��ķ� �ʱ�ȭ�մϴ�. 
	D3DXMatrixIdentity(&m_worldMatrix);
	// 2D �������� ���� ���翵 ����� �����մϴ�.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)m_ScreenWidth, (float)m_ScreenHeight, m_fNearZ, m_fFarZ);
	return true;
}
/** 2D Text ���� �ʱ�ȭ */
bool DirectX::CreateDirectText()
{
	HRESULT result = S_OK;
	IDXGIDevice* dxgiDevice = nullptr;
	// D2D���丮 ����
	result = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED,m_D2DFactory.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// DWD���丮 ����
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)m_DWriteFactory.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(result))
	{
		return false;
	} 
	// d2d��ġ ����
	result = m_D2DFactory->CreateDevice(dxgiDevice, m_D2DDevice.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// d2dcontext ����
	result = m_D2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_D2DContext.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
bool DirectX::CreateTextTargetBitmap()
{
	HRESULT result = S_OK;
	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	ComPtr<IDXGISurface> dxgiBackBuffer;
	ZeroMemory(&bitmapProperties, sizeof(D2D1_BITMAP_PROPERTIES1));
	
	bitmapProperties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96.0f, 96.0f);


	result = m_SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)dxgiBackBuffer.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	result = m_D2DContext->CreateBitmapFromDxgiSurface(dxgiBackBuffer.Get(), &bitmapProperties, m_D2DTargetBitmap.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	m_D2DContext->SetTarget(m_D2DTargetBitmap.Get());
	m_D2DContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

	return true;
}
DirectX*        DirectX::GetInstance()
{
	return g_D3D;
}