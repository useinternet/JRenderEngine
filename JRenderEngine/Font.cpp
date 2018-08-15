#include"Font.h"
#include<fstream>
using namespace JRenderEngine;
using namespace Microsoft::WRL;
using namespace std;


Font::Font()
{
	m_FontInformation = make_unique<FontInformation>();
	m_FontCommonInformation = make_unique<FontCommonType>();
}
Font::~Font()
{

}
bool Font::LoadFont(ID3D11Device* pDevice, const std::string& FontName, const std::wstring& FontTextureName, const float Size)
{
	m_Size = Size;
	ifstream fin;
	// ���� ����
	fin.open(FontName);
	if (fin.fail())
	{
		DebugLog(L"Failed LoadFont : %s \n", FontName.c_str());
		return false;
	}
	// 
	LoadFontTexture(pDevice,FontTextureName);
	LoadFontInformation(fin);
	LoadFontCommonInformation(fin);
	LoadFontData(fin);
	LoadKerningInformation(fin);

	float weight = m_FontCommonInformation->scaleW;
	float height = m_FontCommonInformation->scaleH;
	//float size = (float)m_FontInformation->FontSize;
	for (auto& iter : m_Font)
	{
		FontVertex Mesh;
		// ��Ʈ ũ��( ȭ��� ��ġ��)
		float Height = (float)iter.second.Height * m_Size;
		float Width = (float)iter.second.Width * m_Size;
		// ��Ʈ ũ�� (�ؽ��� ��ġ ��)
		float sizeW = iter.second.Width / weight;
		float sizeH = iter.second.Height / height;
		// �ؽ��� uv;
		float posX = iter.second.x / weight;
		float posY = iter.second.y / height;

		// �ﰢ�� 1
		Mesh.position = { 0.0f, Height, 0.0f };
		Mesh.texture = { posX, posY};
		iter.second.Mesh.push_back(Mesh);

		Mesh.position = { Width, Height ,0.0f };
		Mesh.texture = { posX + sizeW , posY };
		iter.second.Mesh.push_back(Mesh);

		Mesh.position = { 0.0f,0.0f,0.0f };
		Mesh.texture = { posX, posY + sizeH };
		iter.second.Mesh.push_back(Mesh);

		// �ﰢ�� 2

		Mesh.position = { Width,Height, 0.0f };
		Mesh.texture = { posX + sizeW, posY};
		iter.second.Mesh.push_back(Mesh);

		Mesh.position = { Width, 0.0f, 0.0f };
		Mesh.texture = { posX + sizeW, posY + sizeH };
		iter.second.Mesh.push_back(Mesh);


		Mesh.position = { 0.0f, 0.0f ,0.0f };
		Mesh.texture = { posX, posY + sizeH };
		iter.second.Mesh.push_back(Mesh);


	}
	return true;
}
void Font::TransStringPosition(const std::wstring& str, std::vector<FontVertex>* fonts, std::vector<unsigned int>* fontIndex)
{
	CharID ID;
	FontVertex vertex;
	float AccX = 0.0f;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] == ' ')
		{
			AccX += (float)((m_FontInformation->FontSize * m_Size) / 2);
			continue;
		}
		ID = (int)str[i];
		for (auto& iter : m_Font[ID].Mesh)
		{
			vertex = iter;
			vertex.position.x += AccX;
			fonts->push_back(vertex);
		}
		AccX += (m_Font[ID].XAdvance + (m_Font[ID].XAdvance / 5)) * m_Size;
	}
	for (size_t i = 0; i < fonts->size(); ++i)
	{
		fontIndex->push_back(i);
	}
}
ID3D11ShaderResourceView* Font::GetTexture()
{
	return m_FontTexture.Get();
}
bool Font::LoadFontTexture(ID3D11Device* pDevice,const std::wstring& FontTextureName)
{
	// �ؽ��� �ε�
	HRESULT result;

	// �̹����� �ε��Ѵ�.
	result = D3DX11CreateShaderResourceViewFromFile(pDevice, FontTextureName.c_str(), nullptr, nullptr,
		m_FontTexture.GetAddressOf(), nullptr);
	if (FAILED(result))
	{
		DebugLog(L"Error : Failed TextureLoad!! \n");
		return false;
	}
	return true;
}
void Font::LoadFontInformation(std::ifstream& fin)
{
	char Temp = '.';
	size_t Count = 0;
	EqualLoop(fin);

	// �̸� �ޱ�
	fin.get(Temp);
	fin.get(Temp);
	while (Temp != '\"')
	{
		m_FontInformation->FontName.push_back(Temp);
		fin.get(Temp);
	}
	Count = 0;
	EqualLoop(fin);

	// ������ �ޱ�
	fin>>m_FontInformation->FontSize;
	EqualLoop(fin);
	// Bold �� �ޱ�
	fin >> m_FontInformation->bold;
	EqualLoop(fin);
	// iteralic�� �ޱ�
	fin >> m_FontInformation->italic;
	EqualLoop(fin);


	// charset �� �ޱ�
	fin.get(Temp);
	fin.get(Temp);
	while (Temp != '\"')
	{
		m_FontInformation->FontName.push_back(Temp);
		fin.get(Temp);
	}
	Count = 0;
	EqualLoop(fin);

	// unicode
	fin >> m_FontInformation->unicode;
	EqualLoop(fin);

	// stretchH
	fin >> m_FontInformation->stretchH;
	EqualLoop(fin);

	// smooth
	fin >> m_FontInformation->smoothing;
	EqualLoop(fin);

	//aa
	fin >> m_FontInformation->aa;
	EqualLoop(fin);

	// padding
	for (int i = 0; i < 4; ++i)
	{
		fin >> m_FontInformation->padding[i];
		fin.get(Temp);
	}
	EqualLoop(fin);
	// spacing
	for (int i = 0; i < 2; ++i)
	{
		fin >> m_FontInformation->spacing[i];
		fin.get(Temp);
	}
	EqualLoop(fin);

	//outline
	fin >> m_FontInformation->outline;

}
void Font::LoadFontCommonInformation(std::ifstream& fin)
{
	char Temp = '.';
	size_t Count = 0;
	EqualLoop(fin);

	// lineHeight
	fin >> m_FontCommonInformation->lineHeight;
	EqualLoop(fin);


	// base
	fin >> m_FontCommonInformation->base;
	EqualLoop(fin);


	// scaleW
	fin >> m_FontCommonInformation->scaleW;
	EqualLoop(fin);


	// scaleH
	fin >> m_FontCommonInformation->scaleH;
	EqualLoop(fin);


	// pages
	fin >> m_FontCommonInformation->pages;
	EqualLoop(fin);

	// packed
	fin >> m_FontCommonInformation->packed;
	EqualLoop(fin);

	// alphaChnl
	fin >> m_FontCommonInformation->alphaChnl;
	EqualLoop(fin);

	// redChnl
	fin >> m_FontCommonInformation->redChnl;
	EqualLoop(fin);

	// greenChnl
	fin >> m_FontCommonInformation->greenChnl;
	EqualLoop(fin);

	// blueChnl
	fin >> m_FontCommonInformation->blueChnl;
	EqualLoop(fin);

	// pageID
	fin >> m_FontCommonInformation->pageID;
	EqualLoop(fin);

	// pages
	fin.get(Temp);
	fin.get(Temp);
	while (Temp != '\"')
	{
		m_FontCommonInformation->FileName.push_back(Temp);
		fin.get(Temp);
	}
	Count = 0;
}
void Font::LoadFontData(std::ifstream& fin)
{
	char Temp = '.';
	FontType Font;
	size_t Count = 0;
	EqualLoop(fin);

	// CharCount
	fin >> CharCount;
	EqualLoop(fin);

	for (size_t i = 0; i < CharCount; ++i)
	{
		// CharID
		fin >> Font.ID;
		EqualLoop(fin);

		// x
		fin >> Font.x;
		EqualLoop(fin);

		// y
		fin >> Font.y;
		EqualLoop(fin);

		// Width
		fin >> Font.Width;
		EqualLoop(fin);

		// Height
		fin >> Font.Height;
		EqualLoop(fin);

		// XOffset
		fin >> Font.XOffset;
		EqualLoop(fin);

		// YOffset
		fin >> Font.YOffset;
		EqualLoop(fin);

		// XAdvance
		fin >> Font.XAdvance;
		EqualLoop(fin);

		// Page
		fin >> Font.Page;
		EqualLoop(fin);

		// chnl
		fin >> Font.chnl;
		EqualLoop(fin);

		m_Font.insert(pair<const CharID, FontType>(Font.ID, Font));
	}
}
void Font::LoadKerningInformation(std::ifstream& fin)
{
	char Temp = '.';
	KerningType Kerning;
	size_t Count = 0;

	// KerningCount;
	fin >> KerningCount;
	EqualLoop(fin);

	for (size_t i = 0; i < KerningCount; ++i)
	{
		// first
		fin >> Kerning.first;
		EqualLoop(fin);

		// second
		fin >> Kerning.second;
		EqualLoop(fin);

		// amount
		fin >> Kerning.amount;
		if (i != KerningCount - 1)
		{
			EqualLoop(fin);
		}
		m_KerningInformation.insert(pair<PairID, const float>(PairID(Kerning.first, Kerning.second), Kerning.amount));
	}

}
void Font::EqualLoop(ifstream& fin)
{
	char Temp = ' ';
	while (Temp != '=')
	{
		fin.get(Temp);
	}
}