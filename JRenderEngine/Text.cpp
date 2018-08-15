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
	// �ӽ� �⺻
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
	// ���� ���� �ޱ�
	WCHAR str[256];
	va_list ArgList;
	va_start(ArgList, string);
	wvsprintf(str, string.c_str(), ArgList);
	//vwprintf(string.c_str(), ArgList);
	va_end(ArgList);

	// ���� ���ڿ� �����..
	wstring result = str;

	// �ؽ�Ʈ ������Ʈ�� ���
	TextComponentMessage TextMessage;
	TextMessage.Flag = TextComponentMessage::TextWrite;
	TextMessage.Sentence = result;

	// ������.
	Send(&TextMessage);

	// ���� ������Ʈ���� �˸���.
	RenderComponent* Component = dynamic_cast<RenderComponent*>(GetComponent(ComponentName::RenderComponent));
	if (Component == nullptr)
	{
	DebugLog(L"Error : Faild Get RenderComponent ! \n");
	DebugLog(L"Class : Text \n");
	DebugLog(L"Func  : InputText \n");
	}
	// �޽� ������ �Է��Ͽ� ���� ������Ʈ�� ������ �����Ѵ�.
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