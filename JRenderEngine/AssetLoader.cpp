#include "AssetLoader.h"
#include "DirectX.h"
#include <fstream>
using namespace JRenderEngine;
using namespace std;
std::map<const std::string, std::unique_ptr<class Mesh>> AssetLoader::m_mAsset;
ID3D11Device* AssetLoader::m_pDevice;
ID3D11DeviceContext* AssetLoader::m_pDeviceContext;

AssetLoader::AssetLoader()
{
	DirectX* direct = DirectX::GetInstance();
	if (direct != nullptr)
	{
		m_pDevice        = direct->GetDevice();
		m_pDeviceContext = direct->GetDeviceContext();
	}
}
AssetLoader::~AssetLoader()
{

}

bool AssetLoader::AssetInit(HWND hWnd)
{

	// 예외처리 할것
	AssetTxTLoad("Bath", "../AssetModel/Bath.txt");
	AssetTxTLoad("Ground", "../AssetModel/Ground.txt");
	AssetTxTLoad("Plane", "../AssetModel/Plane.txt");
	AssetTxTLoad("Square", "../AssetModel/Square.txt");
	AssetTxTLoad("Wall", "../AssetModel/Wall.txt");
	AssetTxTLoad("Water", "../AssetModel/Water.txt");
	AssetTxTLoad("Box", "../AssetModel/Box.txt");
	return true;
}
Mesh* AssetLoader::Load(const std::string name)
{
#ifdef _DEBUG
	for (auto& iter : m_mAsset)
	{
		if (iter.first == name)
		{
			return iter.second.get();
		}
	}
	DebugLog(L"Error : The name of the mesh to load does not exist. \n");
	DebugLog(L"Class : AssetLoader \n");
	return nullptr;
#endif

#ifndef _DEBUG
	return m_mAsset[name].get();
#endif

}
bool AssetLoader::AssetTxTLoad(const std::string AssetName, const std::string AssetDirectory)
{
	unique_ptr<Mesh> mesh = make_unique<Mesh>();
	ifstream fin;
	char buffer = 0;
	Vertex vertex;

	// 파일 오픈
	fin.open(AssetDirectory, ios_base::in);

	if (!fin.is_open())
	{
		DebugLog(L"Error : Failed AssetTxtLoad \n");
		return false;
	}
	// 파일 읽기
	while (buffer != ':')
	{
		fin >> buffer;
	}
	fin >> buffer;
	fin >> buffer;
	while (buffer != ':')
	{
		fin >> buffer;
	}
	while (!fin.eof())
	{
		fin >> vertex.position.x;
		fin >> vertex.position.y;
		fin >> vertex.position.z;

		fin >> vertex.texture.x;
		fin >> vertex.texture.y;

		fin >> vertex.normal.x;
		fin >> vertex.normal.y;
		fin >> vertex.normal.z;

		mesh->Vertecies.push_back(vertex);
	}

	// tangent / binormal 벡터 계산
	MeshCalculate(mesh.get());

	for (size_t i = 0; i < mesh->Vertecies.size(); ++i)
	{
		mesh->Indecies.push_back(i);
	}

	// 에셋 저장고에 저장
	m_mAsset.insert(pair<const string, unique_ptr<Mesh>>(AssetName, move(mesh)));
	return true;
}
void AssetLoader::MeshCalculate(Mesh* mesh)
{
	D3DXVECTOR3 tagent;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 normal;
	size_t index = 0;
	for (size_t i = 0; i< mesh->Indecies.size() / 3; i++)
	{
		Vertex vertex1 = mesh->Vertecies[index];
		index++;
		Vertex vertex2 = mesh->Vertecies[index];
		index++;
		Vertex vertex3 = mesh->Vertecies[index];
		index++;


		CalculateTangentBinormal(vertex1, vertex2, vertex3, &tagent, &binormal);
		CalculateNormal(&normal, &tagent, &binormal);


		mesh->Vertecies[index - 1].tangent = tagent;
		mesh->Vertecies[index - 1].binormal = binormal;
		mesh->Vertecies[index - 1].normal = normal;


		mesh->Vertecies[index - 2].tangent = tagent;
		mesh->Vertecies[index - 2].binormal = binormal;
		mesh->Vertecies[index - 2].normal = normal;


		mesh->Vertecies[index - 3].tangent = tagent;
		mesh->Vertecies[index - 3].binormal = binormal;
		mesh->Vertecies[index - 3].normal = normal;
	}
}
void AssetLoader::CalculateTangentBinormal(Vertex vertex1, Vertex vertex2, Vertex vertex3,
	D3DXVECTOR3* tangent, D3DXVECTOR3* binormal)
{
	D3DXVECTOR3 v1 = vertex1.position;
	D3DXVECTOR3 v2 = vertex2.position;
	D3DXVECTOR3 v3 = vertex3.position;
	D3DXVECTOR2 w1 = vertex1.texture;
	D3DXVECTOR2 w2 = vertex2.texture;
	D3DXVECTOR2 w3 = vertex3.texture;

	float x1 = v2.x - v1.x;
	float x2 = v3.x - v1.x;
	float y1 = v2.y - v1.y;
	float y2 = v3.y - v1.y;
	float z1 = v2.z - v1.z;
	float z2 = v3.z - v1.z;

	float s1 = w2.x - w1.x;
	float s2 = w3.x - w1.x;
	float t1 = w2.y - w1.y;
	float t2 = w3.y - w1.y;


	float r = 1.0F / (s1 * t2 - s2 * t1);
	(*tangent) = { (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
		(t2 * z1 - t1 * z2) * r };
	(*binormal) = { (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
		(s1 * z2 - s2 * z1) * r };

	D3DXVec3Normalize(tangent, tangent);
	D3DXVec3Normalize(binormal, binormal);
}
void AssetLoader::CalculateNormal(D3DXVECTOR3* normal, D3DXVECTOR3* tangent, D3DXVECTOR3* binormal)
{
	float length = 0.0f;

	normal->x = (tangent->y * binormal->z) - (tangent->z * binormal->y);
	normal->y = (tangent->z * binormal->x) - (tangent->x * binormal->z);
	normal->z = (tangent->x * binormal->y) - (tangent->y * binormal->x);


	length = sqrt((normal->x * normal->x) + (normal->y * normal->y) + (normal->z * normal->z));

	normal->x = normal->x / length;
	normal->y = normal->y / length;
	normal->z = normal->z / length;
}