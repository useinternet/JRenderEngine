////////////////////////////////////////////////////////////////////////////////
// Filename: texture.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
/////////////////////////////////////////////////////////////////////////////////
// ���� ������ �𸣰���
float4 DepthPrePassVS(float4 position : POSITION) : SV_POSITION
{
	position = mul(position, worldMatrix);
    position = mul(position, viewMatrix);
    position = mul(position, projectionMatrix);
	return position;
}
PixelInputType main(VertexInputType input)
{
    PixelInputType output;

	// ��ġ�� ������ ����..
    input.position.w = 1.0f;

	// ���� - �� - ���� ������ ��ǥ ��ȯ
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	//���� ��ǥ�����ΰ����´�.(���߿� ����ȭ ����)
	output.worldPos = mul(input.position, worldMatrix);
	// �ؽ��� ��ǥ
	output.tex = input.tex;

	// ��� ���� ������ǥȭ
	output.normal = mul(input.normal, (float3x3)worldMatrix);

    return output;
}