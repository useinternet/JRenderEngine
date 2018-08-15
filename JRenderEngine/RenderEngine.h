#pragma once
#include"JRenderEngine.h"

#define FARZ       1000.0f
#define NEARZ      0.01f
#define FULLSCREEN false

// 임시
#include"Actor.h"
#include"World.h"
#include"MyPawn.h"
#include"Text.h"

namespace JRenderEngine
{
	class DirectX;
	class AssetLoader;
	class TextureLoader;
	class FontLoader;
	class TerrainMapLoader;
	class ShaderManager;
	class Terrain;

	class RenderEngine
	{
	private:
		// 렌더링 엔진 코어 
		std::unique_ptr<DirectX> m_DirectX;
		// 각종 에셋, 텍스쳐 로드
		std::unique_ptr<AssetLoader>   m_Asset;
		std::unique_ptr<TextureLoader> m_TextureLoader;
		std::unique_ptr<FontLoader>    m_FontLoader;
		std::unique_ptr<TerrainMapLoader> m_TerrainMapLoader;
		// 셰이더 매니저 
		std::unique_ptr<ShaderManager> m_ShaderManager;

		// 실험
		D3DXMATRIX m_2DviewMatrix;
		std::unique_ptr<GameObject>  m_Wall;
		std::unique_ptr<GameObject>  m_Bath;
		std::unique_ptr<GameObject>  m_Ground;
		std::unique_ptr<MyPawn>      m_Pawn;





		// 디버깅 텍스트
		std::unique_ptr<Text>        m_FpsText;
		std::unique_ptr<Text>        m_CharacterPositionText;
		std::unique_ptr<Text>        m_CharacterRotationText;


		// 지형
		std::unique_ptr<Terrain> m_Terrain;
	public:


		RenderEngine();
		~RenderEngine();

		bool Initialization(HWND hWnd, int nScreenWidth, int nScreenHeight);
		bool Update(const float deltaTime);
	private:
		void Frame();
		void Render();
		void TextRender(const float deltaTime);
		void DebugLogOnScreen();

	};

}


