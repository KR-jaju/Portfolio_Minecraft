SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.uv);
    return color;
}