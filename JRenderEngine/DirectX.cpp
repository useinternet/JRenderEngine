
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

/** 기본 클래스 메소드 ( 노트 참조 )
초기화 , 업데이트, 렌더링, 셧다운 함수 */
bool DirectX::Initialization(const int ScreenWidth,const int ScreenHeight, const HWND hWnd, const bool FullScreen, const float FarZ, const float NearZ)
{
	bool result;
	m_ScreenWidth  = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	m_fFarZ = FarZ;
	m_fNearZ = NearZ;

	// 스왑체인 및 디바이스, 디바이스 컨텍스트 초기화
	result = CreateDirect3D(hWnd, FullScreen);
	if (!result)
	{
		DebugLog(L"Error : Failed CreateDirect3D!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	//  백버퍼으로 렌더 타겟뷰 초기화 및 깊이 스텐실뷰를 렌더 타겟에 바인딩
	result = CreateRenderTargetView();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateRenderTargetView!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// 스텐실 상태를 설정
	result = CreateStencilState();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateStencilState!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// 블렌딩 모드 상태를 생성합니다.
	result = CreateBlendingState();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateBlendingState!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// 레스터화기 등록
	result = CreateRaster();
	if (!result)
	{
		DebugLog(L"Error : Failed CreateRaster!! \n");
		DebugLog(L"Class : DirectX \n");
		return false;
	}
	// 뷰포트 및 각 투영,세계,정사영 행렬 생성
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
// z버퍼 키고 && 끄기
void DirectX::TurnZBufferOn()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);
	return;
}
void DirectX::TurnZBufferOff()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthDisabledStencilState.Get(), 1);

}
// 블렌딩 키고 && 끄기
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
// 접근자 함수
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
/** 장치 정보를 불러온다 */
bool DirectX::CreateDirect3D(const HWND hWnd, const bool FullScreen)
{
	HRESULT result = S_OK;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	D3D_FEATURE_LEVEL FeatureLevel[] = 
	{ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
	  D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
	};
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	// 스왑 체인 서술
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


	/** 렌더 타겟 뷰 생성  */
	// 백버퍼의 포인터를 가져옵니다.
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBufferPtr.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// 백버퍼의 포인터로 렌더타겟 뷰를 생성합니다.
	result = m_Device->CreateRenderTargetView(BackBufferPtr.Get(), NULL, m_RenderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	/** 깊이 버퍼 생성 */
	// 깊이 버퍼의 서술사 작성
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
	// 깊이 버퍼 생성
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
	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;            // z버퍼 스텐실 상태
	D3D11_DEPTH_STENCIL_DESC DepthDisalbedStencilDesc;    // z버퍼(x) 스텐실 상태(2d렌더링 용)
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;

	// 스텐실 상태의 서술자 작성
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
	// 스텐실 상태 생성
	result = m_Device->CreateDepthStencilState(&DepthStencilDesc, m_DepthStencilState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// 깊이-스텐실 상태를 설정
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 1);



	// 깊이-스텐실 뷰의 서술자 작성
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이-스텐실 뷰를 생성. 
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &DepthStencilViewDesc, m_DepthStencilView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// 렌더타겟 뷰와 깊이-스텐실 버퍼를 각각 출력 파이프라인에 바인딩합니다.
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencilView.Get());
	
	// 2D 렌더링을 위한 깊이- 스텐실 상태를 만든다. (zbuffer 끄기용)
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
	// 2D 렌더링용 깊이 - 스텐실 상태를 만든다.
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

	// 블렌드 On 모드 서술자 작성 
	/** 블렌드 모드 설명 
	SrcBlend : 픽셀 셰이더가 출력하는 RGB 값에 대해 수행할 연산을 지정
	 - D3D11_BLEND_SRC_ALPHA : 블렌드 인수(A,A,A,A) 픽셀 세이더 알파데이터(A) (즉 모든 RGB값에 알파값을 대입한다는 뜻인듯..)
	DestBlend : 렌더링 대상의 현재 RGB값에 대해 수행할 작업을 지정 
	 - D3D11_BLEND_INV_SRC_ALPHA : 블랜드 팩터는 픽셀 쉐이더의 색상 데이터 (RGB)인( 1-R,1-G,1-B,1-A) 이다.
	BlendOP : SrcBlend 와 DestBlend 작업을 결합하는 방법을 정의 
	 - D3D11_BLEND_OP_ADD : 두 블렌딩 작업을 더한다.
	SrcBlendAlpha : 픽셀셰이더가 출력하는 알파값에 대해 수행할 연산을 지정
	 - D3D11_BLEND_ZERO : 블렌딩 인수는 (0,0,0,0) 이다
	DestBlendAlpha : 렌더링 대상의 앞라값에 대해 수행할 연산을 지정
	 - D3D11_BLEND_ONE : 블렌딩 인수는 (1,1,1,1)
	BlendOpAlpha : 위 두개의 블렌딩 작업을 결합하는 방법 을 정의*/

	BlendStateDesc.RenderTarget[0].BlendEnable = true;
	BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL & ~D3D11_COLOR_WRITE_ENABLE_ALPHA;//0x0f;

	// 알파 블렌딩 On 모드 서술자로 블렌딩 상태 생성
	result = m_Device->CreateBlendState(&BlendStateDesc, m_AlphaEnableBlendingState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}


	// 이번에는 블렌딩 모드 Off 모드로 바꾼 서술자로 블렌딩 상태를 생성한다.
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
	// 어떤 도형을 어떻게 그릴 것인지 결정하는 래스터화기 description을 작성합니다. 
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
	// 작성한 description으로부터 래스터화기 상태를 생성합니다.
	result = m_Device->CreateRasterizerState(&RasterDesc, m_RasterState.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// 래스터화기 상태를 설정합니다. 
	m_DeviceContext->RSSetState(m_RasterState.Get());
	return true;
}
bool DirectX::CreateViewPortAndMatrix()
{
	D3D11_VIEWPORT ViewPort;
	float ScreenAspect = 0;
	// 렌더링을 위한 뷰포트를 설정합니다. 
	ViewPort.Width = (float)m_ScreenWidth;
	ViewPort.Height = (float)m_ScreenHeight;
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.TopLeftX = 0.0f;
	ViewPort.TopLeftY = 0.0f;


	// 뷰포트를 생성합니다. 
	m_DeviceContext->RSSetViewports(1, &ViewPort);

	// 화면 비율을 구한다.
	ScreenAspect = (float)m_ScreenWidth / (float)m_ScreenHeight;
	// 3D 렌더링을 위한 투영 행렬을 생성합니다. 
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, m_fFov, ScreenAspect, m_fNearZ, m_fFarZ);
	// 월드 행렬을 단위 행렬로 초기화합니다. 
	D3DXMatrixIdentity(&m_worldMatrix);
	// 2D 렌더링에 사용될 정사영 행렬을 생성합니다.
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float)m_ScreenWidth, (float)m_ScreenHeight, m_fNearZ, m_fFarZ);
	return true;
}
/** 2D Text 관련 초기화 */
bool DirectX::CreateDirectText()
{
	HRESULT result = S_OK;
	IDXGIDevice* dxgiDevice = nullptr;
	// D2D팩토리 생성
	result = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED,m_D2DFactory.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// DWD팩토리 생성
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
	// d2d장치 생성
	result = m_D2DFactory->CreateDevice(dxgiDevice, m_D2DDevice.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	// d2dcontext 생성
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