#pragma once
#include"JRenderEngine.h"
#include<map>
#include<vector>

namespace JRenderEngine
{
	typedef struct TerrainVertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Texure = { 0.0f,0.0f };
		D3DXVECTOR3 Normal;

		//D3DXVECTOR4 Color = { 1.0f,1.0f,1.0f,1.0f };
	}TerrainVertex;
	typedef unsigned int Index;
	typedef struct TerrainMesh
	{
		std::vector<TerrainVertex> vTerrainVertex;
		std::vector<Index> vIndex;
	}TerrainMesh;

	class TerrainMapLoader
	{
		static std::map<const std::string,std::unique_ptr<TerrainMesh>> m_mTerrain;
		static float m_TextureWrap;
	public:
		TerrainMapLoader();
		~TerrainMapLoader();


		bool TerrainMapInit();
		static TerrainMesh* LoadTerrain(const std::string& name);
		static void LoadDefaultGridMap(TerrainMesh* Mesh,const size_t Width, const size_t Height);
	
	protected:
		bool LoadHeightMap(const std::string& name, const std::string& Directory);
	};
}	