
#include"JRenderEngine.h"


namespace JRenderEngine
{
	class DirectX
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Device>             m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>      m_DeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>           m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_RenderTargetView;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_DepthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  m_DepthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   m_DepthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>    m_RasterState;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState>  m_DepthDisabledStencilState;
		Microsoft::WRL::ComPtr<ID3D11BlendState>         m_AlphaEnableBlendingState;
		Microsoft::WRL::ComPtr<ID3D11BlendState>         m_AlphaDisableBlendingState;
	private:
		/** 텍스트 2D를 위한 장치 변수들
		*/
		Microsoft::WRL::ComPtr<IDWriteFactory>           m_DWriteFactory;
		Microsoft::WRL::ComPtr<ID2D1Factory1>            m_D2DFactory;
		Microsoft::WRL::ComPtr<ID2D1Device>              m_D2DDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext>       m_D2DContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>             m_D2DTargetBitmap;
	private:

		D3DXMATRIX m_projectionMatrix;
		D3DXMATRIX m_worldMatrix;
		D3DXMATRIX m_orthoMatrix;

		int m_ScreenWidth;
		int m_ScreenHeight;

		float m_fFov;
		float m_fFarZ;
		float m_fNearZ;

	public:
		DirectX();
		~DirectX();
		bool Initialization(const int ScreenWidth, const int ScreenHeight, const HWND hWnd, const bool FullScreen, const float FarZ, const float NearZ);


		void BenginDraw(D3DXCOLOR color);
		void EndDraw();
		void TurnZBufferOn();
		void TurnZBufferOff();
		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		ID3D11Device*           GetDevice();
		ID3D11DeviceContext*    GetDeviceContext();
		IDWriteFactory*         GetWriteFactory();
		ID2D1Device*            Get2DDevice();
		ID2D1DeviceContext*     Get2DDeviceContext();
		ID3D11DepthStencilView* GetDepthStencilView();

		const D3DXMATRIX& GetWorldMatrix();
		const D3DXMATRIX& GetProjectionMatrix();
		const D3DXMATRIX& GetOrthoMatrix();


		void SetRenderTarget();
		void SetViewPort();

	private:
		void Shutdown();
		/** 3Direct 관련 초기화
		*/
		bool CreateDirect3D(const HWND hWnd, const bool FullScreen);
		bool CreateRenderTargetView();
		bool CreateStencilState();
		bool CreateBlendingState();
		bool CreateRaster();
		bool CreateViewPortAndMatrix();

		/** 2D Text 관련 초기화 */
		bool CreateDirectText();
		bool CreateTextTargetBitmap();


	public:
		static DirectX* GetInstance();
	};



}

