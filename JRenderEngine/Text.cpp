#include"Text.h"
#include"TextComponent.h"
#include"RenderComponent.h"
using namespace JRenderEngine;
using namespace std;

Text::Text()
{
	SetName(ObjectID::Text);
	m_TextComponent = make_unique<TextComponent>();
	GameObject::InsertComponent<TextComponent>(m_TextComponent.get());
	D3DXVECTOR4 m_TextColor = { 1.0f,1.0f,1.0f,1.0f };
	// 임시 기본
	TextComponentMessage msg;
	msg.Flag = TextComponentMessage::FontInit;
	Send(&msg);
	//m_TextComponent->SetFont("Godic");
}
Text::~Text()
{

}

bool Text::Update(const float deltaTime)
{
	Actor::Update(deltaTime);

	return true;
}
void Text::InputText(const wstring string,...)
{
	// 가변 인자 받기
	WCHAR str[256];
	va_list ArgList;
	va_start(ArgList, string);
	wvsprintf(str, string.c_str(), ArgList);
	//vwprintf(string.c_str(), ArgList);
	va_end(ArgList);

	// 최종 문자열 결과물..
	wstring result = str;

	// 텍스트 컴포넌트와 통신
	TextComponentMessage TextMessage;
	TextMessage.Flag = TextComponentMessage::TextWrite;
	TextMessage.Sentence = result;

	// 보낸다.
	Send(&TextMessage);

	// 렌더 컴포넌트에도 알린다.
	RenderComponent* Component = dynamic_cast<RenderComponent*>(GetComponent(ComponentName::RenderComponent));
	if (Component == nullptr)
	{
	DebugLog(L"Error : Faild Get RenderComponent ! \n");
	DebugLog(L"Class : Text \n");
	DebugLog(L"Func  : InputText \n");
	}
	// 메쉬 정보를 입력하여 렌더 컴포넌트에 정보를 전달한다.
	MeshMessage meshmsg;
	m_TextComponent->GetMeshInformation(&meshmsg);

	RenderComponentMessage RenderMessage;
	RenderMessage.bDefault = false;
	RenderMessage.MeshMsg = meshmsg;
	Send(&RenderMessage);
}

void Text::SetColor(const D3DXVECTOR4& color)
{
	m_TextColor = color;
}
void Text::SetColor(const float r, const float g, const float b, const float a)
{
	m_TextColor = { r,g,b,a };
}
ID3D11ShaderResourceView* Text::GetFontTexture()
{
	return m_TextComponent->GetFont()->GetTexture();
}
size_t Text::GetTextIndexSize()
{
	return m_TextComponent->GetIndexSize();
}