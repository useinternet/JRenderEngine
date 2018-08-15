#pragma once
#include"JRenderEngine.h"
#include"Component.h"
#include"Font.h"
#include"Mesh.h"
/*
텍스트 컴포넌트.. 셰이더 분리..

*/

namespace JRenderEngine
{
	class MeshMessage;
	class TextComponentMessage : public ComponentMessage
	{
	public:
		enum
		{
			TextWrite,FontInit
		};
	public:
		TextComponentMessage() : ComponentMessage() {}
		virtual ~TextComponentMessage() {}
		unsigned int Flag;
		std::wstring Sentence = L"Default";
		std::string FontName = "Godic";
	};
	class TextComponent : public Component
	{
	private:
		Font * m_Font;
		std::unique_ptr<std::wstring> m_Text;
		std::vector<FontVertex> m_FontVertex;
		std::vector<unsigned int> m_FontIndex;
	public:
		TextComponent();
		virtual ~TextComponent();
		virtual bool Update(const float deltaTime) override;
		virtual void Receive(ComponentMessage* msg) override;
		void SetFont(const std::string& fontName);
		Font* GetFont();
		size_t GetIndexSize();
		void GetMeshInformation(MeshMessage* msg);

	};
}