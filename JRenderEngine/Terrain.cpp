#include"Terrain.h"
#include"RenderComponent.h"
#include"TerrainMapLoader.h"
#include"Mesh.h"
using namespace JRenderEngine;
using namespace std;




Terrain::Terrain()
{
	SetName(ObjectID::Terrain);
}
Terrain::~Terrain()
{

}
bool Terrain::Update(const float deltaTime)
{
	Actor::Update(deltaTime);
	return true;
}
void Terrain::SetGrid(const size_t Width, const size_t Height)
{
	// ���� ������ ���´�.
	TerrainMesh mesh = *(TerrainMapLoader::LoadTerrain("HeightMap"));
	//TerrainMapLoader::CreateDefaultMap(&mesh,256, 256);
	IndexSize = mesh.vIndex.size();

	// �޽� �޼��� �ۼ�
	MeshMessage msg;
	msg.Flags = Vertex_Static | Index_Static;

	msg.VertexSize = sizeof(TerrainVertex);
	msg.VertexElementSize = mesh.vTerrainVertex.size();
	msg.VertexType = &(mesh.vTerrainVertex)[0];

	msg.IndexSize = sizeof(Index);
	msg.IndexElementSize = mesh.vIndex.size();
	msg.IndexType = &(mesh.vIndex)[0];

	// ������ ������Ʈ �޽��� �ۼ�
	RenderComponentMessage RenderMessage;
	RenderMessage.bDefault = false;
	RenderMessage.format = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	RenderMessage.MeshMsg = msg;

	// �޼��� �˸�
	Send(&RenderMessage);
}
size_t Terrain::GetIndexSize()
{
	return IndexSize;
}