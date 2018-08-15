#pragma once
#include"JRenderEngine.h"
#include<vector>
#include<map>

namespace JRenderEngine
{
	struct FontMesh;
	struct FontInformation;
	struct FontCommonType;
	struct FontType;
	struct KerningType;
	typedef unsigned int CharID;
	typedef std::pair<const CharID, const CharID> PairID;
	struct FontVertex
	{
		D3DXVECTOR3 position = { 0.0f,0.0f,0.0f };
		D3DXVECTOR2 texture = { 0.0f,0.0f };
	};
	struct FontInformation
	{
		/*
		FontName : ���� Ÿ�� ��Ʈ�� �̸��Դϴ�.
		FontSize : Ʈ�� Ÿ�� ��Ʈ�� ũ��.
		bold     : �۲��� ���� ǥ�õ˴ϴ�.
		italic   : �� ����� ���Դϴ�.
		charset  : ��� �� OEM ���� ��Ʈ�� �̸� (���� �ڵ尡 �ƴ� ���).
		unicode  : ���� �ڵ� ���� ��Ʈ�̸� 1�� �����Ͻʽÿ�.
		stretchH : �۲� ���̰� ������� �þ�ϴ�. 100 %�� ���༺�� ������ �ǹ��մϴ�.
		smoothing: smoothing�� ���� ������ 1�� �����Ͻʽÿ�.
		aa       : ��� �� ���� ���ø� ����. 1�� ���� ���ø��� ������ �ʾ� ���� �ǹ��մϴ�.
		padding[4] : �� ������ �е� (����, ������, �Ʒ���, ����).
		spacing[2] : �� ������ ���� (����, ����).
		outline    : ������ ������ �β��Դϴ�.
		*/
		std::string  FontName;
		size_t FontSize;
		size_t bold;
		size_t italic;
		std::string  charset;
		bool  unicode;
		int   stretchH;
		bool  smoothing;
		bool  aa;
		int padding[4];
		int spacing[2];
		size_t outline;
	};
	struct FontCommonType
	{
		/*
		lineHeight : �� �ؽ�Ʈ �� ������ �Ÿ� (�ȼ� ����)�Դϴ�.
		base       : ���� ���� ��ܿ��� ������ �ر����� �ȼ� ���Դϴ�.
		scaleW     : �ؽ�ó�� ��. �Ϲ������� ���� �̹����� x pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
		scaleH     : �ؽ�ó�� ����. �Ϲ������� ���� �̹����� y pos ũ�⸦ �����ϴ� �� ���˴ϴ�.
		pages      : �۲ÿ� ���� �� �ؽ��� �������� ��.
		packed     : �ܻ� ���ڰ� �� �ؽ�ó ä�ο� ����Ǿ� ������ 1�� �����մϴ�. �� ��� alphaChnl�� �� ä�ο� ����� ������ �����մϴ�.
		alphaChnl  : ä�ο� �׸� �� �����Ͱ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� ������ ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		redChnl    : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �� ������ �ܰ����� ���� �����ϰ��ִ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		greenChnl  : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		blueChnl   : ä�ο� �׸� �� �����Ͱ� ���� �����ǰ��ִ� ���� 0����, ������ ���� �����ϰ��ִ� ���� 1, �׸� �Ŀ� �ܰ����� ���� �����ϴ� ���� 2, �� ���� ������ ���� 3, �� ���� ���� 4���˴ϴ�.
		pageID     : ������ ID.
		FileName   : �ؽ�ó ���� �̸�.
		*/
		float lineHeight;
		float base;
		float scaleW;
		float scaleH;
		float pages;
		float packed;
		float alphaChnl;
		float redChnl;
		float greenChnl;
		float blueChnl;
		float pageID;
		std::string FileName;
	};
	struct FontType
	{
		/*
		ID      : ���� ID�Դϴ�.
		x       : �ؽ����� ���� �̹����� ���� ��ġ.
		y       : �ؽ�ó�� ���� �̹����� ���� ��ġ�Դϴ�.
		Width   : �ؽ�ó�� ���� �̹����� �ʺ��Դϴ�.
		Height  : �ؽ�ó�� ���� �̹����� �����Դϴ�.
		XOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
		YOffset : �ؽ�ó���� ȭ������ �̹����� ���� �� �� ���� ��ġ�� �󸶳� �����µǾ�� �ϴ����� �����մϴ�.
		XAdvance : ĳ���� �׸��� �� ���� ��ġ�� �󸶸�ŭ �մ�� ���մϴ�.
		Page    : ���� �̹������ִ� �ؽ��� ������.
		chnl    : ���� �̹������ִ� �ؽ�ó ä�� (1 = �Ķ���, 2 = ���, 4 = ������, 8 = ����, 15 = ��� ä��).

		*/
		CharID ID;
		size_t x;
		size_t y;
		size_t Width;
		size_t Height;
		float  XOffset;
		float  YOffset;
		float XAdvance;
		float Page;
		float chnl;
		std::vector<FontVertex> Mesh;
	};
	struct KerningType
	{
		/*
		first  : ù ��° ���� ID�Դϴ�.
		second : �� ��° ���� ID�Դϴ�.
		amount : ù ��° ���� �ٷ� ������ �� ��° ���ڸ� �׸� �� x ��ġ�� �����ؾ��ϴ� �����Դϴ�.
		*/
		CharID first;
		CharID second;
		float amount;
	};
	class Font
	{
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_FontTexture;
		std::unique_ptr<FontInformation> m_FontInformation;
		std::unique_ptr<FontCommonType>  m_FontCommonInformation;
		std::map<const CharID,FontType> m_Font;
		std::map<PairID,const float> m_KerningInformation;
		size_t CharCount;
		size_t KerningCount;
		float m_Size;
	public:
		Font();
		~Font();
		bool LoadFont(ID3D11Device* pDevice, const std::string& FontName,const std::wstring& FontTextureName, const float Size = 1.0f);
	    void TransStringPosition(const std::wstring& str,std::vector<FontVertex>* fonts,std::vector<unsigned int>* fontIndex);
		ID3D11ShaderResourceView* GetTexture();
	private:
		bool LoadFontTexture(ID3D11Device* pDevice,const std::wstring& FontTextureName);
		void LoadFontInformation(std::ifstream& fin);
		void LoadFontCommonInformation(std::ifstream& fin);
		void LoadFontData(std::ifstream& fin);
		void LoadKerningInformation(std::ifstream& fin);
		void EqualLoop(std::ifstream& fin);

	};
}