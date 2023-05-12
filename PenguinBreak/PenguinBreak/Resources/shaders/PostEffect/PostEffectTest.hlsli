
cbuffer cbuff0 : register(b0)
{
	matrix world;
	float4 color;
	float time;
	bool isAlive;
};
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TECOORD;
};