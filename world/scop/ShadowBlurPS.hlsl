Texture2D shadow_map : register(t3);

SamplerState sampler0
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = clamp;
    AddressV = clamp;
    AddressW = clamp;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer Offset : register(b1)
{
    float gWeights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f,
        0.1f, 0.1f, 0.1f, 0.05f, 0.05f
    };
};


float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}
