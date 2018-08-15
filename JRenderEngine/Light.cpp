#include"Light.h"
using namespace JRenderEngine;



Light::Light()
{

}
Light::~Light()
{

}


void Light::SetDirection(const D3DXVECTOR4& direction)
{
	m_Direction = direction;
}
void Light::SetLightColor(const D3DXVECTOR4& LightColor)
{
	m_LightColor = LightColor;
}