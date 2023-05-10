
#include "DobuleTexture.hlsli"

Texture2D<float4> tex1 : register(t0);//0番スロットに指定されたテクスチャ
Texture2D<float4> tex2 : register(t1);//1番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	//tex1 テクスチャ１枚目
	//tex2テクスチャ２枚目


	return  tex1.Sample(smp,input.uv) * color;

}

