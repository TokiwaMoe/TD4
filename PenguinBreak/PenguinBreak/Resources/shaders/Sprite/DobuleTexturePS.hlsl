
#include "DobuleTexture.hlsli"

Texture2D<float4> tex1 : register(t0);//0�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
Texture2D<float4> tex2 : register(t1);//1�ԃX���b�g�Ɏw�肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);//�O�ԃX���b�g�Ɏw�肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//tex1 �e�N�X�`���P����
	//tex2�e�N�X�`���Q����


	return  tex1.Sample(smp,input.uv) * color;

}

