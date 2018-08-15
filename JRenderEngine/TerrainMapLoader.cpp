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
			// 왼쪽 위
			TerrInfor.Position = { (float)i, 0.0f , (float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 오른쪽 위
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 2 
			// 오른쪽 위
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)(j + 1) };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 오른쪽 아래
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 3
			// 오른쪽 아래
			TerrInfor.Position = { (float)(i + 1),0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 왼쪽 아래
			TerrInfor.Position = { (float)i ,0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);

			// Line 4
			// 왼쪽 아래
			TerrInfor.Position = { (float)i ,0.0f,(float)j };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 왼쪽 위
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

	// 파일 읽기
	if (fopen_s(&filePtr, Directory.c_str(), "rb") != 0)
	{
		return false;
	}
	/* 비트맵에 관한 설명

	- 비트맵 파일 구조

	파일 헤더(BITMAPFILEHEADER) -> 영상 정보 헤더(BITMAPINFOHEADER) -> 팔레트 -> 픽셀 데이터

	// 픽셀 데이터만 불러들어와 이거에 기반해 높이를 구해 지형의 vertex값의 위치값을 구한다.


	typedef struct tagBITMAPFILEHEADER
	{
	WORD  bfType;                         // 비트맵을 뜻하는 'BM' 알파벳이 셋팅된다.
	DWORD bfSize;                         // 파일 전체 크기를 표시한다.
	WORD  bfReserved1;                    // 사용 안함
	WORD  bfReserved2;                    // 사용 안함
	DWORD bfOffBits;                      // 실제 픽셀 데이터의 시작 offset 주소를 바이트 수로 나타낸다.
	}

	// 비트맵 파일은 영상이 상하 반전이 되어 저장된다.

	자세한 내용은
	http://edps5091.tistory.com/entry/bitmap%ED%8C%8C%EC%9D%BC%EA%B5%AC%EC%A1%B0-1 참조..

	*/


	// 비트맵정보 읽기
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

	// 지형 크기 저장
	float Width = (float)bitmapInforHeader.biWidth;
	float Height = (float)bitmapInforHeader.biHeight;


	// 비트맵 이미지 데이터 크기 계산
	int imageSize = (int)Width * (int)Height * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// 비트맵 데이터의 시작 부분으로 이동
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// 비트맵 이미지 데이터를 읽는다.
	if (fread(bitmapImage, 1, imageSize, filePtr) != imageSize)
	{
		return false;
	}
	// 파일 닫기		

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
	// 볼텍스 구하기
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
			// 인덱스를 넘어섯을때 평탄화
			if (indexRD >= imageSize) indexRD = imageSize - 1;
			if (indexLU >= imageSize) indexLU = indexLD;
			if (indexRU >= imageSize) indexRU = indexRD;
			// Line 1 
			// 왼쪽 아래
			TerrInfor.Position = { (float)i ,(float)bitmapImage[indexLD] * scale,(float)j };
			TerrInfor.Texure = { IncrementW,IncrementH + IncrementTex };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 왼쪽 위
			TerrInfor.Position = { (float)i, (float)bitmapImage[indexLU] * scale , (float)(j + 1) };
			TerrInfor.Texure = { IncrementW,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 오른쪽 위
			TerrInfor.Position = { (float)(i + 1),(float)bitmapImage[indexRU] * scale,(float)(j + 1) };
			TerrInfor.Texure = { IncrementW + IncrementTex,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);


			// Line 2 
			// 왼쪽 아래
			TerrInfor.Position = { (float)i ,(float)bitmapImage[indexLD] * scale,(float)j };
			TerrInfor.Texure = { IncrementW ,IncrementH + IncrementTex };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 오른쪽 위
			TerrInfor.Position = { (float)(i + 1),(float)bitmapImage[indexRU] * scale,(float)(j + 1) };
			TerrInfor.Texure = { IncrementW + IncrementTex ,IncrementH };
			Mesh->vTerrainVertex.push_back(TerrInfor);
			// 오른쪽 아래
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
	// 인덱스 구성
	for (size_t i = 0; i < Mesh->vTerrainVertex.size(); ++i)
	{
		Mesh->vIndex.push_back(i);
	}




	// 넣기
	m_mTerrain.insert(pair<const string, unique_ptr<TerrainMesh>>(name, move(Mesh)));



	delete[] bitmapImage;
	bitmapImage = nullptr;


	return true;
}
/*bool TerrainMapLoader::LoadHeightMap(const std::string& name, const std::string& Directory)
{
	FILE* filePtr = nullptr;

	// 파일 읽기
	if (fopen_s(&filePtr, Directory.c_str(), "rb") != 0)
	{
		return false;
	}

	// 비트맵정보 읽기
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

	// 지형 크기 저장
	float Width = (float)bitmapInforHeader.biWidth;
	float Height = (float)bitmapInforHeader.biHeight;


	// 비트맵 이미지 데이터 크기 계산
	int imageSize = (int)Width * (int)Height * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// 비트맵 데이터의 시작 부분으로 이동
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// 비트맵 이미지 데이터를 읽는다.
	if (fread(bitmapImage, 1, imageSize, filePtr) != imageSize)
	{
		return false;
	}
	// 파일 닫기
	if (fclose(filePtr) != 0)
	{
		return false;
	}

	// 높이 맵 데이터를 저장할 구조체를 만든다.
	unique_ptr<TerrainMesh> tmpMesh = make_unique<TerrainMesh>();

	TerrainVertex vertex;

	//TerrainVertex* HeightMapVertex = new TerrainVertex[Height*Width];
	//if (!HeightMapVertex) return false;


	// 이미지 데이터로 높이맵을 읽어드린다.
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


	//  텍스쳐 좌표 산출
	//ComputeTextureCoord(tmpMesh.get(), Width, Height);




	unique_ptr<TerrainMesh> Mesh = make_unique<TerrainMesh>();
	// 높이맵을 이용해 지형을 생성한다.
	index = 0;
	for (size_t j = 0; j < Height - 1; ++j)
	{
		for (size_t i = 0; i < Width - 1; ++i)
		{
			int index1 = ((int)Height * j) + i;         //왼쪽 아래
			int index2 = ((int)Height * j) + (i + 1);     //오른쪽 아래
			int index3 = ((int)Height * (j + 1)) + i;     // 왼쪽 위
			int index4 = ((int)Height * (j + 1)) + (i + 1); // 오른쪽 위

															//Line 1
															// 왼쪽 위
															// 상단 가장자리를 덮도록 텍스쳐 좌표를 수정

			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index3]);
			Mesh->vIndex.push_back(index);
			index++;



			// 오른쪽 위
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;

			// Line 2
			// 오른쪽 위
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;
			// 왼쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;


			//Line3
			// 왼쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;
			// 왼쪽 위
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index3]);
			Mesh->vIndex.push_back(index);
			index++;

			//Line4
			// 왼쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;

			// 오른쪽 위
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;

			// Line5
			// 오른쪽 위
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index4]);
			Mesh->vIndex.push_back(index);
			index++;
			// 오른쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index2]);
			Mesh->vIndex.push_back(index);
			index++;

			//Line 6
			// 오른쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index2]);
			Mesh->vIndex.push_back(index);
			index++;

			// 왼쪽 아래
			Mesh->vTerrainVertex.push_back(tmpMesh->vTerrainVertex[index1]);
			Mesh->vIndex.push_back(index);
			index++;

		}
	}


	// 넣기
	m_mTerrain.insert(pair<const string, unique_ptr<TerrainMesh>>(name, move(Mesh)));



	delete[] bitmapImage;
	bitmapImage = nullptr;


	return true;
}*/
