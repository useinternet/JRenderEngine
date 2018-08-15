////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


// LightBuffer : �� ���� �Ķ����
// @param : AmbientDown : ������ �� �κ� �� ����
// @param : AmbientUp   : ������ �� �κ� �� ����
// @param : DirToLight  : ���� �����ִ� ����( ����ġ - ��ü��ġ )����ȭ�Ѱ�
// @param : DirLightColor : �� ��
cbuffer LightBuffer : register(b0)
{
	float3 AmbientDown : packoffset(c0);
	float3 AmbientUp : packoffset(c1);
	float3 DirToLight : packoffset(c2);
	float3 DirLightColor : packoffset(c3);
}
// ObjectBuffer : ������Ʈ���� �Ķ����
// @param EyePosition : ī�޶���ġ
// @param specExp     : �ݻ� ���� 
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
	//[-1,1] ���� [0,1]�� ��ȯ
	// ��� ���� y��ǥ( �������� ���ϴ� ��[ -1 ~ 1])�� ���� ��Ÿ���� ����( 0 ~ 1 )�� ġȯ
	float up = normal.y * 0.5f + 0.5f;

	// ���� ����ϴ� ������ + (���ʳ�ֺ��Ͱ� * ���� ����ϴ� ������)
	float3 Ambient = AmbientDown + up * AmbientUp;

	// �ؽ��� �÷��� �������� ���Ѵ�.
	return Ambient * color;
}

// Material
// @param normal : ��ֺ���
// @param diffuseColor : ���ݻ� ��
// @param specExp : �ݻ� ���� 
// @param specIntesity : ���� ���
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

	// ��ֺ��͸��ϴ� ����ȭ
	material.normal = normalize(normal);

	// 
	material.diffuseColor = shaderTexture.Sample(SampleType, uv);

	//
	material.specExp = specExp;


	// ���� ����� ���Ѵ�.
	float specIntensity = saturate(dot(normal, -DirToLight));
	material.specIntensity = specIntensity;
	return material;
}

float3 CalcDirectional(float3 position, Material material)
{
	//�ݻ籤 = �ݻ�� * (�ݻ�Ⱥ��ǻ� * ((������ ���������� ����) ^ �ݻ簭��) * ������ * ��������Ʈ���)

	// ��ֺ��Ϳ� ������ ������ ���Ѵ�.
	float NDotL = dot(DirToLight, material.normal);

	// ���� ��(�ֺ���)
	float3 finalColor = DirLightColor.rgb * saturate(NDotL);

	// ī�޶� ��ġ�� ���� ī�޶� ��ġ ���͸� �����.
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
