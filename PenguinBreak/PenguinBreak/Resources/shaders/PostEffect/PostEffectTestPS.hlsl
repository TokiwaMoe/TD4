
#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー



float wave(float2 uv, float2 emitter, float speed, float phase) {
    float _Time = time;
    float dst = distance(uv, emitter);
    return pow((0.5 + 0.5 * sin(dst * phase - _Time * speed)), 2.0);
}

float4 main(VSOutput input) : SV_TARGET
{
     float2 position = input.uv;
     float _Speed = 10.0;
     float _Power = 0.01;

     float w = wave(position, float2(0.5, 0.5), _Speed, 200.0);
     w += wave(position, float2(0.6, 0.11), _Speed, 20.0);
     w += wave(position, float2(0.9, 0.6), _Speed, 90.0);
     w += wave(position, float2(0.1, 0.84), _Speed, 150.0);
     w += wave(position, float2(0.32, 0.81), _Speed, 150.0);
     w += wave(position, float2(0.16, 0.211), _Speed, 150.0);
     w += wave(position, float2(0.39, 0.46), _Speed, 150.0);
     w += wave(position, float2(0.51, 0.484), _Speed, 150.0);
     w += wave(position, float2(0.732, 0.91), _Speed, 150.0);

     w *= 0.116 * _Power;

     input.uv += w;

     return float4(tex.Sample(smp, input.uv).rgb,1.0f);
}

