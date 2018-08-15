////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;


cbuffer ColorBuffer
{
	float4 pixelColor;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
	float4 color;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    color = shaderTexture.Sample(SampleType, input.tex);

	if (color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.rgb = pixelColor.rgb;
		color.a = 1.0f;
	}



	return color;//float4(1.0f,1.0f,1.0f,1.0f);
}
