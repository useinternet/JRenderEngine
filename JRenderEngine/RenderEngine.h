#pragma once
#include"JRenderEngine.h"

#define FARZ       1000.0f
#define NEARZ      0.01f
#define FULLSCREEN false

// �ӽ�
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
		// ������ ���� �ھ� 
		std::unique_ptr<DirectX> m_DirectX;
		// ���� ����, �ؽ��� �ε�
		std::unique_ptr<AssetLoader>   m_Asset;
		std::unique_ptr<TextureLoader> m_TextureLoader;
		std::unique_ptr<FontLoader>    m_FontLoader;
		std::unique_ptr<TerrainMapLoader> m_TerrainMapLoader;
		// ���̴� �Ŵ��� 
		std::unique_ptr<ShaderManager> m_ShaderManager;

		// ����
		D3DXMATRIX m_2DviewMatrix;
		std::unique_ptr<GameObject>  m_Wall;
		std::unique_ptr<GameObject>  m_Bath;
		std::unique_ptr<GameObject>  m_Ground;
		std::unique_ptr<MyPawn>      m_Pawn;





		// ����� �ؽ�Ʈ
		std::unique_ptr<Text>        m_FpsText;
		std::unique_ptr<Text>        m_CharacterPositionText;
		std::unique_ptr<Text>        m_CharacterRotationText;


		// ����
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


