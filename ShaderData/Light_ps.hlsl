////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


// LightBuffer : 빛 관련 파라미터
// @param : AmbientDown : 간접광 밑 부분 빛 세기
// @param : AmbientUp   : 간접광 윗 부분 빛 세기
// @param : DirToLight  : 빛이 오고있는 벡터( 빛위치 - 물체위치 )정규화한거
// @param : DirLightColor : 빛 색
cbuffer LightBuffer : register(b0)
{
	float3 AmbientDown : packoffset(c0);
	float3 AmbientUp : packoffset(c1);
	float3 DirToLight : packoffset(c2);
	float3 DirLightColor : packoffset(c3);
}
// ObjectBuffer : 오브젝트관련 파라미터
// @param EyePosition : 카메라위치
// @param specExp     : 반사 강도 
cbuffer ObjectBuffer : register(b1)
{
	float3 EyePosition : packoffset(c0);
	float specExp : packoffset(c0.w);
}
//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

float3 CalcAmbient(float3 normal, float3 color)
{
	//[-1,1] 에서 [0,1]로 변환
	// 노멀 벡터 y좌표( 위쪽으로 향하는 점[ -1 ~ 1])을 색을 나타내는 범위( 0 ~ 1 )로 치환
	float up = normal.y * 0.5f + 0.5f;

	// 밑을 담당하는 간접광 + (위쪽노멀벡터값 * 위를 담당하는 간접광)
	float3 Ambient = AmbientDown + up * AmbientUp;

	// 텍스쳐 컬러와 간접광을 곱한다.
	return Ambient * color;
}

// Material
// @param normal : 노멀벡터
// @param diffuseColor : 난반사 색
// @param specExp : 반사 강도 
// @param specIntesity : 감쇄 계수
struct Material
{
	float3 normal;
	float4 diffuseColor;
	float specExp;
	float specIntensity;
};

// @func
Material MaterialInit(float3 normal, float2 uv)
{
	Material material;

	// 노멀벡터를일단 정규화
	material.normal = normalize(normal);

	// 
	material.diffuseColor = shaderTexture.Sample(SampleType, uv);

	//
	material.specExp = specExp;


	// 감쇄 계수를 구한다.
	float specIntensity = saturate(dot(normal, -DirToLight));
	material.specIntensity = specIntensity;
	return material;
}

float3 CalcDirectional(float3 position, Material material)
{
	//반사광 = 반사색 * (반사된빛의색 * ((법선과 하프벡터의 내적) ^ 반사강도) * 감쇄계수 * 스폿라이트계수)

	// 노멀벡터와 빛과의 내적을 구한다.
	float NDotL = dot(DirToLight, material.normal);

	// 최종 색(주변광)
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);

	// 카메라 위치를 토대로 카메라 위치 벡터를 만든다.
	float3 ToEye = EyePosition.xyz - position;
	ToEye = normalize(ToEye);

	// 
	float3 HalfWay = normalize(ToEye + DirToLight);
	float NDotH = saturate(dot(HalfWay, material.normal));
	finalColor += DirLightColor.rgb * pow(NDotH, material.specExp) * material.specIntensity;

	return finalColor * material.diffuseColor.rgb;
}
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET0
{
	/*float3 normal = normalize(input.normal);
	float4 color;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
	color = shaderTexture.Sample(SampleType, input.tex);
	color = float4(color.rgb * color.rgb, color.a);

	return color;*/

	Material material = MaterialInit(input.normal, input.tex);
    float3 color = CalcAmbient(material.normal, material.diffuseColor.rgb);
	color += CalcDirectional(input.worldPos, material);

	return float4(color, 1.0f);
}
