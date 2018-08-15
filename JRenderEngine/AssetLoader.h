#pragma once

#include"JRenderEngine.h"
#include"Mesh.h"
#include<map>
#include<string>

/* 작업 일지 
* 여기에 나중에 FBXLoader클래스를 따로만들어 접목시켜 메쉬를 불러온다.
* 일단 예제로 주어준 txt파일이나 3dMax.OBJ 만 쓰도록하자.
*/

namespace JRenderEngine
{
	class AssetLoader
	{
		static std::map<const std::string, std::unique_ptr<Mesh> > m_mAsset;
		static ID3D11Device* m_pDevice;
		static ID3D11DeviceContext* m_pDeviceContext;
	public:
		AssetLoader();
		~AssetLoader();


		bool AssetInit(HWND hWnd);
		static Mesh* Load(const std::string name);

	private:
		bool AssetTxTLoad(const std::string AssetName, const std::string AssetDirectory);
		void MeshCalculate(Mesh* mesh);
		void CalculateTangentBinormal(struct Vertex vertex1, struct Vertex vertex2, struct Vertex vertex3,
			D3DXVECTOR3* tangent, D3DXVECTOR3* binormal);
		void CalculateNormal(D3DXVECTOR3* normal, D3DXVECTOR3* tangent, D3DXVECTOR3* binormal);
	};
}


