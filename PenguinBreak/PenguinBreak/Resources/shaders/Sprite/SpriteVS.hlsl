#include "Sprite.hlsli"


VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;
	output.svpos = mul(world, pos);
	output.uv = uv;

	return output;
}