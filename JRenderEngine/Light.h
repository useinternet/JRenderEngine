#pragma once
#include"GameObject.h"


namespace JRenderEngine
{
	class Light : public GameObject
	{
	private:
		D3DXVECTOR4 m_Direction;
		D3DXVECTOR4 m_LightColor;
	public:
		Light();
		virtual ~Light();


		void SetDirection(const D3DXVECTOR4& direction);
		void SetLightColor(const D3DXVECTOR4& LightColor);

	};
}