struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

SamplerState sampler0 : register(s0);
Texture2D texture0 : register(t0);

cbuffer constatData : register(b0)
{
    float dx;
    float dy;
};

static float threshold = 0.0f;

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler0, input.uv);
    if (color.x + color.y + color.z >= threshold)
        return float4(color.rgb, 0);
    return float4(0.0f, 0.0f, 0.0f, 0);
}