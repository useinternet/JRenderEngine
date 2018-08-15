#pragma once
#include"JRenderEngine.h"
#include"Actor.h"

namespace JRenderEngine
{
	class TextComponent;
	class Text : public Actor
	{
	private:
		std::unique_ptr<TextComponent> m_TextComponent;
		D3DXVECTOR4 m_TextColor;
	public:
		Text();
		virtual ~Text();

		bool Update(const float deltaTime) override;
		void InputText(const std::wstring string,...);
		void SetColor(const D3DXVECTOR4& color);
		void SetColor(const float r, const float g, const float b, const float a);
		ID3D11ShaderResourceView* GetFontTexture();
		size_t GetTextIndexSize();
	};
};