#include"TerrainMapLoader.h"
#include<stdio.h>
using namespace JRenderEngine;
using namespace std;
std::map<const string, unique_ptr<TerrainMesh>> TerrainMapLoader::m_mTerrain;
float TerrainMapLoader::m_TextureWrap = 8;


TerrainMapLoader::TerrainMapLoader()
{

}
TerrainMapLoader::~TerrainMapLoader()
{

}


bool TerrainMapLoader::TerrainMapInit()
{
	LoadHeightMap("HeightMap","../HeightMap/HeightMap00.bmp");
	return true;
}
void TerrainMapLoader::LoadDefaultGridMap(TerrainMesh* Mesh, const size_t Width, const size_t Height)
{
	TerrainVertex TerrInfor;
	//TerrInfor.Color = { 1.0f,1.0f,1.0f,1.0f };
	for (size_t j = 0; j < Height - 1; ++j)
	{
		for (size_t i = 0; i < Width - 1; ++i)
		{
			// Line 1 
			// ���� ��
			TerrInfor.Position = { (float)i, 0.0f , (float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ������ ��
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 2 
			// ������ ��
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ������ �Ʒ�
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 3
			// ������ �Ʒ�
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ���� �Ʒ�
			TerrInfor.Position = { (float)i ,0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 4
			// ���� �Ʒ�
			TerrInfor.Position = { (float)i ,0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ���� ��
			TerrInfor.Position = { (float)i,0.0f,(float)(j + 1)};
			Mesh->vTerrainVertex.push_back(TerrInfor);
		}
	}

	for (size_t i = 0; i < Mesh->vTerrainVertex.size(); ++i)
	{
		Mesh->vIndex.push_back(i);
	}
}
TerrainMesh* TerrainMapLoader::LoadTerrain(const std::string& name)
{
	return m_mTerrain[name].get();
}
bool TerrainMapLoader::LoadHeightMap(const std::string& name, const std::string& Directory)
{
	FILE* filePtr = nullptr;

	// ���� �б�
	if (fopen_s(&filePtr, Directory.c_str(), "rb") != 0)
	{
		return false;
	}
	/* ��Ʈ�ʿ� ���� ����

	- ��Ʈ�� ���� ����

	���� ���(BITMAPFILEHEADER) -> ���� ���� ���(BITMAPINFOHEADER) -> �ȷ�Ʈ -> �ȼ� ������

	// �ȼ� �����͸� �ҷ����� �̰ſ� ����� ���̸� ���� ������ vertex���� ��ġ���� ���Ѵ�.


	typedef struct tagBITMAPFILEHEADER
	{
	WORD  bfType;                         // ��Ʈ���� ���ϴ� 'BM' ���ĺ��� ���õȴ�.
	DWORD bfSize;                         // ���� ��ü ũ�⸦ ǥ���Ѵ�.
	WORD  bfReserved1;                    // ��� ����
	WORD  bfReserved2;                    // ��� ����
	DWORD bfOffBits;                      // ���� �ȼ� �������� ���� offset �ּҸ� ����Ʈ ���� ��Ÿ����.
	}

	// ��Ʈ�� ������ ������ ���� ������ �Ǿ� ����ȴ�.

	�ڼ��� ������
	http://edps5091.tistory.com/entry/bitmap%ED%8C%8C%EC%9D%BC%EA%B5%AC%EC%A1%B0-1 ����..

	*/


	// ��Ʈ������ �б�
	BITMAPFILEHEADER bitmapFileHeader;
	if (fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr) != 1)
	{
		return false;
	}

	BITMAPINFOHEADER bitmapInforHeader;
	if (fread(&bitmapInforHeader, sizeof(BITMAPINFOHEADER), 1, filePtr) != 1)
	{
		return false;
	}

	// ���� ũ�� ����
	float Width = (float)bitmapInforHeader.biWidth;
	float Height = (float)bitmapInforHeader.biHeight;


	// ��Ʈ�� �̹��� ������ ũ�� ���
	int imageSize = (int)Width * (int)Height * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// ��Ʈ�� �������� ���� �κ����� �̵�
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// ��Ʈ�� �̹��� �����͸� �д´�.
	if (fread(bitmapImage, 1, imageSize, filePtr) != imageSize)
	{
		return false;
	}
	// ���� �ݱ�		

	if (fclose(filePtr) != 0)
	{
		return false;
	}

	unique_ptr<TerrainMesh> Mesh = make_unique<TerrainMesh>();
	float IncrementTex = m_TextureWrap / (float)Width;
	int   incrementCount = (int)Width / (size_t)m_TextureWrap;
	int wCount = 0;
	int hCount = 0;
	int pos = 0;
	float scale = 0.1f;
	TerrainVertex TerrInfor;
	// ���ؽ� ���ϱ�
	for (size_t j = 0; j < Height; ++j)
	{
		for (size_t i = 0; i < Width; ++i)
		{
			float IncrementW = IncrementTex * wCount;
			float IncrementH = 1 - (IncrementTex * hCount) - IncrementTex;
			int indexLD = ( ((int)Height*j) + i ) * 3;
			int indexRD = (((int)Height*j) + (i+1)) * 3;
			int indexLU = (((int)Height*(j+1)) + i) * 3;
			int indexRU = (((int)Height*(j+1)) + (i+1)) * 3;
			// �ε����� �Ѿ���� ��źȭ
			if (indexRD >= imageSize) indexRD = imageSize - 1;
			if (indexLU >= imageSize) indexLU = indexLD;
			if (indexRU >= imageSize) indexRU = indexRD;
			// Line 1 
			// ���� �Ʒ�
			TerrInfor.Position = { (float)i ,(float)bitmapImage[indexLD] * scale,(float)j };
			TerrInfor.Texure = { IncrementW,IncrementH + IncrementTex };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ���� ��
			TerrInfor.Position = { (float)i, (float)bitmapImage[indexLU] * scale , (float)(j + 1) };
			TerrInfor.Texure = { IncrementW,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ������ ��
			TerrInfor.Position = { (float)(i + 1),(float)bitmapImage[indexRU] * scale,(float)(j + 1) };
			TerrInfor.Texure = { IncrementW + IncrementTex,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);


			// Line 2 
			// ���� �Ʒ�
			TerrInfor.Position = { (float)i ,(float)bitmapImage[indexLD] * scale,(float)j };
			TerrInfor.Texure = { IncrementW ,IncrementH + IncrementTex };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ������ ��
			TerrInfor.Position = { (float)(i + 1),(float)bitmapImage[indexRU] * scale,(float)(j + 1) };
			TerrInfor.Texure = { IncrementW + IncrementTex ,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// ������ �Ʒ�
			TerrInfor.Position = { (float)(i + 1),(float)bitmapImage[indexRD] * scale,(float)j };
			TerrInfor.Texure = { IncrementW + IncrementTex ,IncrementH + IncrementTex };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 
			wCount++;
			if (wCount == incrementCount)
			{
				wCount = 0;
			}
		}
		hCount++;
		if (hCount == incrementCount)
		{
			hCount = 0;
		}
	}
	// �ε��� ����
	for (size_t i = 0; i < Mesh->vTerrainVertex.size(); ++i)
	{
		Mesh->vIndex.push_back(i);
	}




	// �ֱ�
	m_mTerrain.insert(pair<const string, unique_ptr<TerrainMesh>>(name, move(Mesh)));



	delete[] bitmapImage;
	bitmapImage = nullptr;


	return true;
}
/*bool TerrainMapLoader::LoadHeightMap(const std::string& name, const std::string& Directory)
{
	FILE* filePtr = nullptr;

	// ���� �б�
	if (fopen_s(&filePtr, Directory.c_str(), "rb") != 0)
	{
		return false;
	}

	// ��Ʈ������ �б�
	BITMAPFILEHEADER bitmapFileHeader;
	if (fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr) != 1)
	{
		return false;
	}

	BITMAPINFOHEADER bitmapInforHeader;
	if (fread(&bitmapInforHeader, sizeof(BITMAPINFOHEADER), 1, filePtr) != 1)
	{
		return false;
	}

	// ���� ũ�� ����
	float Width = (float)bitmapInforHeader.biWidth;
	float Height = (float)bitmapInforHeader.biHeight;


	// ��Ʈ�� �̹��� ������ ũ�� ���
	int imageSize = (int)Width * (int)Height * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// ��Ʈ�� �������� ���� �κ����� �̵�
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// ��Ʈ�� �̹��� �����͸� �д´�.
	if (fread(bitmapImage, 1, imageSize, filePtr) != imageSize)
	{
		return false;
	}
	// ���� �ݱ�
	if (fclose(filePtr) != 0)
	{
		return false;
	}

	// ���� �� �����͸� ������ ����ü�� �����.
	unique_ptr<TerrainMesh> tmpMesh = make_unique<TerrainMesh>();

	TerrainVertex vertex;

	//TerrainVertex* HeightMapVertex = new TerrainVertex[Height*Width];
	//if (!HeightMapVertex) return false;


	// �̹��� �����ͷ� ���̸��� �о�帰��.
	int pos = 0;
	int index = 0;
	for (size_t i = 0; i < Height; ++i)
	{
		for (size_t j = 0; j < Width; ++j)
		{
			float height = (float)bitmapImage[pos] * 0.1f;
			int idx = ((int)Height * i) + j;

			vertex.Position.x = (float)i;
			vertex.Position.y = (float)height;
			vertex.Position.z = (float)j;
			tmpMesh->vTerrainVertex.push_back(vertex);
			tmpMesh->vIndex.push_back(index);
			index++;
			pos += 3;
		}
	}


	//  �ؽ��� ��ǥ ����
	//ComputeTextureCoord(tmpMesh.get(), Width, Height);




	unique_ptr<TerrainMesh> Mesh = make_unique<TerrainMesh>();
	// ���̸��� �̿��� ������ �����Ѵ�.
	index = 0;
	for (size_t j = 0; j < Height - 1; ++j)
	{
		for (size_t i = 0; i < Width - 1; ++i)
		{
			int index1 = ((int)Height * j) + i;         //���� �Ʒ�
			int index2 = ((int)Height * j) + (i + 1);     //������ �Ʒ�
			int index3 = ((int)Height * (j + 1)) + i;     // ���� ��
			int index4 = ((int)Height * (j + 1)) + (i + 1); // ������ ��

															//Line 1
															// ���� ��
															// ��� �����ڸ��� ������ �ؽ��� ��ǥ�� ����

			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index3]);
			Mesh->vIndex.push_back(index);
			index++;



			// ������ ��
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;

			// Line 2
			// ������ ��
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;
			// ���� �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;


			//Line3
			// ���� �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;
			// ���� ��
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index3]);
			Mesh->vIndex.push_back(index);
			index++;

			//Line4
			// ���� �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;

			// ������ ��
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;

			// Line5
			// ������ ��
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;
			// ������ �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index2]);
			Mesh->vIndex.push_back(index);
			index++;

			//Line 6
			// ������ �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index2]);
			Mesh->vIndex.push_back(index);
			index++;

			// ���� �Ʒ�
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;

		}
	}


	// �ֱ�
	m_mTerrain.insert(pair<const string, unique_ptr<TerrainMesh>>(name, move(Mesh)));



	delete[] bitmapImage;
	bitmapImage = nullptr;


	return true;
}*/
