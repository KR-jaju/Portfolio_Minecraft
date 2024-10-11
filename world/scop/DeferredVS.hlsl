struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer MVP : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
}

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.uv = input.uv;
    return output;
}