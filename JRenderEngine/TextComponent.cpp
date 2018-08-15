#include"TextComponent.h"
#include"FontLoader.h"

using namespace JRenderEngine;
using namespace std;


TextComponent::TextComponent() : Component()
{
	SetName(ComponentName::TextComponent);
	m_Text = make_unique<wstring>();
	m_Font = nullptr;
}
TextComponent::~TextComponent()
{


}

bool TextComponent::Update(const float deltaTime)
{
	Component::Update(deltaTime);

	if (m_Font == nullptr)
	{
		DebugLog(L"Error : TextComponent's Font is nullptr !! \n");
	}
	return true;
}
void TextComponent::Receive(ComponentMessage* msg)
{
	Component::Receive(msg);


	TextComponentMessage* Msg = dynamic_cast<TextComponentMessage*>(msg);
	if (Msg)
	{
		switch (Msg->Flag)
		{
		case TextComponentMessage::TextWrite:
			*m_Text = Msg->Sentence;
			if (m_FontVertex.size() != 0 || m_FontIndex.size() != 0)
			{
				m_FontVertex.clear();
				m_FontIndex.clear();
			}
			if (m_Font)
			{
				m_Font->TransStringPosition(*m_Text, &m_FontVertex, &m_FontIndex);
			}
			break;
		case TextComponentMessage::FontInit:
			m_Font = FontLoader::LoadFont(Msg->FontName);
			break;
		default:
			break;
		}

	}

	// 문자를 전달받아 메쉬 생성
	/*if (msg.Name == ComponentName::TextComponent)
	{
		*m_Text = *(wstring*)msg.Slot;
		// FontVertex의 사이즈가 0이 아니라면 리셋하고 다시 만든다.
		if (m_FontVertex.size() != 0 || m_FontIndex.size() != 0)
		{
			m_FontVertex.clear();
			m_FontIndex.clear();
		}
		if (m_Font)
		{
			m_Font->TransStringPosition(*m_Text,&m_FontVertex,&m_FontIndex);
		}
	}*/
}
void TextComponent::SetFont(const std::string& fontName)
{
	m_Font = FontLoader::LoadFont(fontName);
}
Font* TextComponent::GetFont() { return m_Font; }
size_t TextComponent::GetIndexSize() { return m_FontVertex.size(); }
void TextComponent::GetMeshInformation(MeshMessage* msg)
{
	msg->Flags = Vertex_Dynamic | Index_Dynamic;
	msg->VertexElementSize = m_FontVertex.size();
	msg->VertexSize = sizeof(FontVertex);
	msg->VertexType = &m_FontVertex[0];

	msg->IndexElementSize = m_FontIndex.size();
	msg->IndexSize = sizeof(unsigned int);
	msg->IndexType = &m_FontIndex[0];
}

