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
// 나도 목적을 모르겠음
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

	// 위치를 점으로 간주..
    input.position.w = 1.0f;

	// 월드 - 뷰 - 투영 순으로 좌표 변환
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	//월드 좌표를따로가져온다.(나중에 최적화 ㄱㄱ)
	output.worldPos = mul(input.position, worldMatrix);
	// 텍스쳐 좌표
	output.tex = input.tex;

	// 노멀 벡터 월드좌표화
	output.normal = mul(input.normal, (float3x3)worldMatrix);

    return output;
}