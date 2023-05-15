
#include "PostEffectTest.hlsli"

Texture2D<float4> tex : register(t0);//0番スロットに指定されたテクスチャ
SamplerState smp : register(s0);//０番スロットに指定されたサンプラー

float WhiteNoise(float2 coord)
{
    return frac(sin(dot(coord, float2(8.7819, 3.255))) * 437.645);
}

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

     //グリッチのパラメーターの計算
     float VertGlitchPase = 0.5f;
     float HorzGlitchPase = 0.5f;
     float GlicthStepValue = 0.05f;
     float vertNoise = WhiteNoise(float2(floor((input.uv.x) / VertGlitchPase) * VertGlitchPase, time * 0.1));
     float horzNoise = WhiteNoise(float2(floor((input.uv.x) / HorzGlitchPase) * HorzGlitchPase, time * 0.2));
     float vertGlitchStrength = vertNoise / GlicthStepValue;
     float horzGlitchStrength = horzNoise / GlicthStepValue;
     vertGlitchStrength = vertGlitchStrength * 2.0f - 1.0f;
     horzGlitchStrength = horzGlitchStrength * 2.0f - 1.0f;
     float V = step(vertNoise, GlicthStepValue * 2) * vertGlitchStrength;
     float H = step(horzNoise, GlicthStepValue * 2) * horzGlitchStrength;
     //一定周期でグリッチを有効
     float4 Tex = tex.Sample(smp, input.uv);
     float sinv = sin(input.uv.y * 2 - time * -0.1f);
     float steped = 1 - step(0.99f, sinv * sinv);
     float timeFrac;
     if (isAlive)
     {
         timeFrac = steped * 1;
     }
     else
     {
         timeFrac = steped * 0;
     }
     //rずらし
     input.uv.x += timeFrac * (V + H);
     Tex.r = tex.Sample(smp, input.uv).r;
     //gずらし
     input.uv.x += 0.6f * timeFrac * (V + H);
     Tex.g = tex.Sample(smp, input.uv).g;
     //bずらし
     input.uv.x += 0.3f * timeFrac * (V + H);
     Tex.b = tex.Sample(smp, input.uv).b;

     return float4(Tex.rgb, 1.0f);
}

