#pragma once
#include"Actor.h"
#include<vector>

namespace JRenderEngine
{

	class Terrain : public Actor
	{
	private:
		size_t m_TerrainWidth;
		size_t m_TerrainHeight;

		size_t IndexSize;
	public:
		Terrain();
		virtual ~Terrain();
		virtual bool Update(const float deltaTime);
		void SetGrid(const size_t Width,const size_t Height);
		size_t GetIndexSize();
	};
}